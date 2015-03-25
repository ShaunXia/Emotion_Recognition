#include "emotion_train_tool.h"


Emotion_Train_Tool::Emotion_Train_Tool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	timer = new QTimer(this);
	timer->setInterval(60);
	ui.video_status->setDisabled(true);
	ui.group_emotion->setDisabled(true);
	ui.group_facs->setDisabled(true);
	isdetect=0;

	connect(timer,SIGNAL(timeout()),this,SLOT(startLoopSlot()));
	connect(ui.choose_path, SIGNAL(clicked()), this, SLOT(getFileList()));
	connect(ui.fileList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemclick(QListWidgetItem*)));
	connect(ui.video_status,SIGNAL(sliderPressed()),this,SLOT(slide_press()));
	connect(ui.video_status,SIGNAL(sliderReleased()),this,SLOT(slide_release()));
	connect(ui.but_capture,SIGNAL(clicked()),this,SLOT(but_capture()));
	connect(ui.but_happy,SIGNAL(clicked()),this,SLOT(but_happy()));
	connect(ui.but_normal,SIGNAL(clicked()),this,SLOT(but_normal()));
	connect(ui.but_sleepy,SIGNAL(clicked()),this,SLOT(but_sleepy()));

	connect(ui.but_svm_or_facs,SIGNAL(clicked()),this,SLOT(but_svm_or_facs()));
	connect(ui.but_load_model,SIGNAL(clicked()),this,SLOT(but_load_model()));
	connect(ui.but_detect_trigger,SIGNAL(clicked()),this,SLOT(but_detect_trigger()));
	connect(ui.but_facs_info,SIGNAL(clicked()),this,SLOT(but_facs_info()));

}

Emotion_Train_Tool::~Emotion_Train_Tool()
{
	timer->stop();
	cap.release();
}

void Emotion_Train_Tool::getFileList()
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

void Emotion_Train_Tool::itemclick (QListWidgetItem *item)
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
		frame_tolandmark=imread(tppath.toStdString());
		double scale = 456*1.0/frame_tolandmark.rows;
		cv::resize(frame_tolandmark,frame_tolandmark,Size(frame_tolandmark.cols*scale,frame_tolandmark.rows*scale),0,0,CV_INTER_LINEAR);
		vector<Point> tp;
		frame_towrite=frame_tolandmark.clone();
		if (isdetect)
		{
			landmark.getLandmark(frame_tolandmark,tp);
		}
		cvtColor(frame_tolandmark, frame_tolandmark, CV_BGR2RGB); 
		QImage *qJpg = new QImage((unsigned char*)frame_tolandmark.data, // uchar* data  
			frame_tolandmark.cols, frame_tolandmark.rows, // width height
			frame_tolandmark.step, //bytesPerLine  
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

void Emotion_Train_Tool::startLoopSlot()
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
	cap >> frame_tolandmark; // get a new frame from camera
	frame_towrite=frame_tolandmark.clone();
	vector<Point> tp;
	if (isdetect)
	{
		landmark.getLandmark(frame_tolandmark,tp);
	}
	cvtColor(frame_tolandmark, frame, CV_BGR2RGB); 
	qCam = new QImage((unsigned char*)frame.data, // uchar* data  
		frame.cols, frame.rows, // width height  
		frame.step, //bytesPerLine  
		QImage::Format_RGB888); //format  
	ui.picshow->setPixmap(QPixmap::fromImage(*qCam));
	ui.picshow->resize(qCam->size());
}

void Emotion_Train_Tool::slide_press()
{
	timer->stop();
}
void Emotion_Train_Tool::slide_release()
{
	cap.set(CV_CAP_PROP_POS_FRAMES,ui.video_status->value());
	current_frame=ui.video_status->value();
	timer->start();
}

void Emotion_Train_Tool::but_capture()
{
	timer->stop();
	ui.but_happy->setDisabled(false);
	ui.but_normal->setDisabled(false);
	ui.but_sleepy->setDisabled(false);
}

