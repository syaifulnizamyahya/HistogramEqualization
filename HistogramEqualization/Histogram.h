#pragma once
#include <iostream>
#include <vector>
#include <opencv2/imgproc.hpp>

class Histogram
{
private:
	static void equalizeHist8Bit(const cv::Mat &_src, cv::Mat &_dst);
	static void equalizeHist16BitVector(const cv::Mat &_src, cv::Mat &_dst);
	static void equalizeHist16BitArray(const cv::Mat &_src, cv::Mat &_dst);
public:
	static void equalizeHistogram(const cv::Mat &_src, cv::Mat &_dst);

};

