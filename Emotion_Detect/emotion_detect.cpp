#include "emotion_detect.h"

Emotion_Detect::Emotion_Detect(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	timer = new QTimer(this);
	timer->setInterval(60);
	ui.video_status->setDisabled(true);
	ui.predict_control->setDisabled(true);
	video_is_detect=0;
	is_show_landmark=0;

	connect(timer,SIGNAL(timeout()),this,SLOT(startLoopSlot()));
	connect(ui.choose_path, SIGNAL(clicked()), this, SLOT(getFileList()));
	connect(ui.fileList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemclick(QListWidgetItem*)));
	connect(ui.video_status,SIGNAL(sliderPressed()),this,SLOT(slide_press()));
	connect(ui.video_status,SIGNAL(sliderReleased()),this,SLOT(slide_release()));
	connect(ui.but_capture,SIGNAL(clicked()),this,SLOT(but_capture()));
	connect(ui.but_loadlbf,SIGNAL(clicked()),this,SLOT(but_loadmodel()));
	connect(ui.but_start_detect,SIGNAL(clicked()),this,SLOT(but_start_detect()));
	connect(ui.but_show_landmark,SIGNAL(clicked()),this,SLOT(but_show_landmark()));



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


		//QMessageBox::information(this,"helo",tppath);
		frame_towrite=imread(tppath.toStdString());
		double scale = 456*1.0/frame_towrite.rows;

		cv::resize(frame_towrite,frame_towrite,Size(frame_towrite.cols*scale,frame_towrite.rows*scale),0,0,CV_INTER_LINEAR);
		frame_with_landMark=frame_towrite.clone();

		if (video_is_detect==1)
		{
			ui.label->setText(QString::fromStdString(LPredict.predict(frame_with_landMark)));
		}
		if (is_show_landmark)
		{
			frame_towrite=frame_with_landMark;
		}
		cvtColor(frame_towrite, frame_towrite, CV_BGR2RGB); 
		QImage *qJpg = new QImage((unsigned char*)frame_towrite.data, // uchar* data  
			frame_towrite.cols, frame_towrite.rows, // width height
			frame_towrite.step, //bytesPerLine  
			QImage::Format_RGB888); //format  
		ui.picshow->setPixmap(QPixmap::fromImage(*qJpg));
		ui.picshow->resize(qJpg->size());
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
			cap.set(CV_CAP_PROP_FRAME_WIDTH,480);
			cap.set(CV_CAP_PROP_FRAME_HEIGHT,320);
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
	if (captype==1)
	{
		if (current_frame==ui.video_status->maximum())
		{
			current_frame=0;
			cap.set(CV_CAP_PROP_POS_FRAMES,0);
		}
		ui.video_status->setValue(current_frame++);
	}

	cap >> frame_towrite; // get a new frame from camera
	frame_with_landMark=frame_towrite.clone();
		if (video_is_detect==1)
	{
		ui.label->setText(QString::fromStdString(LPredict.predict(frame_with_landMark)));
	}
			if (is_show_landmark)
		{
			frame_towrite=frame_with_landMark;
		}

	cvtColor(frame_towrite, frame_towrite, CV_BGR2RGB); 
	qCam = new QImage((unsigned char*)frame_towrite.data, // uchar* data  
		frame_towrite.cols, frame_towrite.rows, // width height  
		frame_towrite.step, //bytesPerLine  
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
	ui.predict_control->setDisabled(false);
	timer->stop();
}