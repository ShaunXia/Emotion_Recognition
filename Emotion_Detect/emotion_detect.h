#ifndef EMOTION_DETECT_H
#define EMOTION_DETECT_H

#include <QtWidgets/QMainWindow>
#include "ui_emotion_detect.h"
#include <QDebug>
#include <QDir>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qpropertyanimation.h>
#include <string>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <qtimer.h>
#include <qdatetimeedit.h>

#include "LandmarkPredict.h"


class Emotion_Detect : public QMainWindow
{
	Q_OBJECT

public:
	Emotion_Detect(QWidget *parent = 0);
	~Emotion_Detect();
	void but_tri(int res);

	private slots: 
	void getFileList();
	void itemclick (QListWidgetItem*);
	void startLoopSlot();
	void slide_press();
	void slide_release();
	void but_capture();
	void but_loadmodel();
	void but_start_detect();
	void but_show_landmark();
	 

private:
	Ui::Emotion_DetectClass ui;
	QString filePath;
	QImage *qCam;
	QTimer *timer;

	Mat frame_ori;
	Mat frame_with_landMark;
	//Mat frame_with_nolandMark;
	Mat frame_forQTshow;
	Mat neutral_face;

	VideoCapture cap;
	int captype;
	int current_frame;
	LandmarkPredict LPredict;
	int video_is_detect;
	int is_show_landmark;


	QVector<QLabel*> vec_qlabel;
	vector<Point> neutral_landMark;
	vector<Point> neutral_landMark_fixed;
	vector<Point> current_landMark;
	vector<Point> current_landMark_fixed;

	int get_neutral;
	void draw_subdiv( Mat& img, Subdiv2D& subdiv, Scalar delaunay_color );

};

#endif // EMOTION_DETECT_H
