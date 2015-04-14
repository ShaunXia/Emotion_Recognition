#include "emotion_detect.h"

Emotion_Detect::Emotion_Detect(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	timer = new QTimer(this);
	timer->setInterval(60);
	ui.video_status->setDisabled(true);
	//ui.predict_control->setDisabled(true);
	video_is_detect=0;
	is_show_landmark=0;
	get_neutral=0;
	neutral_face=Mat::zeros(640,780,CV_32FC1);
	string num2emo[100];
	ifstream map_file("model/ecode_map.txt");
	int tpcount=0;
	while (!map_file.eof())
	{
		int ecode;
		string estring;
		map_file>>estring>>ecode;
		num2emo[ecode]= estring;
		QString st = QString::fromStdString(estring);
		st.append(" - ");
		st.append(QString::number(ecode));
		vec_qlabel.push_back(new QLabel(st));
		vec_qlabel[tpcount]->setStyleSheet(QStringLiteral("font: 75 14pt 'Arial';color: black;"));
		ui.result_layout->addWidget(vec_qlabel[tpcount++]);

	}
	map_file.close();


	connect(timer,SIGNAL(timeout()),this,SLOT(startLoopSlot()));
	connect(ui.choose_path, SIGNAL(clicked()), this, SLOT(getFileList()));
	connect(ui.fileList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemclick(QListWidgetItem*)));
	connect(ui.video_status,SIGNAL(sliderPressed()),this,SLOT(slide_press()));
	connect(ui.video_status,SIGNAL(sliderReleased()),this,SLOT(slide_release()));
	connect(ui.but_capture,SIGNAL(clicked()),this,SLOT(but_capture()));
	connect(ui.but_loadlbf,SIGNAL(clicked()),this,SLOT(but_loadmodel()));
	connect(ui.but_start_detect,SIGNAL(clicked()),this,SLOT(but_start_detect()));
	connect(ui.but_show_landmark,SIGNAL(clicked()),this,SLOT(but_show_landmark()));

	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(but_tri()));


}

Emotion_Detect::~Emotion_Detect()
{
	timer->stop();
	cap.release();
}

void Emotion_Detect::getFileList()
{
	ui.fileList->clear();
	filePath = QFileDialog::getExistingDirectory(NULL, tr("Ñ¡ÔñÎÄ¼þ¼Ð"),"D:\\",QFileDialog::ShowDirsOnly);
	QDir dir(filePath);
	QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < list.size(); i++)
	{
		QFileInfo fileinfo = list.at(i);
		ui.fileList->insertItem(0,fileinfo.fileName());
	}
	ui.fileList->insertItem(0,"##CAM");
}

