// HistogramEqualization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

string type2str(int type) {
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

void equalizeHist8bit(const Mat &_src, Mat &_dst)
{
	_dst = _src.clone();

	const int intSize = 256;
	// Generate the histogram
	int histogram[intSize];

	// initialize all intensity values to 0
	for (int i = 0; i < intSize; i++)
	{
		histogram[i] = 0;
	}

	// calculate the no of pixels for each intensity values
	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
			histogram[(int)_src.at<uchar>(y, x)]++;

	// Caluculate the size of image
	int size = _src.rows * _src.cols;
	float alpha = double(intSize - 1) / size;

	// Calculate the probability of each intensity
	float PrRk[intSize];
	for (int i = 0; i < intSize; i++)
	{
		PrRk[i] = (double)histogram[i] / size;
	}

	// Generate cumulative frequency histogram
	int cumhistogram[intSize];
	cumhistogram[0] = histogram[0];

	for (int i = 1; i < intSize; i++)
	{
		cumhistogram[i] = histogram[i] + cumhistogram[i - 1];
	}

	//Scale the histogram
	int Sk[intSize];
	for (int i = 0; i < intSize; i++)
	{
		Sk[i] = cvRound((double)cumhistogram[i] * alpha);
	}

	// Generate the equlized histogram
	float PsSk[intSize];
	for (int i = 0; i < intSize; i++)
	{
		PsSk[i] = 0;
	}

	for (int i = 0; i < intSize; i++)
	{
		PsSk[Sk[i]] += PrRk[i];
	}

	int final[intSize];
	for (int i = 0; i < intSize; i++)
		final[i] = cvRound(PsSk[i] * (intSize - 1));

	// Generate the equlized image
	_dst = _src.clone();

	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
			_dst.at<uchar>(y, x) = saturate_cast<uchar>(Sk[_src.at<uchar>(y, x)]);
}

void equalizeHist16bit(const Mat &_src, Mat &_dst)
{
	cout << "Using vector" << endl;
	_dst = _src.clone();

	const int intSize = 65536;
	// Generate the histogram
	//array<int, intSize> histogram;
	vector<int> histogram;
	//int histogram[intSize];

	// initialize all intensity values to 0
	for (int i = 0; i < intSize; i++)
	{
		//histogram[i] = 0;
		histogram.push_back(0);
	}

	// calculate the no of pixels for each intensity values
	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
			//histogram[(int)_src.at<uchar>(y, x)]++;
			histogram[(int)_src.at<unsigned short int>(y, x)]++;

	// Caluculate the size of image
	int size = _src.rows * _src.cols;
	float alpha = double(intSize-1) / size;

	// Calculate the probability of each intensity
	//float PrRk[intSize];
	//array<float, intSize> PrRk;
	vector<float> PrRk;
	for (int i = 0; i < intSize; i++)
	{
		//PrRk[i] = (double)histogram[i] / size;
		PrRk.push_back((double)histogram[i] / size);
	}

	// Generate cumulative frequency histogram
	//int cumhistogram[intSize];
	//array<int, intSize> cumhistogram;
	vector<int> cumhistogram;
	//cumhistogram[0] = histogram[0];
	cumhistogram.push_back(histogram[0]);

	for (int i = 1; i < intSize; i++)
	{
		//cumhistogram[i] = histogram[i] + cumhistogram[i - 1];
		cumhistogram.push_back(histogram[i] + cumhistogram[i - 1]);
	}

	//Scale the histogram
	//array<int, intSize> Sk;
	vector<int> Sk;
	//int Sk[intSize];
	for (int i = 0; i < intSize; i++)
	{
		//Sk[i] = cvRound((double)cumhistogram[i] * alpha);
		Sk.push_back(cvRound((double)cumhistogram[i] * alpha));
	}

	// Generate the equlized histogram
	//array<float, intSize> PsSk;
	vector<float> PsSk;
	//float PsSk[intSize];
	for (int i = 0; i < intSize; i++)
	{
		//PsSk[i] = 0;
		PsSk.push_back(0);
	}

	for (int i = 0; i < intSize; i++)
	{
		PsSk[Sk[i]] += PrRk[i];
	}

	//int final[intSize];
	//array<int, intSize> final;
	vector<int> final;
	for (int i = 0; i < intSize; i++)
		//final[i] = cvRound(PsSk[i] * (intSize - 1));
		final.push_back(cvRound(PsSk[i] * (intSize-1)));

	// Generate the equlized image
	_dst = _src.clone();

	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
			_dst.at<unsigned short int>(y, x) = saturate_cast<unsigned short int>(Sk[_src.at<unsigned short int>(y, x)]);
			//_dst.at<uchar>(y, x) = saturate_cast<uchar>(Sk[_src.at<uchar>(y, x)]);
	/*
	*/
	//cin.get();
}

void equalizeHist16bit2(const Mat &_src, Mat &_dst)
{
	cout << "Using array" << endl;
	_dst = _src.clone();

	const int intSize = 65536;
	// Generate the histogram
	//array<int, intSize> histogram;
	//vector<int> histogram;
	//int histogram[intSize];
	int *histogram = new int[intSize];

	// initialize all intensity values to 0
	for (int i = 0; i < intSize; i++)
	{
		histogram[i] = 0;
		//histogram.push_back(0);
	}

	// calculate the no of pixels for each intensity values
	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
			//histogram[(int)_src.at<uchar>(y, x)]++;
			histogram[(int)_src.at<unsigned short int>(y, x)]++;

	// Caluculate the size of image
	int size = _src.rows * _src.cols;
	float alpha = double(intSize - 1) / size;

	// Calculate the probability of each intensity
	//float PrRk[intSize];
	//array<float, intSize> PrRk;
	//vector<float> PrRk;
	float *PrRk= new float[intSize];

	for (int i = 0; i < intSize; i++)
	{
		PrRk[i] = (double)histogram[i] / size;
		//PrRk.push_back((double)histogram[i] / size);
	}

	// Generate cumulative frequency histogram
	//int cumhistogram[intSize];
	//array<int, intSize> cumhistogram;
	//vector<int> cumhistogram;
	int *cumhistogram = new int[intSize];
	cumhistogram[0] = histogram[0];
	//cumhistogram.push_back(histogram[0]);

	for (int i = 1; i < intSize; i++)
	{
		cumhistogram[i] = histogram[i] + cumhistogram[i - 1];
		//cumhistogram.push_back(histogram[i] + cumhistogram[i - 1]);
	}

	//Scale the histogram
	//array<int, intSize> Sk;
	int *Sk = new int[intSize];
	//vector<int> Sk;
	//int Sk[intSize];
	for (int i = 0; i < intSize; i++)
	{
		Sk[i] = cvRound((double)cumhistogram[i] * alpha);
		//Sk.push_back(cvRound((double)cumhistogram[i] * alpha));
	}

	// Generate the equlized histogram
	//array<float, intSize> PsSk;
	//vector<float> PsSk;
	float *PsSk = new float[intSize];
	//float PsSk[intSize];
	for (int i = 0; i < intSize; i++)
	{
		PsSk[i] = 0;
		//PsSk.push_back(0);
	}

	for (int i = 0; i < intSize; i++)
	{
		PsSk[Sk[i]] += PrRk[i];
	}

	//int final[intSize];
	//array<int, intSize> final;
	//vector<int> final;
	int *final = new int[intSize];
	for (int i = 0; i < intSize; i++)
		final[i] = cvRound(PsSk[i] * (intSize - 1));
		//final.push_back(cvRound(PsSk[i] * (intSize - 1)));

	// Generate the equlized image
	_dst = _src.clone();

	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
			_dst.at<unsigned short int>(y, x) = saturate_cast<unsigned short int>(Sk[_src.at<unsigned short int>(y, x)]);
	//_dst.at<uchar>(y, x) = saturate_cast<uchar>(Sk[_src.at<uchar>(y, x)]);
/*
*/
	//cin.get();
}

int main(int argc, char** argv)
{
	String imageName;
	imageName = "Image\\16bit_grayscale.TIF"; // by default
	//imageName = "Image\\Hilbert_Curve.256x256,16-bit_greyscale.png"; // by default
	//imageName = "Image\\test.tif"; // by default
	//imageName = "Image\\8bit_grayscale_sm.png"; // by default

	Mat image;
	image = imread(imageName, IMREAD_ANYDEPTH); // Read the file
	//image = imread(imageName, IMREAD_COLOR); // Read the file

	if (image.empty())                      // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		cin.get();
		return -1;
	}

	// select small region
	Rect r1(0, 0, 10, 10);
	Mat smallImg;
	smallImg = image(r1);

	ofstream input("input.txt");
	if (input.is_open())
	{
		input << "M = " << endl << " " << smallImg << endl << endl;
		input.close();
	}

	namedWindow("Input window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Input window", image);                // Show our image inside it.

	Mat resultImage;
	//cvtColor(image, image, COLOR_BGR2GRAY);
	//equalizeHist(image, resultImage);
	string ty = type2str(image.type());
	cout << "Type : " << image.type() << endl;
	cout << "Matrix: " << ty.c_str() << " " << image.cols << "x" << image.rows << endl;
	cout << "depth : " << image.depth() << endl;
	cout << "channels : " << image.channels() << endl;

	if (image.type()==0)
	{
		equalizeHist8bit(image, resultImage);
	}
	else if (image.type() == 2)
	{
		equalizeHist16bit2(image, resultImage);
	}

	// select small region
	Rect r2(0, 0, 10, 10);
	smallImg = resultImage(r2);

	ofstream output("output.txt");
	if (output.is_open())
	{
		output << "M = " << endl << " " << smallImg << endl << endl;
		output.close();
	}

	imshow("Equalized Image", resultImage);

	waitKey(0); // Wait for a keystroke in the window

	//CommandLineParser parser(argc, argv, "{@input | ../data/lena.jpg | input image}");
	//Mat src = imread(parser.get<String>("@input"), IMREAD_COLOR);
	//if (src.empty())
	//{
	//	cout << "Could not open or find the image!\n" << endl;
	//	cout << "Usage: " << argv[0] << " <Input image>" << endl;
	//	return -1;
	//}
	//cvtColor(src, src, COLOR_BGR2GRAY);
	//Mat dst;
	//equalizeHist(src, dst);
	//imshow("Source image", src);
	//imshow("Equalized Image", dst);
	//waitKey();

	cin.get();
	return 0;
}


