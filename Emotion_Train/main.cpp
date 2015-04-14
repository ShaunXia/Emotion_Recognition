#include "opencv/cv.h"
#include "opencv/cvaux.h"
#include "opencv/highgui.h"
#include <string>
#include <cmath>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"
#include <ctime>
#include <iostream>
#include <fstream>
#include <io.h>
#include "LBFModel.h"
using namespace std;
using namespace cv;



#define OPEN_TEMPLATE_FILE_FAIL 0
#define DATA_NUM 622

#define POINT_WIDTH 650
#define POINT_HEIGHT 640

#define WITHOUT_PCA 0
#define WITH_PCA 1
#define WITH_PCA_LDA 2

#define DR_TYPE 1


LBF_model model;
string face_cascade_name = "haarcascade_frontalface_alt.xml"; 
ofstream predict_log;
void findPointsBorder(vector<Point> &points,Point &ul,Point &dr)
{
	double max_point_x,min_point_x,max_point_y,min_point_y;
	max_point_x=points[0].x;
	min_point_x=points[0].x;
	max_point_y=points[0].y;
	min_point_y=points[0].y;
	for (int i = 0; i < points.size(); i++)
	{
		if (max_point_x<points[i].x)
			max_point_x=points[i].x;
		if (min_point_x>points[i].x)
			min_point_x=points[i].x;
		if (max_point_y<points[i].y)
			max_point_y=points[i].y;
		if (min_point_y>points[i].y)
			min_point_y=points[i].y;
	}
	ul.x=min_point_x;
	ul.y=min_point_y;
	dr.x=max_point_x;
	dr.y=max_point_y;
}

int getLandmark(Mat &frame,vector<Point> &vec_landmark)
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
			Point center( parts[j].x, parts[j].y ); 
			ellipse( frame, center, Size( 1, 1), 0, 0, 0, Scalar( 255, 0, 255 ), 4, 8, 0); 
			/*
			char c[3];
			sprintf(c, "%d", j);
			string words= c;  
			putText( frame, words, center,CV_FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 0, 0)); 
			*/
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
			ellipse( landMark_face_fixed, center, Size( 1, 1), 0, 0, 0, Scalar( 255, 255, 255 ), 4, 8, 0 ); 
			char c[3];
			sprintf(c, "%d", j);
			string words= c;  
			putText( landMark_face_fixed, words, center, CV_FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0));

			imshow("landMark_face_fixed", landMark_face_fixed);

		}
	}
	return 1;
}

