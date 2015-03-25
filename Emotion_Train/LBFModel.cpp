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
#include "LBFModel.h"

using namespace std;
using namespace cv;


CascadeClassifier face_cascade;
#define OPEN_TEMPLATE_FILE_FAIL 0

int LBF_Model_Load(string file_name, LBF_model& models/*shape_predictor sp*/){
	FILE *initial_shape_read = fopen(file_name.data(), "rb");
	if(NULL==initial_shape_read){
		cout<<"Can not open this file!"<<endl;
		return OPEN_TEMPLATE_FILE_FAIL;
	}


	/////////////////////////////////////////--------initial_shape
	int initial_shape_size = 0;
	fread(&initial_shape_size, sizeof(int), 1, initial_shape_read);
	std::vector<float> initial_shape(initial_shape_size/*sp.initial_shape.size()*/);
	for (int i = 0; i < initial_shape_size /*sp.initial_shape.size()*/; ++i)
	{
		fread(&initial_shape[i], sizeof(float), 1, initial_shape_read);
	}

	/////////////////////////////////////////--------anchor_idx
	int anchor_idx_size = 0;
	fread(&anchor_idx_size, sizeof(int), 1, initial_shape_read);
	int anchor_idx_size_size = 0;
	std::vector<std::vector<unsigned long> > anchor_idx(anchor_idx_size/*sp.anchor_idx.size()*/);
	for (int i = 0; i < anchor_idx_size /*sp.anchor_idx.size()*/; ++i)
	{
		fread(&anchor_idx_size_size, sizeof(int), 1, initial_shape_read);
		anchor_idx[i].resize(anchor_idx_size_size/*sp.anchor_idx[i].size()*/);
		for (int j = 0; j < anchor_idx_size_size/*sp.anchor_idx[i].size()*/; ++j)
		{
			fread(&anchor_idx[i][j], sizeof(unsigned long), 1, initial_shape_read);
		}
	}

	/////////////////////////////////////////--------deltas
	int deltas_size = 0;
	fread(&deltas_size, sizeof(int), 1, initial_shape_read);
	int deltas_size_size = 0;
	std::vector<std::vector<deltas_values > > deltas(deltas_size);
	for (int i = 0; i < deltas_size /*sp.deltas.size()*/; ++i)
	{
		fread(&deltas_size_size, sizeof(int), 1, initial_shape_read);
		deltas[i].resize(deltas_size_size);
		for (int j = 0; j < deltas_size_size /*sp.deltas[i].size()*/; ++j)
		{
			fread(&deltas[i][j].a, sizeof(float), 1, initial_shape_read);
			fread(&deltas[i][j].b, sizeof(float), 1, initial_shape_read);
		}
	}

	/////////////////////////////////////////--------forest
	int forest_size = 0;
	fread(&forest_size, sizeof(int), 1, initial_shape_read);
	int forests_size_size = 0;
	int forests_size_size_splits_size = 0;
	int forests_size_size_tree_leaf_size = 0;
	int forests_size_size_tree_leaf_size_size = 0;

	std::vector<std::vector<regression_tree_load> > forest_load(forest_size/*sp.forests.size()*/);
	for (int vector_one = 0; vector_one < forest_size /*sp.forests.size()*/; ++vector_one)
	{
		fread(&forests_size_size, sizeof(int), 1, initial_shape_read);
		forest_load[vector_one].resize(forests_size_size/*sp.forests[vector_one].size()*/);
		for (int vector_two = 0; vector_two < forests_size_size /*sp.forests[vector_one].size()*/; ++vector_two)
		{
			fread(&forests_size_size_splits_size, sizeof(int), 1, initial_shape_read);
			regression_tree_load* tree = &forest_load[vector_one][vector_two];
			(*tree).splits.resize(forests_size_size_splits_size/*sp.forests[vector_one][vector_two].splits.size()*/);
			for (int split_size = 0; split_size < forests_size_size_splits_size/*sp.forests[vector_one][vector_two].splits.size()*/; ++split_size)
			{
				fread(&(*tree).splits[split_size].idx1, sizeof(unsigned long), 1, initial_shape_read);
				fread(&(*tree).splits[split_size].idx2, sizeof(unsigned long), 1, initial_shape_read);
				fread(&(*tree).splits[split_size].thresh, sizeof(unsigned long), 1, initial_shape_read);
			}

			fread(&forests_size_size_tree_leaf_size, sizeof(int), 1, initial_shape_read);

			(*tree).leaf_values.resize(forests_size_size_tree_leaf_size/*sp.forests[vector_one][vector_two].leaf_values.size()*/);
			for (int leaf_values_size = 0; leaf_values_size < forests_size_size_tree_leaf_size /*sp.forests[vector_one][vector_two].leaf_values.size()*/; ++leaf_values_size)
			{
				fread(&forests_size_size_tree_leaf_size_size, sizeof(int), 1, initial_shape_read);
				(*tree).leaf_values[leaf_values_size].resize(forests_size_size_tree_leaf_size_size/*sp.forests[vector_one][vector_two].leaf_values[leaf_values_size].size()*/);
				for (int leaf_values_size_in = 0; leaf_values_size_in < forests_size_size_tree_leaf_size_size /*sp.forests[vector_one][vector_two].leaf_values[leaf_values_size].size()*/; ++leaf_values_size_in)
				{
					fread(&((*tree).leaf_values[leaf_values_size][leaf_values_size_in]), sizeof(float), 1, initial_shape_read);
				}

			}
		}
	}

	fclose(initial_shape_read);
	models.initial_shape = initial_shape;
	models.deltas = deltas;
	models.anchor_idx = anchor_idx;
	models.forests = forest_load;
	return 1;
}


