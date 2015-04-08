#include "LandmarkPredict.h"
#include "qdebug.h"
#include "qstring.h"
LandmarkPredict::LandmarkPredict(void)
{

}
void LandmarkPredict::loadModel()
{
	face_cascade_name = "haarcascade_frontalface_alt.xml"; 
	string file_name = "shape_1.dat";

	if (!LBF_Model_Load(file_name, model))
	{
		cout<<"Can not load the file!"<<endl;
		return;
	}
	

	SVM.load("model/svm_model_pca");
	pca.eigenvalues=loadMat("model/pca_eigenvalues");
	pca.eigenvectors=loadMat("model/pca_eigenvectors");
	pca.mean=loadMat("model/pca_mean");
	lda.load("model/lda_data");
}

LandmarkPredict::~LandmarkPredict(void)
{
}

Mat LandmarkPredict::loadMat(string mat_name)
{

	float one_float;
	ifstream mat_file(mat_name);
	vector<float> vec;
	int mat_row,mat_col;
	char spacer;
	mat_row=0;
	mat_col=0;
	int getcol=0;
	mat_file.get(spacer);
	while(spacer!=']')
	{
		mat_file>>one_float;mat_file.get(spacer);
		vec.push_back(one_float);
		if (!getcol)
		{
			mat_col++;
		}
		if (spacer==';'||spacer==']')
		{
			getcol=1;
			mat_row++;
		}
	}
	mat_file.close();
	Mat ou(mat_row,mat_col,CV_32FC1);
	for (int i = 0; i < ou.rows; i++)
	{
		for (int j = 0; j  < ou.cols; j ++)
		{
			ou.at<float>(i,j)=vec[i*ou.cols+j];
		}
	}

	return ou;
}

int LandmarkPredict::predict(Mat &input)
{
	vector<Point> face_landmark_fixed;
	if (getLandmark(input,face_landmark_fixed)==-1)
	{
		return -1;
	}
	int eresult=doPredict(face_landmark_fixed,SVM,pca,lda);

	return eresult;
}
string LandmarkPredict::doPredictFACS(vector<Point> &vec_landmark)
{
	return "x";
}

void LandmarkPredict::landMarkInfo(vector<Point>& input)
{

}

int LandmarkPredict::getLandmark(Mat frame,vector<Point> &vec_landmark)
{
	int max_point_x,min_point_x,max_point_y,min_point_y;
	double box[4];
	vector<point<double>> parts;

	if( !face_cascade.load( face_cascade_name ) ){  
		printf("级联分类器错误，可能未找到文件，拷贝该文件到工程目录下！\n");
		return -1;
	}
	std::vector<Rect> faces;
	face_cascade.detectMultiScale( frame, faces, 1.15, 5, 0);
	if (faces.size()!=1)
	{
		return -1;
	}
	for( int i = 0; i < faces.size(); i++ ){

		box[0] = faces[i].x;
		box[1] = faces[i].y;
		box[2] = (box[0] + faces[i].width);
		box[3] = (box[1] + faces[i].height);
		calculate_shape<Mat>(frame, box, model, parts);

		max_point_x=parts[0].x;
		min_point_x=parts[0].x;
		max_point_y=parts[0].y;
		min_point_y=parts[0].y;

		for (int j = 0; j < parts.size(); ++j)
		{
			if (max_point_x<parts[j].x)
				max_point_x=parts[j].x;
			if (min_point_x>parts[j].x)
				min_point_x=parts[j].x;
			if (max_point_y<parts[j].y)
				max_point_y=parts[j].y;
			if (min_point_y>parts[j].y)
				min_point_y=parts[j].y;
			//Point center( parts[j].x, parts[j].y ); 
			//ellipse( frame, center, Size( 1, 1), 0, 0, 0, Scalar( 255, 0, 255 ), 4, 8, 0); 
			/*
			char c[3];
			sprintf(c, "%d", j);
			string words= c;  
			putText( frame, words, center,CV_FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 0, 0)); */
		}

		int l_width=max_point_x-min_point_x;
		int l_height = max_point_y-min_point_y;
		double point_scale=(POINT_WIDTH*1.0/l_width)>(POINT_HEIGHT*1.0/l_height)?(POINT_WIDTH*1.0/l_width):(POINT_HEIGHT*1.0/l_height);

		Mat landMark_face(POINT_HEIGHT,POINT_WIDTH+300,CV_8UC3,Scalar(0,0,0));
		Mat landMark_face_fixed(POINT_HEIGHT,POINT_WIDTH+300,CV_8UC3,Scalar(0,0,0));

		int  turnDir= parts[36].y>parts[45].y?0:1;
		double abs_x_min = abs(parts[45].x-parts[36].x);
		double abs_y_min = abs(parts[45].y-parts[36].y);
		double abs_h = sqrt(abs_x_min*abs_x_min+abs_y_min*abs_y_min);
		double cosbeta;
		double beta;
		cosbeta = abs_x_min/abs_h;
		beta=acos(cosbeta);   //Roate angle

		//x1=cos(angle)*x-sin(angle)*y;
		//y1=cos(angle)*y+sin(angle)*x;

		double nx,ny;
		double cx,cy;
		cx = (parts[27].x-min_point_x)*point_scale;
		cy = (parts[27].y-min_point_y)*point_scale;

		if (turnDir==0) //image needs to roate clockwise
		{
			nx = cos(beta)*cx-sin(beta)*cy;
			ny = cos(beta)*cy+sin(beta)*cx;
		}
		else //image needs to roate anticlockwise
		{
			nx = cos(-beta)*cx-sin(-beta)*cy;
			ny = cos(-beta)*cy+sin(-beta)*cx;
		}
		double gx = cx-nx;
		double gy = cy-ny;

		for (int j = 0; j < parts.size(); j++)
		{
			Point center( (parts[j].x-min_point_x)*point_scale, (parts[j].y-min_point_y)*point_scale); 

			if (turnDir==0) //image needs to roate clockwise
			{
				nx = cos(beta)*center.x-sin(beta)*center.y;
				ny = cos(beta)*center.y+sin(beta)*center.x;
			}
			else //image needs to roate anticlockwise
			{
				nx = cos(-beta)*center.x-sin(-beta)*center.y;
				ny = cos(-beta)*center.y+sin(-beta)*center.x;
			}

			center.x = nx+gx;
			center.y = ny+gy;

			vec_landmark.push_back(center);
			/*
			ellipse( landMark_face_fixed, center, Size( 1, 1), 0, 0, 0, Scalar( 255, 255, 255 ), 4, 8, 0 ); 
			char c[3];
			sprintf(c, "%d", j);
			string words= c;  
			putText( landMark_face_fixed, words, center, CV_FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0));

			imshow("neutral", landMark_face_fixed);
			*/
		}
	}
	return 1;
}