void Emotion_Train_Tool::but_happy()
{
	QDateTime time = QDateTime::currentDateTime();
	QString str("happy#");
	str.append(time.toString("MMdd_hh_mm_ss"));
	str.append(".jpg");
	imwrite(str.toStdString(),frame_towrite);
	ui.but_happy->setDisabled(true);
	ui.but_normal->setDisabled(true);
	ui.but_sleepy->setDisabled(true);
	timer->start();
}


void Emotion_Train_Tool::but_normal()
{
	QDateTime time = QDateTime::currentDateTime();
	QString str("normal#");
	str.append(time.toString("MMdd_hh_mm_ss"));
	str.append(".jpg");
	imwrite(str.toStdString(),frame_towrite);
	ui.but_happy->setDisabled(true);
	ui.but_normal->setDisabled(true);
	ui.but_sleepy->setDisabled(true);
	timer->start();
}


void Emotion_Train_Tool::but_sleepy()
{
	QDateTime time = QDateTime::currentDateTime();
	QString str("sleepy#");
	str.append(time.toString("MMdd_hh_mm_ss"));
	str.append(".jpg");
	imwrite(str.toStdString(),frame_towrite);
	ui.but_happy->setDisabled(true);
	ui.but_normal->setDisabled(true);
	ui.but_sleepy->setDisabled(true);
	timer->start();
}

void Emotion_Train_Tool::but_svm_or_facs()
{
	if (ui.group_emotion->isEnabled())
	{
		ui.group_emotion->setDisabled(true);
		ui.group_facs->setDisabled(false);
		ui.fileList->setDisabled(true);
	}
	else
	{
		ui.group_emotion->setDisabled(false);
		ui.group_facs->setDisabled(true);
		ui.but_happy->setDisabled(true);
		ui.but_normal->setDisabled(true);
		ui.but_sleepy->setDisabled(true);
		ui.fileList->setDisabled(false);
	}
}

void Emotion_Train_Tool::but_load_model()
{
	landmark.loadModel();
	ui.but_load_model->setText("Loaded!");
}
void Emotion_Train_Tool::but_detect_trigger()
{
	isdetect=!isdetect;
	QString str("Detect:");
	str.append(QString::number(isdetect));
	ui.but_detect_trigger->setText(str);
}
void Emotion_Train_Tool::but_facs_info()
{
	for (int i = 1; i < ui.fileList->count(); i++)
	{
		QString tppath = filePath;
		tppath.append("/").append(ui.fileList->item(i)->text());
		
		frame_tolandmark=imread(tppath.toStdString());
		double scale = 456*1.0/frame_tolandmark.rows;
		cv::resize(frame_tolandmark,frame_tolandmark,Size(frame_tolandmark.cols*scale,frame_tolandmark.rows*scale),0,0,CV_INTER_LINEAR);
		vector<Point> tp;
		frame_towrite=frame_tolandmark.clone();
		
		
		cvtColor(frame_tolandmark, frame_tolandmark, CV_BGR2RGB); 
		QImage *qJpg = new QImage((unsigned char*)frame_tolandmark.data, // uchar* data  
			frame_tolandmark.cols, frame_tolandmark.rows, // width height
			frame_tolandmark.step, //bytesPerLine  
			QImage::Format_RGB888); //format  
		ui.picshow->setPixmap(QPixmap::fromImage(*qJpg));
		ui.picshow->resize(qJpg->size());

		landmark.getLandmark(frame_tolandmark,tp);
		if (tp.size()!=0)
		{
			double dis = sqrt((tp[33].x-tp[30].x)*(tp[33].x-tp[30].x)+(tp[33].y-tp[30].y)*(tp[33].y-tp[30].y));

		ui.log_facs_info->insertPlainText(QString::number(dis));
		//ui.log_facs_info->insertPlainText(dis);
		ui.log_facs_info->insertPlainText("\n");
		}
		else
			ui.log_facs_info->insertPlainText("no face \n");
		
		waitKey(0);


	}
}