template <class T_1, class T_2>
double length_squared(point<T_1> point_1, point<T_2> point_2)
{
	return (((T_2)point_1.x - point_2.x)*((T_2)point_1.x - point_2.x) + ((T_2)point_1.y - point_2.y)*((T_2)point_1.y - point_2.y));
}

void find_similarity_transform (std::vector<point<float> >& from_points, std::vector<point<float> >& to_points, point_transform_affine& p)
{
	point<double> mean_from, mean_to;
	mean_from.x = 0;
	mean_from.y = 0;
	mean_to.x = 0;
	mean_to.y = 0;
	double sigma_from = 0, sigma_to = 0;
	CvMat* cov = cvCreateMat(2, 2, CV_64FC1);
	cvZero(cov);

	for (unsigned int i = 0; i < from_points.size(); ++i)
	{
		mean_from.x += from_points[i].x;
		mean_from.y += from_points[i].y;
		mean_to.x += to_points[i].x;
		mean_to.y += to_points[i].y;
	}

	mean_from.x /= from_points.size();
	mean_from.y /= from_points.size();
	mean_to.x /= to_points.size();
	mean_to.y /= to_points.size();

	double cov_temp[4] = {0, 0, 0, 0};
	for (unsigned long i = 0; i < from_points.size(); ++i)
	{
		sigma_from += length_squared<float, double>(from_points[i], mean_from);
		sigma_to += length_squared<float, double>(to_points[i], mean_to);
		cov_temp[0] += (to_points[i].x - mean_to.x)*(from_points[i].x - mean_from.x); 
		cov_temp[1] += (to_points[i].x - mean_to.x)*(from_points[i].y - mean_from.y);
		cov_temp[2] += (to_points[i].y - mean_to.y)*(from_points[i].x - mean_from.x);
		cov_temp[3] += (to_points[i].y - mean_to.y)*(from_points[i].y - mean_from.y);
	}
	sigma_from /= from_points.size();
	sigma_to   /= from_points.size();

	cov_temp[0] /= from_points.size();
	cov_temp[1] /= from_points.size();
	cov_temp[2] /= from_points.size();
	cov_temp[3] /= from_points.size();
	cvmSet(cov, 0, 0, cov_temp[0]);
	cvmSet(cov, 0, 1, cov_temp[1]);
	cvmSet(cov, 1, 0, cov_temp[2]);
	cvmSet(cov, 1, 1, cov_temp[3]);

	CvMat* u = cvCreateMat(2, 2, CV_64FC1);
	CvMat* v = cvCreateMat(2, 2, CV_64FC1);
	CvMat* d = cvCreateMat(2, 2, CV_64FC1);
	cvSVD(cov, d, u, v, CV_SVD_V_T);

	if (cvDet(cov) < 0)
	{
		if (cvmGet(d, 1, 1) < cvmGet(d, 0, 0))
		{
			cvmSet(u, 1, 1, -cvmGet(u, 1, 1));
			cvmSet(u, 0, 1, -cvmGet(u, 0, 1));
			cvmSet(d, 1, 1, -cvmGet(d, 1, 1));
			cvmSet(d, 0, 1, -cvmGet(d, 0, 1));
		}
		else
		{
			cvmSet(u, 0, 0, -cvmGet(u, 0, 0));
			cvmSet(u, 1, 0, -cvmGet(u, 1, 0));
			cvmSet(d, 0, 0, -cvmGet(d, 0, 0));
			cvmSet(d, 1, 0, -cvmGet(d, 1, 1));
		}
	}

	CvMat* r = cvCreateMat(2, 2, CV_64FC1);
	cvZero(r);
	cvMatMul(u, v, r);

	double c = 1; 
	if (sigma_from != 0)
		c = 1.0/sigma_from * (cvmGet(d, 0, 0) + cvmGet(d, 1, 1));
	point<double> t;

	cvmSet(r, 0, 0, cvmGet(r, 0, 0)*c);
	cvmSet(r, 0, 1, cvmGet(r, 0, 1)*c);
	cvmSet(r, 1, 0, cvmGet(r, 1, 0)*c);
	cvmSet(r, 1, 1, cvmGet(r, 1, 1)*c);

	t.x = mean_to.x - (cvmGet(r, 0, 0)*mean_from.x + cvmGet(r, 0, 1)*mean_from.y);
	t.y = mean_to.y - (cvmGet(r, 1, 0)*mean_from.x + cvmGet(r, 1, 1)*mean_from.y);

	p.m = cvCloneMat(r);
	p.b = t;

}

