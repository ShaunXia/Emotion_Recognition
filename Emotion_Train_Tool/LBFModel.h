#pragma once

#ifndef LBF_MODEL_H
#define LBF_MODEL_H
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

using namespace std;
using namespace cv;

extern CascadeClassifier face_cascade;
#define OPEN_TEMPLATE_FILE_FAIL 0

struct split_feature_load
{
	unsigned long idx1;
	unsigned long idx2;
	float thresh;
};


struct regression_tree_load
{
	std::vector<split_feature_load> splits;
	std::vector<std::vector<float> > leaf_values;
};
struct deltas_values{
	float a;
	float b;
};
struct LBF_model{
	std::vector<float> initial_shape;
	std::vector<std::vector<unsigned long> > anchor_idx;
	std::vector<std::vector<deltas_values > > deltas;
	std::vector<std::vector<regression_tree_load> > forests;
};


template <class T>
struct point{
	T x;
	T y;
};

struct point_transform_affine{
	CvMat* m;
	point<double> b;
};

int LBF_Model_Load(string file_name, LBF_model& models/*shape_predictor sp*/);


template <class T_1, class T_2>
double length_squared(point<T_1> point_1, point<T_2> point_2);

void find_similarity_transform (std::vector<point<float> >& from_points, std::vector<point<float> >& to_points, point_transform_affine& p);

void unnormalizing_tform(double rect[4], point_transform_affine& p);


void find_tform_between_shapes(std::vector<float> from_shape, std::vector<float> to_shape, point_transform_affine& p);

template <typename image_type>
void extract_feature_pixel_values (
	image_type& img_,
	double* rect,
	std::vector<float>& current_shape,
	std::vector<float>& reference_shape,
	std::vector<unsigned long>& reference_pixel_anchor_idx,
	std::vector<deltas_values >& reference_pixel_deltas,
	std::vector<float>& feature_pixel_values
	){
		point_transform_affine tform;
		find_tform_between_shapes(reference_shape, current_shape, tform);

		point_transform_affine tform_to_img;
		unnormalizing_tform(rect, tform_to_img);

		feature_pixel_values.resize(reference_pixel_deltas.size());
		for (unsigned long i = 0; i < feature_pixel_values.size(); ++i)
		{
			// Compute the point in the current shape corresponding to the i-th pixel and
			// then map it from the normalized shape space into pixel space.
			point<double> point_p;

			point_p.x = cvmGet(tform.m, 0, 0)*reference_pixel_deltas[i].a + cvmGet(tform.m, 0, 1)*reference_pixel_deltas[i].b + current_shape[reference_pixel_anchor_idx[i]*2];
			point_p.y = cvmGet(tform.m, 1, 0)*reference_pixel_deltas[i].a + cvmGet(tform.m, 1, 1)*reference_pixel_deltas[i].b + current_shape[reference_pixel_anchor_idx[i]*2 + 1];

			int point_temp_x = cvmGet(tform_to_img.m, 0, 0)*point_p.x + cvmGet(tform_to_img.m, 0, 1)*point_p.y + tform_to_img.b.x;
			int point_temp_y = cvmGet(tform_to_img.m, 1, 0)*point_p.x + cvmGet(tform_to_img.m, 1, 1)*point_p.y + tform_to_img.b.y;

			if (point_temp_x > 0 && point_temp_x < img_.cols && point_temp_y > 0 && point_temp_y < img_.rows)
			{
				//feature_pixel_values[i] = (float)(img_.at<unsigned char>(point_temp_y, point_temp_x));
				feature_pixel_values[i] = img_.template at<Vec3b>(point_temp_y, point_temp_x)[0]*0.2989 + img_.template at<Vec3b>(point_temp_y, point_temp_x)[1]*0.5870 + img_.template at<Vec3b>(point_temp_y, point_temp_x)[2]*0.1140;
				//feature_pixel_values[i] = (img_.at<Vec3b>(point_temp_y, point_temp_x)[0] + img_.at<Vec3b>(point_temp_y, point_temp_x)[1] + img_.at<Vec3b>(point_temp_y, point_temp_x)[2])/3;
			}
			else
			{
				feature_pixel_values[i] = 0;
			}
		}

}

std::vector<float>& get_delta_shape(regression_tree_load& tree,const std::vector<float>& feature_pixel_values);

void change_shape(std::vector<float>& current_shape, std::vector<float>& delta_shape);

template <typename image_type>
void calculate_shape(image_type& img, double* rect, LBF_model& model, std::vector<point<double> >& parts)
{
	std::vector<float> current_shape = model.initial_shape;
	std::vector<float> feature_pixel_values;

	for (int iter = 0; iter < model.forests.size()/* - 12*/; ++iter)
	{
		extract_feature_pixel_values(img, rect, current_shape, model.initial_shape, model.anchor_idx[iter], model.deltas[iter], feature_pixel_values);

		for (unsigned long i = 0; i < model.forests[iter].size(); ++i)
		{
			change_shape(current_shape, get_delta_shape(model.forests[iter][i], feature_pixel_values));
		}
	}

	point_transform_affine tform_to_img;
	unnormalizing_tform(rect, tform_to_img);
	//std::vector<point<double> > parts(current_shape.size()/2);
	parts.resize(current_shape.size()/2);
	for (unsigned long i = 0; i < parts.size(); ++i)
	{
		parts[i].x = cvmGet(tform_to_img.m, 0, 0)*current_shape[2*i] + cvmGet(tform_to_img.m, 0, 1)*current_shape[2*i + 1] + tform_to_img.b.x;
		parts[i].y = cvmGet(tform_to_img.m, 1, 0)*current_shape[2*i] + cvmGet(tform_to_img.m, 1, 1)*current_shape[2*i + 1] + tform_to_img.b.y;
		//cout<<parts[i].x<<"	"<<parts[i].y<<endl;
	}
	//cout<<"ok"<<endl;
}

void get_rectangles(std::vector<point<float> >& temp, double* box);

double compute_error(std::vector<point<double> >& pre, std::vector<point<float> > gt);



#endif