//void equalizeHist(InputArray _src, OutputArray _dst)
//{
//	//CV_INSTRUMENT_REGION();
//
//	CV_Assert(_src.type() == CV_8UC1);
//
//	if (_src.empty())
//		return;
//
//	CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
//		ocl_equalizeHist(_src, _dst))
//
//		Mat src = _src.getMat();
//	_dst.create(src.size(), src.type());
//	Mat dst = _dst.getMat();
//
//	CV_OVX_RUN(!ovx::skipSmallImages<VX_KERNEL_EQUALIZE_HISTOGRAM>(src.cols, src.rows),
//		openvx_equalize_hist(src, dst))
//
//		Mutex histogramLockInstance;
//
//	const int hist_sz = EqualizeHistCalcHist_Invoker::HIST_SZ;
//	int hist[hist_sz] = { 0, };
//	int lut[hist_sz];
//
//	EqualizeHistCalcHist_Invoker calcBody(src, hist, &histogramLockInstance);
//	EqualizeHistLut_Invoker      lutBody(src, dst, lut);
//	cv::Range heightRange(0, src.rows);
//
//	if (EqualizeHistCalcHist_Invoker::isWorthParallel(src))
//		parallel_for_(heightRange, calcBody);
//	else
//		calcBody(heightRange);
//
//	int i = 0;
//	while (!hist[i]) ++i;
//
//	int total = (int)src.total();
//	if (hist[i] == total)
//	{
//		dst.setTo(i);
//		return;
//	}
//
//	float scale = (hist_sz - 1.f) / (total - hist[i]);
//	int sum = 0;
//
//	for (lut[i++] = 0; i < hist_sz; ++i)
//	{
//		sum += hist[i];
//		lut[i] = saturate_cast<uchar>(sum * scale);
//	}
//
//	if (EqualizeHistLut_Invoker::isWorthParallel(src))
//		parallel_for_(heightRange, lutBody);
//	else
//		lutBody(heightRange);
//}