void unnormalizing_tform(double rect[4], point_transform_affine& p)
{
	std::vector<point<float> > from_points, to_points;
	point<float> temp;
	temp.x = rect[0]; temp.y = rect[1]; to_points.push_back(temp);
	temp.x = 0; temp.y = 0; from_points.push_back(temp);
	temp.x = rect[2]; temp.y = rect[1]; to_points.push_back(temp);
	temp.x = 1; temp.y = 0; from_points.push_back(temp);
	temp.x = rect[2]; temp.y = rect[3]; to_points.push_back(temp);
	temp.x = 1; temp.y = 1; from_points.push_back(temp);
	find_similarity_transform(from_points, to_points, p);
}


void find_tform_between_shapes(std::vector<float> from_shape, std::vector<float> to_shape, point_transform_affine& p)
{
	std::vector<point<float> > from_points, to_points;
	const unsigned long num = from_shape.size()/2;
	if (num == 1)
	{
		exit(1);
	}
	from_points.resize(num);
	to_points.resize(num);
	for (unsigned long i = 0; i < num; ++i)
	{
		from_points[i].x = from_shape[2*i];
		from_points[i].y = from_shape[2*i + 1];
		to_points[i].x = to_shape[2*i];
		to_points[i].y = to_shape[2*i + 1];
	}
	find_similarity_transform(from_points, to_points, p);
}



std::vector<float>& get_delta_shape(regression_tree_load& tree,const std::vector<float>& feature_pixel_values)
{
	unsigned long i = 0;
	float temp = 0.0f;
	while (i < tree.splits.size())
	{
		//temp = feature_pixel_values[tree.splits[i].idx1] > feature_pixel_values[tree.splits[i].idx2] ? feature_pixel_values[tree.splits[i].idx1] : feature_pixel_values[tree.splits[i].idx2];
		if ((feature_pixel_values[tree.splits[i].idx1] - feature_pixel_values[tree.splits[i].idx2])/* / temp */> tree.splits[i].thresh)
			i = 2*i + 1;
		else
			i = 2*i + 2;
	}
	return tree.leaf_values[i - tree.splits.size()];
}

void change_shape(std::vector<float>& current_shape, std::vector<float>& delta_shape)
{
	for (int i = 0; i < delta_shape.size(); ++i)
	{
		current_shape[i] += delta_shape[i];
	}
}


void get_rectangles(std::vector<point<float> >& temp, double* box)
{
	double min_x = temp[0].x;
	double max_x = temp[0].x;
	double min_y = temp[0].y;
	double max_y = temp[0].y;
	for (int i = 0; i < temp.size(); ++i)
	{
		if (min_x > temp[i].x)
		{
			min_x = temp[i].x;
		}
		if (max_x < temp[i].x)
		{
			max_x = temp[i].x;
		}
		if (min_y > temp[i].y)
		{
			min_y = temp[i].y;
		}
		if (max_y < temp[i].y)
		{
			max_y = temp[i].y;
		}
	}
	box[0] = min_x;
	box[1] = min_y;
	box[2] = max_x;
	box[3] = max_y;

}

double compute_error(std::vector<point<double> >& pre, std::vector<point<float> > gt)
{
	double left_eye_x = 0;
	double left_eye_y = 0;
	for (int i = 36; i <= 41; ++i)
	{
		left_eye_x += gt[i].x;
		left_eye_y += gt[i].y;
	}
	left_eye_x /= 6;
	left_eye_y /= 6;

	double right_eye_x = 0;
	double right_eye_y = 0;
	for (int i = 42; i <= 47; ++i)
	{
		right_eye_x += gt[i].x;
		right_eye_y += gt[i].y;
	}
	right_eye_x /= 6;
	right_eye_y /= 6;

	double scalar = sqrt((left_eye_x - right_eye_x) * (left_eye_x - right_eye_x) + (left_eye_y - right_eye_y) * (left_eye_y - right_eye_y));

	double count = 0;
	for (int i = 0; i < gt.size(); ++i)
	{
		count += sqrt((pre[i].x - gt[i].x) * (pre[i].x - gt[i].x) + (pre[i].y - gt[i].y) * (pre[i].y - gt[i].y)) / scalar;
	}
	count /= gt.size();
	return count;
}