int getFeature(Mat &frame,vector<double> &vec_feature)
{
	vector<Point> fixed_point;
	vector<Point> point_eyebrow;
	vector<Point> point_leye;
	vector<Point> point_reye;
	vector<Point> point_mouth;
	vector<Point> point_nose;

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
		return 0;
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

		for (int j = 0; j < parts.size(); ++j)  //face 
		{
			if (max_point_x<parts[j].x)
				max_point_x=parts[j].x;
			if (min_point_x>parts[j].x)
				min_point_x=parts[j].x;
			if (max_point_y<parts[j].y)
				max_point_y=parts[j].y;
			if (min_point_y>parts[j].y)
				min_point_y=parts[j].y;
			Point center( parts[j].x, parts[j].y ); 
			ellipse( frame, center, Size( 1, 1), 0, 0, 0, Scalar( 255, 0, 255 ), 4, 8, 0); 
			/*
			char c[3];
			sprintf(c, "%d", j);
			string words= c;  
			putText( frame, words, center,CV_FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 0, 0)); 
			*/
		} // get 

		int l_width=max_point_x-min_point_x;
		int l_height = max_point_y-min_point_y;
		double point_scale=POINT_WIDTH*1.0/l_width;

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

			fixed_point.push_back(center);
			ellipse( landMark_face_fixed, center, Size( 1, 1), 0, 0, 0, Scalar( 255, 255, 255 ), 4, 8, 0 ); 
			char c[3];
			sprintf(c, "%d", j);
			string words= c;  
			putText( landMark_face_fixed, words, center, CV_FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0));

			
			
		}

		//imshow("landMark_face_fixed", landMark_face_fixed);
		//waitKey(0);

		point_eyebrow.clear();
		point_leye.clear();
		point_reye.clear();
		point_mouth.clear();
		point_nose.clear();

		//get all point 
		//eyebrow
		for (int j = 17; j < 27; j++)
		{
			point_eyebrow.push_back(fixed_point[j]);
		}

		//Left eye
		for (int j = 36; j < 42; j++)
		{
			point_leye.push_back(fixed_point[j]);
		}

		//right eye
		for (int j = 42; j < 48; j++)
		{
			point_reye.push_back(fixed_point[j]);
		}

		// mouth
		for (int j = 48; j < parts.size(); j++)
		{
			point_mouth.push_back(fixed_point[j]);
		}

		//nose
		for (int j = 31; j < 35; j++)
		{
			point_nose.push_back(fixed_point[j]);
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
				vec_mouth.push_back(distance_x);
				vec_mouth.push_back(distance_y);
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

		//double dis_36_39=sqrt((fixed_point[36].x - fixed_point[39].x)*(fixed_point[36].x - fixed_point[39].x)+(fixed_point[36].y - fixed_point[39].y)*(fixed_point[36].y - fixed_point[39].y));
		double ES0 = fixed_point[44].x-fixed_point[38].x;
		double IRISD0=fixed_point[46].y-fixed_point[44].y;
		double ENS0 = fixed_point[33].y-fixed_point[27].y;
		double MNS0 = fixed_point[51].y-fixed_point[33].y;
		double MW0 = fixed_point[54].x - fixed_point[48].x;


		for (int j = 17; j < fixed_point.size()-1; j++)
		{
			for (int l = j+1; l < fixed_point.size(); l++)
			{
				double distance_x,distance_y,distance_h;
				distance_x=fixed_point[j].x - fixed_point[l].x;
				distance_y=fixed_point[j].y - fixed_point[l].y;
				distance_h=sqrt(distance_x*distance_x+distance_y*distance_y);
				vec_feature.push_back(distance_h*1.0/ES0);
			//	vec_feature.push_back(distance_y*1.0/ES0);
			//	vec_feature.push_back(distance_h*1.0/IRISD0);
			//	vec_feature.push_back(distance_h*1.0/ENS0);
			//	vec_feature.push_back(distance_h*1.0/MNS0);
			//	vec_feature.push_back(distance_h*1.0/MW0);
			}
		}

	}
	return 1;

}

double getPointDistance(Point p1,Point p2)
{
	return sqrt((p1.x - p2.x)*(p1.x - p2.x)+(p1.y - p2.y)*(p1.y - p2.y));
}

int doTrain(vector<vector<double>> &vec_traindata,vector<int> &vec_trainlabel,vector<string> img_list)
{
	int max_point_x,min_point_x,max_point_y,min_point_y;
	int find_feature_sample_num=0;
	int current_emotion_code=0;

	if( !face_cascade.load( face_cascade_name ) ){  
		printf("级联分类器错误，可能未找到文件，拷贝该文件到工程目录下！\n");
		return -1;
	}

	map <string, int>::iterator m1_Iter;
	ofstream out("train_log.txt");
	vector<string> notdetect_file;
	map<string,int> emotion_2_number;

	for (int i = 0; i < img_list.size(); i++)
	{
		string ntp;
		int loc;
		loc=img_list[i].find("#");
		int rloc=img_list[i].rfind("\\");
		if (loc!=string::npos)
		{
			//labels.at<float>(i,0)=0;
			ntp=img_list[i].substr(rloc+1,loc-rloc-1);
			if (emotion_2_number.find(ntp)==emotion_2_number.end())
			{
				emotion_2_number.insert(pair<string,int>(ntp,current_emotion_code++));
			}

		}
	}

	ofstream map_file("ecode_map.txt");
	cout<<"Train string-code Map"<<endl;
	int count=0;
	for (m1_Iter = emotion_2_number.begin(); m1_Iter!=emotion_2_number.end(); m1_Iter++)
	{
		count++;
		cout<<"Emotion: "<<m1_Iter->first<<" | "<<" Code "<<m1_Iter->second<<endl;
		out<<"Emotion: "<<m1_Iter->first<<" | "<<" Code "<<m1_Iter->second<<endl;
		map_file<<m1_Iter->first<<" "<<m1_Iter->second;
		if (count!=emotion_2_number.size())
		{
			map_file<<endl;
		}
	}
	map_file.close();


	for (int k = 0; k < img_list.size(); k++)
	{
		cout<<k+1<<" / " <<img_list.size()<<"  --->  ";

		vector<double> one_train_data;
		Mat tpframe=imread(img_list[k]);
		int detected=getFeature(tpframe,one_train_data);

		if (detected==1)
		{
			find_feature_sample_num++;
			vec_traindata.push_back(one_train_data);
			map <string, int>::iterator emIt;
			int loc=img_list[k].find("#");
			int rloc = img_list[k].rfind("\\");
			string ntp;
			int emotion_num=0;
			if (loc!=string::npos)
			{
				ntp=img_list[k].substr(rloc+1,loc-rloc-1);
				emIt = emotion_2_number.find(ntp);
			}
			vec_trainlabel.push_back(emIt->second);
			cout<<" -- Fine"<<endl;
		}
		else
		{
			notdetect_file.push_back(img_list[k]);
			cout<<" ## Detect None "<<endl;
		}
		cout<<"\nFind: "<<find_feature_sample_num<<endl;
		cout<<"Label size: "<<vec_trainlabel.size()<<endl;


	}
	out<<find_feature_sample_num<<"/"<<img_list.size()<<endl;
	out<<"No Face Img:"<<endl;
	for (int i = 0; i < notdetect_file.size(); i++)
	{
		out<<i<<" "<<notdetect_file[i]<<endl;
	}
	out.close();

}

