#include "pch.h"
#include "Histogram.h"

void Histogram::equalizeHistogram(const cv::Mat &_src, cv::Mat &_dst)
{
	// null check
	if (_src.empty())
		return;

	// type check
	auto type = _src.type();
	if (type == 0)
	{
		//equalizeHist8Bit(_src, _dst);
		cv::equalizeHist(_src, _dst);
	}
	else if (type == 2)
	{
		equalizeHist16BitArray(_src, _dst);
		//equalizeHist16BitVector(_src, _dst);
	}
	else
	{
		return;
	}
}

void Histogram::equalizeHist8Bit(const cv::Mat &_src, cv::Mat &_dst)
{
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
	double alpha = double(intSize - 1) / size;

	// Calculate the probability of each intensity
	double PrRk[intSize];
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
	double PsSk[intSize];
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
			_dst.at<uchar>(y, x) = cv::saturate_cast<uchar>(Sk[_src.at<uchar>(y, x)]);
}

void Histogram::equalizeHist16BitVector(const cv::Mat &_src, cv::Mat &_dst)
{
	const int intSize = 65536;
	// Generate the histogram
	std::vector<int> histogram;

	// initialize all intensity values to 0
	for (int i = 0; i < intSize; i++)
	{
		histogram.push_back(0);
	}

	// calculate the no of pixels for each intensity values
	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
			histogram[(int)_src.at<unsigned short int>(y, x)]++;

	// Caluculate the size of image
	int size = _src.rows * _src.cols;
	double alpha = double(intSize - 1) / size;

	// Calculate the probability of each intensity
	std::vector<double> PrRk;
	for (int i = 0; i < intSize; i++)
	{
		PrRk.push_back((double)histogram[i] / size);
	}

	// Generate cumulative frequency histogram
	std::vector<int> cumhistogram;
	cumhistogram.push_back(histogram[0]);

	for (int i = 1; i < intSize; i++)
	{
		cumhistogram.push_back(histogram[i] + cumhistogram[i - 1]);
	}

	//Scale the histogram
	std::vector<int> Sk;
	for (int i = 0; i < intSize; i++)
	{
		Sk.push_back(cvRound((double)cumhistogram[i] * alpha));
	}

	// Generate the equlized histogram
	std::vector<double> PsSk;
	for (int i = 0; i < intSize; i++)
	{
		PsSk.push_back(0);
	}

	for (int i = 0; i < intSize; i++)
	{
		PsSk[Sk[i]] += PrRk[i];
	}

	std::vector<int> final;
	for (int i = 0; i < intSize; i++)
	{
		final.push_back(cvRound(PsSk[i] * (intSize - 1)));
	}

	// Generate the equlized image
	_dst = _src.clone();

	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
			_dst.at<unsigned short int>(y, x) = cv::saturate_cast<unsigned short int>(Sk[_src.at<unsigned short int>(y, x)]);
}

void Histogram::equalizeHist16BitArray(const cv::Mat &_src, cv::Mat &_dst)
{
	const int intSize = 65536;
	// Generate the histogram
	int *histogram = new int[intSize] {};

	// calculate the no of pixels for each intensity values
	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
			histogram[(int)_src.at<unsigned short int>(y, x)]++;

	// Caluculate the size of image
	int size = _src.rows * _src.cols;
	double alpha = double(intSize - 1) / size;

	// Calculate the probability of each intensity
	double *PrRk = new double[intSize];

	for (int i = 0; i < intSize; i++)
	{
		PrRk[i] = (double)histogram[i] / size;
	}

	// Generate cumulative frequency histogram
	int *cumhistogram = new int[intSize];
	cumhistogram[0] = histogram[0];
	for (int i = 1; i < intSize; i++)
	{
		cumhistogram[i] = histogram[i] + cumhistogram[i - 1];
	}

	//Scale the histogram
	int *Sk = new int[intSize];
	for (int i = 0; i < intSize; i++)
	{
		Sk[i] = cvRound((double)cumhistogram[i] * alpha);
	}

	// Generate the equlized histogram
	double *PsSk = new double[intSize] {};

	for (int i = 0; i < intSize; i++)
	{
		PsSk[Sk[i]] += PrRk[i];
	}

	int *final = new int[intSize];
	for (int i = 0; i < intSize; i++)
		final[i] = cvRound(PsSk[i] * (intSize - 1));

	// Generate the equlized image
	_dst = _src.clone();

	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
			_dst.at<unsigned short int>(y, x) = cv::saturate_cast<unsigned short int>(Sk[_src.at<unsigned short int>(y, x)]);
}
