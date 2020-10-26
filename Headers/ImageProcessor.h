#pragma once
#include<windows.h>
#include<iostream>
#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

class ImageProcessor
{
public:
	
	const char* frontImageFileName = NULL;
	const char* backImageFileName = NULL;

	Mat frontImage, backImage;
	vector<vector<Point> > frontImageContourSet;
	vector<vector<Point>> backImageContourSet;

	ImageProcessor();
	~ImageProcessor();

	int loadFrontImage(const char* imagePath);
	int loadBackImage(const char* imagePath);
	void processFrontImage();
	void processBackImage();


private:
	Mat executeBackgroundSubstraction(Mat image);
};