void Emotion_Detect::itemclick (QListWidgetItem *item)
{
	current_landMark.clear();
	current_landMark_fixed.clear();
	for (int i = 0; i < vec_qlabel.size(); i++)
	{
		vec_qlabel[i]->setStyleSheet(QStringLiteral("font: 75 14pt 'Arial';color: black;"));
	}

	QString tppath = filePath;
	tppath.append("/").append(item->text());
	if (item->text().compare("##CAM")!=0&&item->text().lastIndexOf("avi")==-1)
	{
		ui.video_status->setDisabled(true);
		captype=-1;
		if (timer->isActive())
		{
			timer->stop();
			cap.release();
		}


		// image FEATURE
		frame_ori=imread(tppath.toStdString());
		double scale = 456*1.0/frame_ori.rows;

		cv::resize(frame_ori,frame_ori,Size(frame_ori.cols*scale,frame_ori.rows*scale),0,0,CV_INTER_LINEAR);
		frame_with_landMark=frame_ori.clone();

		Mat show_fixed_point=Mat::zeros(640,780,CV_8UC3);

		if (video_is_detect==1)
		{

			int res=LPredict.predict(frame_ori);
			if (res!=-1)
			{
				vec_qlabel[res]->setStyleSheet(QStringLiteral("font: 75 14pt 'Arial';color: rgb(255, 0, 0);"));
			}

		}
		LPredict.getLandmark(frame_ori,current_landMark,current_landMark_fixed);
		if (is_show_landmark)
		{
			for (int i = 0; i < current_landMark.size(); i++)
			{
				//Show LANDMARK IN PIC
				Point center( current_landMark[i].x, current_landMark[i].y ); 
				ellipse( frame_with_landMark, center, Size( 1, 1), 0, 0, 0, Scalar( 255, 0, 255 ), 4, 8, 0); 
				
				line(frame_with_landMark, current_landMark[27], current_landMark[i], Scalar( 255, 255, 255 ), 1, CV_AA, 0);

				//Show Fixed Point 
				Point center1( current_landMark_fixed[i].x, current_landMark_fixed[i].y ); 
				ellipse( show_fixed_point, center1, Size( 1, 1), 0, 0, 0, Scalar( 255, 0, 255 ), 4, 8, 0); 
				char c[3];
				sprintf(c, "%d", i);
				string words= c;  
				putText( show_fixed_point, words, center1,CV_FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 0)); 

			}
			frame_forQTshow=frame_with_landMark.clone();
		}
		else
			frame_forQTshow=frame_ori.clone();
		cvtColor(frame_forQTshow, frame_forQTshow, CV_BGR2RGB); 
		QImage *qJpg = new QImage((unsigned char*)frame_forQTshow.data, // uchar* data  
			frame_forQTshow.cols, frame_forQTshow.rows, // width height
			frame_forQTshow.step, //bytesPerLine  
			QImage::Format_RGB888); //format  
		ui.picshow->setPixmap(QPixmap::fromImage(*qJpg));
		ui.picshow->resize(qJpg->size());
		imshow("x",show_fixed_point);
		waitKey(0);
	}
	else 
	{ 
		if (item->text().compare("##CAM")==0) //FROM CAM
		{
			ui.video_status->setDisabled(true);
			captype=0;
			if (!timer->isActive())
			{

				cap.open(0); // open the default camera
				if(!cap.isOpened())  // check if we succeeded
					return ;
				timer->start();
			}
			else
			{
				timer->stop();
				cap.release();
				cap.open(0); // open the default camera
				if(!cap.isOpened())  // check if we succeeded
					return ;
				timer->start();
			}
			cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
			cap.set(CV_CAP_PROP_FRAME_HEIGHT,160);
		}

		else // AVI file
		{
			ui.video_status->setDisabled(false);
			captype=1;
			current_frame=0;
			if (!timer->isActive())
			{

				cap.open(tppath.toStdString()); // open the default camera
				if(!cap.isOpened())  // check if we succeeded
					return ;

			}
			else
			{
				timer->stop();
				cap.release();
				cap.open(tppath.toStdString()); // open the default camera
				if(!cap.isOpened())  // check if we succeeded
					return ;
			}
			ui.video_status->setMaximum(cap.get(CV_CAP_PROP_FRAME_COUNT));
			timer->start();

		}
	}
}

void Emotion_Detect::startLoopSlot()
{
	current_landMark.clear();
	current_landMark_fixed.clear();
	for (int i = 0; i < vec_qlabel.size(); i++)
	{
		vec_qlabel[i]->setStyleSheet(QStringLiteral("font: 75 14pt 'Arial';color: black;"));
	}
	if (captype==1)
	{
		if (current_frame==ui.video_status->maximum())
		{
			current_frame=0;
			cap.set(CV_CAP_PROP_POS_FRAMES,0);
		}
		ui.video_status->setValue(current_frame++);
	}

	cap >> frame_ori; // get a new frame from camera
	frame_with_landMark=frame_ori.clone();
	if (video_is_detect==1)
	{
		int res=LPredict.predict(frame_ori);
		if (res!=-1)
		{
			vec_qlabel[res]->setStyleSheet(QStringLiteral("font: 75 14pt 'Arial';color: red;"));
		}

	}
	LPredict.getLandmark(frame_ori,current_landMark,current_landMark_fixed);
	if (is_show_landmark)
	{
		for (int i = 0; i < current_landMark.size(); i++)
		{
			Point center( current_landMark[i].x, current_landMark[i].y ); 
			ellipse( frame_with_landMark, center, Size( 1, 1), 0, 0, 0, Scalar( 255, 0, 255 ), 4, 8, 0); 

		}
		frame_forQTshow=frame_with_landMark.clone();
	}
	else
		frame_forQTshow=frame_ori.clone();

	Mat new_face= neutral_face.clone();
	for (int i = 0; i < current_landMark_fixed.size(); i++)
	{
		//show_neutral.at<float>(neutral_landMark[i].x,neutral_landMark[i].x);
		Point center;
		if (get_neutral)
		{
			Point pt=neutral_landMark[27]-current_landMark_fixed[27];
			center=Point( current_landMark_fixed[i].x+pt.x, current_landMark_fixed[i].y+pt.y ); 
		}
		else
			center=Point( current_landMark_fixed[i].x, current_landMark_fixed[i].y ); 
		ellipse( new_face, center, Size( 1, 1), 0, 0, 0, Scalar( 255), 4, 8, 0); 
	}
	imshow("neutral",new_face);
	waitKey(30);
	cvtColor(frame_forQTshow, frame_forQTshow, CV_BGR2RGB); 
	qCam = new QImage((unsigned char*)frame_forQTshow.data, // uchar* data  
		frame_forQTshow.cols, frame_forQTshow.rows, // width height  
		frame_forQTshow.step, //bytesPerLine  
		QImage::Format_RGB888); //format
	ui.picshow->setPixmap(QPixmap::fromImage(*qCam));
	ui.picshow->resize(qCam->size());


}

