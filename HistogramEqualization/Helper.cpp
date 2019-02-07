#include "pch.h"
#include "Helper.h"

string Helper::MatTypeToString(int type)
{
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

void Helper::histDisplay8bit(int histogram[], const char* name)
{
	int hist[256];
	for (int i = 0; i < 256; i++)
	{
		hist[i] = histogram[i];
	}
	// draw the histograms
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / 256);

	cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(255, 255, 255));

	// find the maximum intensity element from histogram
	int max = hist[0];
	for (int i = 1; i < 256; i++) {
		if (max < hist[i]) {
			max = hist[i];
		}
	}

	// normalize the histogram between 0 and histImage.rows

	for (int i = 0; i < 256; i++) {
		hist[i] = ((double)hist[i] / max)*histImage.rows;
	}


	// draw the intensity line for histogram
	for (int i = 0; i < 256; i++)
	{
		line(histImage, cv::Point(bin_w*(i), hist_h),
			cv::Point(bin_w*(i), hist_h - hist[i]),
			cv::Scalar(0, 0, 0), 1, 8, 0);
	}

	// display histogram
	cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
	imshow(name, histImage);
}

void Helper::imhist8bit(cv::Mat image, int histogram[])
{

	// initialize all intensity values to 0
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}

	// calculate the no of pixels for each intensity values
	for (int y = 0; y < image.rows; y++)
		for (int x = 0; x < image.cols; x++)
			histogram[(int)image.at<uchar>(y, x)]++;

}

void Helper::histDisplay16bit(int histogram[], const char* name)
{
	const int hist_size = 65536;
	int hist[hist_size];
	for (int i = 0; i < hist_size; i++)
	{
		hist[i] = histogram[i];
	}
	// draw the histograms
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / hist_size);

	cv::Mat histImage(hist_h, hist_w, CV_16UC1, cv::Scalar(255, 255, 255));

	// find the maximum intensity element from histogram
	int max = hist[0];
	for (int i = 1; i < hist_size; i++) {
		if (max < hist[i]) {
			max = hist[i];
		}
	}

	// normalize the histogram between 0 and histImage.rows

	for (int i = 0; i < hist_size; i++) {
		hist[i] = ((double)hist[i] / max)*histImage.rows;
	}


	// draw the intensity line for histogram
	for (int i = 0; i < hist_size; i++)
	{
		line(histImage, cv::Point(bin_w*(i), hist_h),
			cv::Point(bin_w*(i), hist_h - hist[i]),
			cv::Scalar(0, 0, 0), 1, 8, 0);
	}

	// display histogram
	cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
	imshow(name, histImage);
}

void Helper::imhist16bit(cv::Mat image, int histogram[])
{
	const int hist_size = 65536;

	// initialize all intensity values to 0
	for (int i = 0; i < hist_size; i++)
	{
		histogram[i] = 0;
	}

	// calculate the no of pixels for each intensity values
	for (int y = 0; y < image.rows; y++)
		for (int x = 0; x < image.cols; x++)
			histogram[(int)image.at<ushort>(y, x)]++;

}
