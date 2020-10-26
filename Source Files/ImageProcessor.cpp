#include "ImageProcessor.h"

ImageProcessor::ImageProcessor()
{
}

ImageProcessor::~ImageProcessor()
{
}

int ImageProcessor::loadFrontImage(const char* imagePath) {
	frontImage = cv::imread(imagePath, cv::IMREAD_COLOR);
	frontImageFileName = imagePath;
	return 1;
}

int ImageProcessor::loadBackImage(const char* imagePath) {
	backImage = cv::imread(imagePath, cv::IMREAD_COLOR);
	backImageFileName = imagePath;
	return 1;
}

Mat ImageProcessor::executeBackgroundSubstraction(Mat image) {

	Mat grayScaleImage, blurImage, resultImage;
	resultImage = Mat::zeros(image.rows, image.cols, CV_8UC3);

	blur(image, blurImage, cv::Size(15, 15));

	for (int i = 0; i < blurImage.rows; i++)
	{
		for (int j = 0; j < blurImage.cols; j++)
		{
			cv::Vec3b color = blurImage.at<cv::Vec3b>(cv::Point(j, i));

			if (color[0] <= 250 && color[1] <= 250 && color[2] <= 250)
			{
				color[0] = 255;
				color[1] = 255;
				color[2] = 255;
				resultImage.at<cv::Vec3b>(cv::Point(j, i)) = color;
			}
			else {
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;
				resultImage.at<cv::Vec3b>(cv::Point(j, i)) = color;
			}

			/*if (color[0] >= 5 && color[1] >= 5 && color[2] >= 5)
			{
				color[0] = 255;
				color[1] = 255;
				color[2] = 255;
				resultImage.at<cv::Vec3b>(cv::Point(j, i)) = color;
			}
			else {
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;
				resultImage.at<cv::Vec3b>(cv::Point(j, i)) = color;
			}*/
		}
	}

	cvtColor(resultImage, resultImage, cv::COLOR_BGR2GRAY);
	return resultImage;

}

void ImageProcessor::processFrontImage() {

	int interval = 10;
	int threshold = 100;

	Mat backgroundSubstactedImage = executeBackgroundSubstraction(frontImage);
	Mat element = getStructuringElement(MORPH_RECT,
		Size(21,21),
		Point(7, 7));

	/// Apply the erosion operation
	erode(backgroundSubstactedImage, backgroundSubstactedImage, element);
	findContours(backgroundSubstactedImage, frontImageContourSet, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));
}

void ImageProcessor::processBackImage() {
	int threshold = 100;

	Mat backgroundSubstactedImage = executeBackgroundSubstraction(backImage);
	Mat element = getStructuringElement(MORPH_RECT,
		Size(21, 21),
		Point(7, 7));

	/// Apply the erosion operation
	erode(backgroundSubstactedImage, backgroundSubstactedImage, element);
	findContours(backgroundSubstactedImage, backImageContourSet, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));

}