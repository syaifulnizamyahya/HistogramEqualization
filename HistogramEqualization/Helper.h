#pragma once

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

class Helper
{
public:
	static string MatTypeToString(int type);
	static void histDisplay8bit(int histogram[], const char* name);
	static void imhist8bit(cv::Mat image, int histogram[]);
	static void histDisplay16bit(int histogram[], const char* name);
	static void imhist16bit(cv::Mat image, int histogram[]);
};