int LandmarkPredict::doPredict(vector<Point> &vec_landmark,CvSVM &SVM,PCA &pca,LDA &lda)
{
	vector<Point> point_eyebrow;
	vector<Point> point_leye;
	vector<Point> point_reye;
	vector<Point> point_mouth;
	vector<Point> point_nose;

	map<int,string> number_2_emotion;

	map <string, int>::iterator m1_Iter;
	int current_emotion_code;

	point_eyebrow.clear();
	point_leye.clear();
	point_reye.clear();
	point_mouth.clear();
	point_nose.clear();

	//get all point 
	//eyebrow
	for (int j = 17; j < 27; j++)
	{
		point_eyebrow.push_back(vec_landmark[j]);
	}

	//Left eye
	for (int j = 36; j < 42; j++)
	{
		point_leye.push_back(vec_landmark[j]);
	}

	//right eye
	for (int j = 42; j < 48; j++)
	{
		point_reye.push_back(vec_landmark[j]);
	}

	// mouth
	for (int j = 48; j < vec_landmark.size(); j++)
	{
		point_mouth.push_back(vec_landmark[j]);
	}

	//nose
	for (int j = 31; j < 35; j++)
	{
		point_nose.push_back(vec_landmark[j]);
	}

	//vector<> for point feature
	//	
	vector<double> vec_mouth,vec_leye,vec_reye,vec_eyebrow;
	for (int j = 0; j < point_mouth.size()-1; j++)
	{
		for (int l = j+1; l < point_mouth.size(); l++)
		{
			double distance_x,distance_y,distance_h;
			distance_x=point_mouth[j].x - point_mouth[l].x;
			distance_y=point_mouth[j].y - point_mouth[l].y;
			distance_h=sqrt(distance_x*distance_x+distance_y*distance_y);
			vec_mouth.push_back(distance_x/distance_h);
			vec_mouth.push_back(distance_y/distance_h);
		}
	}

	for (int j = 0; j < point_leye.size()-1; j++)
	{
		for (int l = j+1; l < point_leye.size(); l++)
		{
			double distance_x,distance_y,distance_h;
			distance_x=point_leye[j].x - point_leye[l].x;
			distance_y=point_leye[j].y - point_leye[l].y;
			distance_h=sqrt(distance_x*distance_x+distance_y*distance_y);
			//vec_leye.push_back(distance_x/distance_h);
			//vec_leye.push_back(distance_y/distance_h);
			vec_leye.push_back(distance_x);
			vec_leye.push_back(distance_y);
		}
	}

	for (int j = 0; j < point_reye.size()-1; j++)
	{
		for (int l = j+1; l < point_reye.size(); l++)
		{
			double distance_x,distance_y,distance_h;
			distance_x=point_reye[j].x - point_reye[l].x;
			distance_y=point_reye[j].y - point_reye[l].y;
			distance_h=sqrt(distance_x*distance_x+distance_y*distance_y);
			//vec_reye.push_back(distance_x/distance_h);
			//vec_reye.push_back(distance_y/distance_h);
			vec_reye.push_back(distance_x);
			vec_reye.push_back(distance_y);
		}
	}

	for (int j = 0; j < point_eyebrow.size()-1; j++)
	{
		for (int l = j+1; l < point_eyebrow.size(); l++)
		{
			double distance_x,distance_y,distance_h;
			distance_x=point_eyebrow[j].x - point_eyebrow[l].x;
			distance_y=point_eyebrow[j].y - point_eyebrow[l].y;
			distance_h=sqrt(distance_x*distance_x+distance_y*distance_y);
			//vec_eyebrow.push_back(distance_x/distance_h);
			//vec_eyebrow.push_back(distance_y/distance_h);
			vec_eyebrow.push_back(distance_x);
			vec_eyebrow.push_back(distance_y);
		}
	}


	// all vec
	vector<double> vec_all;
	for (int j = 17; j < vec_landmark.size()-1; j++)
	{
		for (int l = j+1; l < vec_landmark.size(); l++)
		{
			double distance_x,distance_y,distance_h;
			distance_x=vec_landmark[j].x - vec_landmark[l].x;
			distance_y=vec_landmark[j].y - vec_landmark[l].y;
			distance_h=sqrt(distance_x*distance_x+distance_y*distance_y);
			vec_all.push_back(distance_x);
			vec_all.push_back(distance_y);
		}
	}

	Mat current_test_mat(1,vec_all.size(),CV_32FC1);
	
	for (int j = 0; j < vec_all.size(); j++)
	{
		current_test_mat.at<float>(0,j)=vec_all[j];
	}

	Mat dst = pca.project(current_test_mat);
	return SVM.predict(dst);

}
