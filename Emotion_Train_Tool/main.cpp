#include "emotion_train_tool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Emotion_Train_Tool w;
	w.show();
	return a.exec();
}
