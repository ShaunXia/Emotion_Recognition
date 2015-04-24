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
	}
	map_file.close();
	ui.label_res_0->setText(QString::fromStdString(num2emo[0]));
	ui.label_res_1->setText(QString::fromStdString(num2emo[1]));
	ui.label_res_2->setText(QString::fromStdString(num2emo[2]));
	ui.label_res_3->setText(QString::fromStdString(num2emo[3]));
	cap.set(CV_CAP_PROP_FRAME_WIDTH,240);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,160);

	connect(timer,SIGNAL(timeout()),this,SLOT(startLoopSlot()));
	connect(ui.choose_path, SIGNAL(clicked()), this, SLOT(getFileList()));
	connect(ui.fileList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemclick(QListWidgetItem*)));
	connect(ui.video_status,SIGNAL(sliderPressed()),this,SLOT(slide_press()));
	connect(ui.video_status,SIGNAL(sliderReleased()),this,SLOT(slide_release()));
	connect(ui.but_capture,SIGNAL(clicked()),this,SLOT(but_capture()));
	connect(ui.but_loadlbf,SIGNAL(clicked()),this,SLOT(but_loadmodel()));
	connect(ui.but_start_detect,SIGNAL(clicked()),this,SLOT(but_start_detect()));
	connect(ui.but_show_landmark,SIGNAL(clicked()),this,SLOT(but_show_landmark()));

	//connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(but_tri()));



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
	QStringList filters;
	filters<<QString("*.jpg")<<QString("*.png")<<QString("*.bmp")<<QString("*.avi");
	dir.setNameFilters(filters);
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
		double scale = 240*1.0/frame_ori.rows;

		cv::resize(frame_ori,frame_ori,Size(frame_ori.cols*scale,frame_ori.rows*scale),0,0,CV_INTER_LINEAR);
		frame_with_landMark=frame_ori.clone();

		Mat show_fixed_point=Mat::zeros(640,780,CV_8UC3);

		if (video_is_detect==1)
		{

			int res=LPredict.predict(frame_ori);
			but_tri(res);

			LPredict.getLandmark(frame_ori,current_landMark,current_landMark_fixed);
			if (is_show_landmark)
			{
				for (int i = 0; i < current_landMark.size(); i++)
				{
					//Show LANDMARK IN PIC
					Point center( current_landMark[i].x, current_landMark[i].y ); 
					ellipse( frame_with_landMark, center, Size( 1, 1), 0, 0, 0, Scalar( 255, 0, 255 ), 4, 8, 0); 

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
	double scale = 240*1.0/frame_ori.rows;

	cv::resize(frame_ori,frame_ori,Size(frame_ori.cols*scale,frame_ori.rows*scale),0,0,CV_INTER_LINEAR);
	Mat show_fixed_point=Mat::zeros(640,780,CV_8UC3);
	frame_with_landMark=frame_ori.clone();
	int getit=0;
	if (video_is_detect==1)
	{
		int res=LPredict.predict(frame_ori);
		but_tri(res);
		getit=LPredict.getLandmark(frame_ori,current_landMark,current_landMark_fixed);
		if (is_show_landmark&&getit!=-1)
		{
			Scalar active_facet_color(0, 0, 255), delaunay_color(255,255,255);
			Rect rect(0, 0, 1000, 1000);
			Subdiv2D subdiv(rect);


			for (int i = 0; i < current_landMark.size(); i++)
			{
				Point center( current_landMark[i].x, current_landMark[i].y ); 
				ellipse( frame_with_landMark, center, Size( 1, 1), 0, 0, 0, Scalar( 0, 0, 255 ), 4, 8, 0); 
				Point center_fix(current_landMark_fixed[i].x, current_landMark_fixed[i].y);
				ellipse( show_fixed_point, center_fix, Size( 1, 1), 0, 0, 0, Scalar( 0, 0, 255 ), 4, 8, 0); 
				char c[3];
				sprintf(c, "%d", i);
				string words= c;  
				putText( show_fixed_point, words, center_fix,CV_FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 0)); 

				if (i%2==0)
				{
					subdiv.insert(current_landMark[i]);
				}
			}
			draw_subdiv( frame_with_landMark, subdiv, delaunay_color );
			frame_forQTshow=frame_with_landMark.clone();
			imshow("x",show_fixed_point);



		}
		else
			frame_forQTshow=frame_ori.clone();
	}
	else
		frame_forQTshow=frame_ori.clone();

	imshow("Ori",frame_ori);
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
	ui.model_info->setText(QString::fromStdString(LPredict.getModelInfo()));
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
	if (timer->isActive())
	{
		timer->stop();
	}
	else
		timer->start();
}

void Emotion_Detect::draw_subdiv( Mat& img, Subdiv2D& subdiv, Scalar delaunay_color )
{
	bool draw;
	vector<Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	vector<Point> pt(3);

	for(size_t i = 0; i < triangleList.size(); ++i)
	{
		Vec6f t = triangleList[i];

		pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
		pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
		pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
		// MY PIECE OF CODE
		draw=true;

		for(int i=0;i<3;i++){
			if(pt[i].x>img.cols||pt[i].y>img.rows||pt[i].x<0||pt[i].y<0)
				draw=false;
		}
		if (draw){
			line(img, pt[0], pt[1], delaunay_color, 1);
			line(img, pt[1], pt[2], delaunay_color, 1);
			line(img, pt[2], pt[0], delaunay_color, 1);
		}


	}
}

void Emotion_Detect::but_tri(int res)
{
	ui.label_res_0->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0,0);"));
	ui.label_res_1->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0,0);"));
	ui.label_res_2->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0,0);"));
	ui.label_res_3->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0,0);"));
	ui.label_res_4->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0,0);"));
	ui.label_res_5->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0,0);"));
	ui.label_res_6->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0,0);"));
	ui.label_res_7->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0,0);"));

	if (res==0)
	{
		ui.label_res_0->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0);"));
		ui.label_res_4->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0);"));
	}
	if (res==1)
	{
		ui.label_res_1->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0);"));
		ui.label_res_5->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0);"));
	}
	if (res==2)
	{
		ui.label_res_2->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0);"));
		ui.label_res_6->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0);"));
	}
	if (res==3)
	{
		ui.label_res_3->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0);"));
		ui.label_res_7->setStyleSheet(QLatin1String("background-color: rgb(255, 85, 0);"));
	}



}