#include "ImgLandMark.h"


ImgLandMark::ImgLandMark(void)
{
}


ImgLandMark::~ImgLandMark(void)
{
}



void ImgLandMark::loadModel()
{
	face_cascade_name = "haarcascade_frontalface_alt.xml"; 
	string file_name = "shape_1.dat";

	if (!LBF_Model_Load(file_name, model))
	{
		cout<<"Can not load the file!"<<endl;
		return;
	}
}


string ImgLandMark::FACS_info(Mat &input)
{
	vector<Point> face_landmark_fixed;
	if (getLandmark(input,face_landmark_fixed)==-1)
	{
		return "NoneFace";
	}
	landMarkInfo(face_landmark_fixed);
}

void ImgLandMark::landMarkInfo(vector<Point>& input)
{

}

int ImgLandMark::getLandmark(Mat &frame,vector<Point> &vec_landmark)
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
