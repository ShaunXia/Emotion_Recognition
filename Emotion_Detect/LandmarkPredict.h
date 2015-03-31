#pragma once
#include "LBFModel.h"


#define POINT_WIDTH 480
#define POINT_HEIGHT 640
class LandmarkPredict
{
public:
	LandmarkPredict(void);
	~LandmarkPredict(void);
	LBF_model model;
	string face_cascade_name;
	int doPredict(vector<Point>&,CvSVM &SVM);
	int getLandmark(Mat &,vector<Point> &);
	string doPredictFACS(vector<Point>&);
	int predict(Mat&);
	void landMarkInfo(vector<Point>&);
	void loadModel();
};