int doPredict(vector<string> &img_list,CvSVM &SVM,map<string,int> &emotion_2_number,PCA &pca,LDA &lda)
{

	int find_feature_sample_num=0;
	int correct_num=0;
	int predict_result_correct_num[10];
	int predict_result_all_num[10];
	for (int i = 0; i < 10; i++)
	{
		predict_result_correct_num[i]=0;
		predict_result_all_num[i]=0;
	}
	
	map <string, int>::iterator m1_Iter;
	int current_emotion_code;
	int max_point_x,min_point_x,max_point_y,min_point_y;


	for (int k = 0; k < img_list.size(); k++)
	{
		cout<<k+1<<" / " <<img_list.size()<<"  --->  ";
		predict_log<<k+1<<" / " <<img_list.size()<<"  --->  ";
		predict_log<<img_list[k]<<" ";
		int loc=img_list[k].find("#");
		int rloc = img_list[k].rfind("\\");
		string ntp;
		if (loc!=string::npos)
		{
			ntp=img_list[k].substr(rloc+1,loc-rloc-1);
			m1_Iter = emotion_2_number.find(ntp);
		}
		current_emotion_code=m1_Iter->second;

		Mat frame=imread(img_list[k]);
		vector<double> vec_all;
		int detected=getFeature(frame,vec_all);
		int totalDimension=vec_all.size();
		int currentDim=0;

		if (detected==1)
		{
			find_feature_sample_num++;
			Mat current_test_mat(1,totalDimension,CV_32FC1);
			for (int j = 0; j < totalDimension; j++)
			{
				current_test_mat.at<float>(0,currentDim)=vec_all[j];
				currentDim++;
			}

			Mat dst;Mat ldadst;Mat lda32;Mat final_test;
			if (DR_TYPE==WITH_PCA||DR_TYPE==WITH_PCA_LDA)
			{

				dst = pca.project(current_test_mat);

				if (DR_TYPE==WITH_PCA_LDA)
				{
					ldadst = lda.project(dst);

					ldadst.convertTo(lda32,CV_32FC1);
					final_test=lda32.clone();
				}
				else
				{
					final_test=dst.clone();
				}

			}
			else
			{
				final_test=current_test_mat;
			}


			int svm_result = SVM.predict(final_test);

			cout<<" Label : "<<current_emotion_code<<"  Predict "<<svm_result;
			predict_log<<" Label : "<<current_emotion_code<<"  Predict "<<svm_result;
			if (current_emotion_code==svm_result)
			{
				correct_num++;
				predict_result_correct_num[current_emotion_code]++;
				predict_result_all_num[current_emotion_code]++;
				cout<<" -- Correct!"<<endl;
				predict_log<<" -- Correct!"<<endl;
			}
			else
			{
				predict_result_all_num[current_emotion_code]++;
				cout<<" -- Incorrect!"<<endl;
				predict_log<<" -- Incorrect!"<<endl;
			}
		}
		else
		{
			cout<<"  ## Detect None "<<endl;
			predict_log<<"  ## Detect None "<<endl;
		}
	}

	predict_log<<"Correct / All :"<< correct_num<<" / "<<find_feature_sample_num<<" | "<< correct_num*1.0/find_feature_sample_num<<endl;
	cout<<"Correct / All :"<< correct_num<<" / "<<find_feature_sample_num<<" | "<< correct_num*1.0/find_feature_sample_num<<endl;

	for (int i = 0; i < emotion_2_number.size(); i++)
	{
		cout<<"Code "<<i<<" :"<<predict_result_correct_num[i]<<" / "<<predict_result_all_num[i]<<endl;
		predict_log<<"Code "<<i<<" :"<<predict_result_correct_num[i]<<" / "<<predict_result_all_num[i]<<endl;
	}

	predict_log<<"C :"<<SVM.get_params().C<<endl;
	return 1;
}