/*
class EqualizeHistCalcHist_Invoker : public cv::ParallelLoopBody
{
public:
	enum {HIST_SZ = 256};

	EqualizeHistCalcHist_Invoker(cv::Mat& src, int* histogram, cv::Mutex* histogramLock)
		: src_(src), globalHistogram_(histogram), histogramLock_(histogramLock)
	{ }

	void operator()( const cv::Range& rowRange ) const CV_OVERRIDE
	{
		int localHistogram[HIST_SZ] = {0, };

		const size_t sstep = src_.step;

		int width = src_.cols;
		int height = rowRange.end - rowRange.start;

		if (src_.isContinuous())
		{
			width *= height;
			height = 1;
		}

		for (const uchar* ptr = src_.ptr<uchar>(rowRange.start); height--; ptr += sstep)
		{
			int x = 0;
			for (; x <= width - 4; x += 4)
			{
				int t0 = ptr[x], t1 = ptr[x+1];
				localHistogram[t0]++; localHistogram[t1]++;
				t0 = ptr[x+2]; t1 = ptr[x+3];
				localHistogram[t0]++; localHistogram[t1]++;
			}

			for (; x < width; ++x)
				localHistogram[ptr[x]]++;
		}

		cv::AutoLock lock(*histogramLock_);

		for( int i = 0; i < HIST_SZ; i++ )
			globalHistogram_[i] += localHistogram[i];
	}

	static bool isWorthParallel( const cv::Mat& src )
	{
		return ( src.total() >= 640*480 );
	}

private:
	EqualizeHistCalcHist_Invoker& operator=(const EqualizeHistCalcHist_Invoker&);

	cv::Mat& src_;
	int* globalHistogram_;
	cv::Mutex* histogramLock_;
};

void cv::equalizeHist( InputArray _src, OutputArray _dst )
{
	CV_INSTRUMENT_REGION();

	CV_Assert( _src.type() == CV_8UC1 );

	if (_src.empty())
		return;

	CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
			   ocl_equalizeHist(_src, _dst))

	Mat src = _src.getMat();
	_dst.create( src.size(), src.type() );
	Mat dst = _dst.getMat();

	CV_OVX_RUN(!ovx::skipSmallImages<VX_KERNEL_EQUALIZE_HISTOGRAM>(src.cols, src.rows),
			   openvx_equalize_hist(src, dst))

	Mutex histogramLockInstance;

	const int hist_sz = EqualizeHistCalcHist_Invoker::HIST_SZ;
	int hist[hist_sz] = {0,};
	int lut[hist_sz];

	EqualizeHistCalcHist_Invoker calcBody(src, hist, &histogramLockInstance);
	EqualizeHistLut_Invoker      lutBody(src, dst, lut);
	cv::Range heightRange(0, src.rows);

	if(EqualizeHistCalcHist_Invoker::isWorthParallel(src))
		parallel_for_(heightRange, calcBody);
	else
		calcBody(heightRange);

	int i = 0;
	while (!hist[i]) ++i;

	int total = (int)src.total();
	if (hist[i] == total)
	{
		dst.setTo(i);
		return;
	}

	float scale = (hist_sz - 1.f)/(total - hist[i]);
	int sum = 0;

	for (lut[i++] = 0; i < hist_sz; ++i)
	{
		sum += hist[i];
		lut[i] = saturate_cast<uchar>(sum * scale);
	}

	if(EqualizeHistLut_Invoker::isWorthParallel(src))
		parallel_for_(heightRange, lutBody);
	else
		lutBody(heightRange);
}


*/