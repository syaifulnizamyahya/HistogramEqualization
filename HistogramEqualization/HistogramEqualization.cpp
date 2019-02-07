// HistogramEqualization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "HistogramEqualization.h"
#include "Helper.h"
#include "../Histogram/Histogram.h"

using namespace cv;
using namespace std;

// --TODO : Write a C++ program to read a 16 - bit grayscale image
// --TODO : perform histogram equalization *directly on the 16 - bit image*, 
// --TODO : save the resulting equalized image(also as a 16 - bit image).
// --TODO : write a simple class for this.
// --TODO : Write a separate class to test the above class.
// TODO : use CMake for this task

void Task()
{
	// read 16 bit image
	string imageName = "16bit_grayscale.TIF";
	string inputPath = "Image\\";
	string inputFilename = inputPath + imageName;
	cout << "Reading image : " << inputFilename << endl;
	Mat image = imread(inputFilename, IMREAD_ANYDEPTH);
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return;
	}

	// perform histogram equalization
	Mat resultImage;
	cout << "Histogram equalization." << endl;
	Histogram::equalizeHistogram(image, resultImage);

	// save resulting equalize image as a 16 bit image
	string outputPath = "Output\\";
	string outputFileName = outputPath + imageName;
	//if exist, delete
	if (remove(outputFileName.c_str()) != 0)
		cout << "Error deleting " << outputFileName << endl;
	cout << "Writing image : " << outputFileName << endl;
	try {
		imwrite(outputFileName, resultImage);
	}
	catch (cv::Exception& ex) {
		cout << "Exception converting image : " << ex.what();
		return;
	}
	cout << "Done histogram equalization!" << endl;
}

void Dev()
{
	const string inputWindowName = "dInput window";
	const string outputWindowName = "dOutput window";

	String imageName;
	imageName = "Image\\16bit_grayscale.TIF"; // by default
	//imageName = "Image\\Hilbert_Curve.256x256,16-bit_greyscale.png"; // by default
	//imageName = "Image\\test.tif"; // by default
	//imageName = "Image\\8bit_grayscale_sm.png"; // by default

	Mat image;
	image = imread(imageName, IMREAD_ANYDEPTH); 

	if (image.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return;
	}

	// get small region matrix for 16bit verification
	Rect r1(0, 0, 10, 10);
	Mat smallImg;
	smallImg = image(r1);
	ofstream input("input.txt");
	if (input.is_open())
	{
		input << "(Input) Matrix for first 10x10 pixels = " << endl << " " << smallImg << endl << endl;
		input.close();
		cout << "(Input) Matrix for first 10x10 pixels : " << endl;
		cout << smallImg << endl << endl;
	}

	namedWindow(inputWindowName, WINDOW_AUTOSIZE);
	imshow(inputWindowName, image);

	Mat resultImage;

	// displaying image info
	string ty = Helper::MatTypeToString(image.type());
	cout << "Type : " << image.type() << endl;
	cout << "Matrix : " << ty.c_str() << " " << image.cols << "x" << image.rows << endl;
	cout << "Depth : " << image.depth() << endl;
	cout << "Channels : " << image.channels() << endl;

	// benchmarking
	auto type = image.type();
	if (type == 0)
	{
		auto a = getTickCount();
		Histogram::equalizeHistogram(image, resultImage);
		auto b = getTickCount();
		cout << "8 bit histogram equalization time : " << b - a << " ticks." << endl;
	}
	else if (type == 2)
	{
		auto a = getTickCount();
		Histogram::equalizeHistogram(image, resultImage);
		auto b = getTickCount();
		cout << "16 bit histogram equalization time : " << b - a  << " ticks."<< endl;
	}
	else
	{
		cout << "Unsupported image" << std::endl;
		return;
	}

	// get small region matrix for 16bit verification
	Rect r2(0, 0, 10, 10);
	smallImg = resultImage(r2);
	ofstream output("output.txt");
	if (output.is_open())
	{
		output << "(output) Matrix for first 10x10 pixels = " << endl << " " << smallImg << endl << endl;
		output.close();
		cout << "(output) Matrix for first 10x10 pixels : " << endl;
		cout << smallImg << endl << endl;
	}

	namedWindow(outputWindowName, WINDOW_AUTOSIZE);
	imshow(outputWindowName, resultImage);
}

int main(int argc, char** argv)
{
	Dev();
	Task();

	waitKey(0);

	return 0;
}