void Emotion_Detect::slide_press()
{
	timer->stop();
}
void Emotion_Detect::slide_release()
{
	cap.set(CV_CAP_PROP_POS_FRAMES,ui.video_status->value());
	current_frame=ui.video_status->value();
	timer->start();
}



void Emotion_Detect::but_loadmodel()
{
	LPredict.loadModel();
	ui.but_loadlbf->setText("Loaded!");
}

void Emotion_Detect::but_start_detect()
{
	video_is_detect=!video_is_detect;
	QString str("Detect:");
	str.append(QString::number(video_is_detect));
	ui.but_start_detect->setText(str);

}


void Emotion_Detect::but_show_landmark()
{
	is_show_landmark=!is_show_landmark;
	QString str("LandMark:");
	str.append(QString::number(is_show_landmark));
	ui.but_show_landmark->setText(str);
}


void Emotion_Detect::but_capture()
{
	get_neutral=1;
	neutral_landMark.clear();
	timer->stop();
	LPredict.getLandmark(frame_ori,neutral_landMark,neutral_landMark_fixed);
	neutral_face=Mat::zeros(640,780,CV_32FC1);
	for (int i = 0; i < neutral_landMark_fixed.size(); i++)
	{
		//show_neutral.at<float>(neutral_landMark[i].x,neutral_landMark[i].x);
		Point center( neutral_landMark_fixed[i].x, neutral_landMark_fixed[i].y ); 
		ellipse( neutral_face, center, Size( 1, 1), 0, 0, 0, Scalar( 255, 0, 255 ), 4, 8, 0); 
	}
	imshow("neutral",neutral_face);
	timer->start();
	waitKey(0);
}

void Emotion_Detect::draw_subdiv( Mat& img, Subdiv2D& subdiv, Scalar delaunay_color )
{
#if 1
	vector<Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	vector<Point> pt(3);

	for( size_t i = 0; i < triangleList.size(); i++ )
	{
		Vec6f t = triangleList[i];
		pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
		pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
		pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
		line(img, pt[0], pt[1], delaunay_color, 1, CV_AA, 0);
		line(img, pt[1], pt[2], delaunay_color, 1, CV_AA, 0);
		line(img, pt[2], pt[0], delaunay_color, 1, CV_AA, 0);
	}
#else
	vector<Vec4f> edgeList;
	subdiv.getEdgeList(edgeList);
	for( size_t i = 0; i < edgeList.size(); i++ )
	{
		Vec4f e = edgeList[i];
		Point pt0 = Point(cvRound(e[0]), cvRound(e[1]));
		Point pt1 = Point(cvRound(e[2]), cvRound(e[3]));
		line(img, pt0, pt1, delaunay_color, 1, CV_AA, 0);
	}
#endif
}

void Emotion_Detect::but_tri()
{
	Scalar active_facet_color(0, 0, 255), delaunay_color(255,255,255);
	Rect rect(0, 0, 1000, 1000);
	Subdiv2D subdiv(rect);
	Mat img(rect.size(), CV_8UC3);

	img = Scalar::all(0);
	string win = "Delaunay Demo";
	imshow(win, img);

	for( int i = 0; i < neutral_landMark.size(); i++ )
	{
		subdiv.insert(neutral_landMark[i]);
		img = Scalar::all(0);
	}
	draw_subdiv( img, subdiv, delaunay_color );
	imshow( win, img );
	waitKey(0);

}