void getImgList(string path,vector<string> &imglist)
{
	// get image list
	string img_path=path;
	string exten = "*";
	string fileFolder = img_path+"\\*."+exten;
	char fileName[2000];
	struct  _finddata_t fileInfo;
	long findResult = _findfirst(fileFolder.c_str(),&fileInfo);
	if (findResult == -1)
	{
		_findclose(findResult);
		return ;
	}

	do
	{
		sprintf(fileName,"%s\\%s",img_path.c_str(),fileInfo.name);
		if (fileInfo.attrib==_A_ARCH)
		{
			imglist.push_back(fileName);
		}

	}while (!_findnext(findResult,&fileInfo));
	_findclose(findResult); 
}

Mat loadMat(string mat_name)
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

int main( int argc, char** argv ){ 

	int train_type=0;

	string train_path = "another";
	string test_path = "new_test";

	//cout<<"Input train path: ";
	//cin>>train_path;
	//cout<<"Input test path: ";
	//cin>>test_path;

	vector<string> trainlist,testlist;
	getImgList(train_path,trainlist);
	getImgList(test_path,testlist);

	map<string,int> emotion_2_number;
	vector<vector<double>> vec_traindata;
	vector<int> vec_trainlabel;

	cout<<"training img : "<<trainlist.size()<<endl;
	cout<<"test img : "<<testlist.size()<<endl;

	printf("Loading Model!\nWait......\n");
	string file_name = "shape_1.dat";
	predict_log.open("predict_log.txt");

	predict_log<<"DR_TYPE "<<DR_TYPE<<endl;

	if (!LBF_Model_Load(file_name, model))
	{
		cout<<"Can not load the file!"<<endl;
		return 0;
	}

	cout<<"==============Now Loading Img Feature ============="<<endl;
	int use_model_file=0; 

	//DR 
	//DR 0 No PCA OR LDA
	//DR 1 With PCA
	//DR 2 with PCA+LDA

	PCA pca;
	LDA lda;
	CvSVM SVM;
	string lda_name="with_lda";
	string pca_name="pca_lda";
	string svm_name="svm_model_with_all";
	//Mat showCom(600,600,CV_32FC1);
	ofstream out_pri_2("out_prin.txt");
	if (!use_model_file)
	{
		doTrain(vec_traindata,vec_trainlabel,trainlist);
		Mat training_mat(vec_trainlabel.size(),vec_traindata[0].size(),CV_32FC1);
		Mat train_label(vec_trainlabel.size(),1,CV_32FC1);
		cout<<"\n---------Img Feature Load Finished -----------"<<endl;
		cout<<"Data Dimension : "<<vec_traindata[0].size()<<endl;
		cout<<"Find Feature Data : "<<vec_trainlabel.size()<<endl<<endl;
		for (int i = 0; i < vec_trainlabel.size(); i++)
		{
			train_label.at<float>(i,0) = vec_trainlabel[i];
		}

		for (int i = 0; i < vec_traindata.size(); i++)
		{
			Mat tp(vec_traindata[i]);
			tp=tp.t();
			tp.copyTo(training_mat.row(i));
		}

		cout<<"\n==============Now SVM Training ============="<<endl;
		CvSVMParams params;
		params.svm_type = CvSVM::C_SVC;
		params.kernel_type = CvSVM::LINEAR;
		params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER,1000,5e-3);
		//CvSVMParams(,)
		params.C= 0.131250;
		params.p=5e-3;
		params.gamma=0.01;



		Mat final_train;
		Mat dst;Mat cv32;Mat ldadst;
		if (DR_TYPE==WITH_PCA||DR_TYPE==WITH_PCA_LDA)
		{

			//Here comes PCA 
			ofstream pca_data_eigenvalues(pca_name+"_eigenvalues");
			ofstream pca_data_eigenvectors(pca_name+"_eigenvectors");
			ofstream pca_data_mean(pca_name+"_mean");

			pca(training_mat, Mat(),CV_PCA_DATA_AS_ROW,25); 

			pca_data_eigenvalues<<pca.eigenvalues<<endl;
			pca_data_eigenvectors<<pca.eigenvectors<<endl;
			pca_data_mean<<pca.mean<<endl;
			pca_data_eigenvalues.close();
			pca_data_eigenvectors.close();
			pca_data_mean.close();

			dst= pca.project(training_mat);


			if (DR_TYPE==WITH_PCA_LDA)
			{
				lda.compute(dst,train_label);
				ldadst=lda.project(dst);
				lda.save(lda_name);

				ldadst.convertTo(cv32,CV_32FC1);
				//training_mat.deallocate();
				final_train=cv32.clone();
			}
			else
			{
				//training_mat.deallocate();
				final_train=dst.clone();
			}
		}
		else
		{
			final_train=training_mat;
		}

		for (int i = 0; i < vec_trainlabel.size(); i++)
		{

			out_pri_2<<vec_trainlabel[i]<<","<<final_train.at<float>(i,0)<<","<<final_train.at<float>(i,1)<<";";

		}
		out_pri_2.close();
		
		
		cout<<"Auto Train"<<endl;
		CvSVM atSVM;
		CvParamGrid CvParamGrid_C(pow(2.0,-5), pow(2.0,15), pow(2.0,2));
		CvParamGrid CvParamGrid_gamma(pow(2.0,-15), pow(2.0,3), pow(2.0,2));
		if (!CvParamGrid_C.check() || !CvParamGrid_gamma.check())
		cout<<"The grid is NOT VALID."<<endl;
		CvSVMParams paramz;
		paramz.kernel_type = CvSVM::LINEAR;
		paramz.svm_type = CvSVM::C_SVC;
		paramz.term_crit = cvTermCriteria(CV_TERMCRIT_ITER,100,0.000001);
		atSVM.train_auto(final_train, train_label, Mat(), Mat(), paramz,10, CvParamGrid_C, CvParamGrid_gamma, CvSVM::get_default_grid(CvSVM::P), CvSVM::get_default_grid(CvSVM::NU), CvSVM::get_default_grid(CvSVM::COEF), CvSVM::get_default_grid(CvSVM::DEGREE), false);
		
		//Parms: C = 0.031250, P = 0.000000,gamma = 1.000000

		CvSVMParams params_re = atSVM.get_params();

		float C = params_re.C;
		float P = params_re.p;
		float gamma = params_re.gamma;

		cout<<final_train<<endl;
		printf("\nParms: C = %f, P = %f,gamma = %f \n",C,P,gamma);
		cout<<"~~~~~~~~~~~~Auto Train~~~~~~~~~~~~~~"<<endl;

		//params.C=C;
		//params.gamma=gamma;
		
		SVM.train(final_train,train_label,Mat(),Mat(),params);
		SVM.save(svm_name.c_str());
		cout<<"\n---------Training Finished & File Saved -----------"<<endl;
	}
	else
	{

		if (DR_TYPE==WITH_PCA||DR_TYPE==WITH_PCA_LDA)
		{
			pca.eigenvalues=loadMat(pca_name+"_eigenvalues");
			pca.eigenvectors=loadMat(pca_name+"_eigenvectors");
			pca.mean=loadMat(pca_name+"_mean");
			if (DR_TYPE==WITH_PCA_LDA)
			{
				lda.load(lda_name);
			}
		}

		SVM.load(svm_name.c_str());
	}


	cout<<"Load Map String - Code "<<endl;
	ifstream map_file("ecode_map.txt");
	while (!map_file.eof())
	{
		int ecode;
		string estring;
		map_file>>estring>>ecode;
		emotion_2_number.insert(pair<string,int>(estring,ecode));
	}
	map_file.close();

	map <string, int>::iterator m1_Iter;
	for (m1_Iter = emotion_2_number.begin(); m1_Iter!=emotion_2_number.end(); m1_Iter++)
	{

		cout<<"Emotion: "<<m1_Iter->first<<" | "<<" Code "<<m1_Iter->second<<endl;
	}

	cout<<"----start predict--------"<<endl;

	doPredict(testlist,SVM,emotion_2_number,pca,lda);


	return 0;
}

