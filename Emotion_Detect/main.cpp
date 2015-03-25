#include "emotion_detect.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Emotion_Detect w;
	w.show();
	return a.exec();
}
