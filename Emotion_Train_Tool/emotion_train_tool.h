#ifndef EMOTION_TRAIN_TOOL_H
#define EMOTION_TRAIN_TOOL_H

#include <QtWidgets/QMainWindow>
#include "ui_emotion_train_tool.h"
#include <QDebug>
#include <QDir>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include "opencv/cv.h"
#include "opencv/cvaux.h"
#include "opencv/highgui.h"
#include <string>
#include <cmath>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <ctime>
#include <iostream>
#include <fstream>
#include <qtimer.h>
#include <qdatetimeedit.h>
#include "ImgLandMark.h"

using namespace cv;

class Emotion_Train_Tool : public QMainWindow
{
	Q_OBJECT

public:
	Emotion_Train_Tool(QWidget *parent = 0);
	~Emotion_Train_Tool();

	private slots: 
	void getFileList();
	void itemclick (QListWidgetItem*);
	void startLoopSlot();
	void slide_press();
	void slide_release();
	void but_capture();
	void but_happy();
	void but_normal();
	void but_sleepy();

	void but_svm_or_facs();
	void but_load_model();
	void but_detect_trigger();
	void but_facs_info();

	double vec_angle(Point vc1,Point vc2); 
	double point_distance(Point p1,Point p2);
	int face_vector_result(vector<Point> point);

private:
	Ui::Emotion_Train_ToolClass ui;
	QString filePath;
	QImage *qCam;
	QTimer *timer;
	Mat frame_towrite;
	Mat frame_tolandmark;
	VideoCapture cap;
	int captype;
	int current_frame;

	int isdetect;
	ImgLandMark landmark;
	int is_model_loaded;

};

#endif // EMOTION_TRAIN_TOOL_H
