#pragma once
#include "LBFModel.h"

#define POINT_WIDTH 480
#define POINT_HEIGHT 640

class ImgLandMark
{
public:
	ImgLandMark(void);
	~ImgLandMark(void);

	LBF_model model;
	string face_cascade_name;
	int getLandmark(Mat &,vector<Point> &);
	string FACS_info(Mat&);
	void landMarkInfo(vector<Point>&);
	void loadModel();
};

