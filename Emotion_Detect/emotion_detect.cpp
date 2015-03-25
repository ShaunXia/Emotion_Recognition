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

	connect(timer,SIGNAL(timeout()),this,SLOT(startLoopSlot()));
	connect(ui.choose_path, SIGNAL(clicked()), this, SLOT(getFileList()));
	connect(ui.fileList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemclick(QListWidgetItem*)));
	connect(ui.video_status,SIGNAL(sliderPressed()),this,SLOT(slide_press()));
	connect(ui.video_status,SIGNAL(sliderReleased()),this,SLOT(slide_release()));
	connect(ui.but_capture,SIGNAL(clicked()),this,SLOT(but_capture()));
	connect(ui.but_loadlbf,SIGNAL(clicked()),this,SLOT(but_loadmodel()));
	connect(ui.but_start_detect,SIGNAL(clicked()),this,SLOT(but_start_detect()));
	connect(ui.but_stop_detect,SIGNAL(clicked()),this,SLOT(but_stop_detect()));
	
	

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
	video_is_detect=0;
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
		Mat image2;
		cv::resize(frame_towrite,image2,Size(frame_towrite.cols*scale,frame_towrite.rows*scale),0,0,CV_INTER_LINEAR);

		cvtColor(image2, image2, CV_BGR2RGB); 
		QImage *qJpg = new QImage((unsigned char*)image2.data, // uchar* data  
			image2.cols, image2.rows, // width height
			image2.step, //bytesPerLine  
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

	Mat frame;
	cap >> frame_towrite; // get a new frame from camera
	cvtColor(frame_towrite, frame, CV_BGR2RGB); 
	qCam = new QImage((unsigned char*)frame.data, // uchar* data  
		frame.cols, frame.rows, // width height  
		frame.step, //bytesPerLine  
		QImage::Format_RGB888); //format  
	ui.picshow->setPixmap(QPixmap::fromImage(*qCam));
	ui.picshow->resize(qCam->size());
	if (video_is_detect==1)
	{
		ui.label->setText(QString::fromStdString(LPredict.predict(frame_towrite)));
	}
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
}

void Emotion_Detect::but_start_detect()
{
	if (captype==-1)
	{
		ui.label->setText(QString::fromStdString(LPredict.predict(frame_towrite)));
	}
	else
		video_is_detect=1;
	
}


void Emotion_Detect::but_stop_detect()
{
	video_is_detect=0;
}


void Emotion_Detect::but_capture()
{
	ui.predict_control->setDisabled(false);
}