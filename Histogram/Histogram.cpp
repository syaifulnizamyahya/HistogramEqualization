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
		equalizeHist16BitOpenCV(_src, _dst);
		//equalizeHist16BitArray(_src, _dst);
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
	std::vector<int> histogram(intSize, 0);

	// initialize all intensity values to 0
	//for (int i = 0; i < intSize; i++)
	//{
	//	histogram.push_back(0);
	//}

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

void Histogram::equalizeHist16BitOpenCV(const cv::Mat &_src, cv::Mat &_dst)
{
	const int hist_sz = 65536;
	//int* pdata, int width, int height, int max_val = 255;
	//int total = width * height;
	//int total = _src.rows * _src.cols;
	//int total = (int)_src.total();
	//int intSize = max_val + 1;

	_dst = _src.clone();

	// Compute histogram
	std::vector<int> hist(hist_sz, 0);
	std::vector<int> lut(hist_sz, 0);
	//for (int i = 0; i < total; ++i) {
	//	hist[pdata[i]]++;
	//}
	// can be parallel
	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
			hist[(int)_src.at<unsigned short int>(y, x)]++;

	// Build LUT from cumulative histrogram

	// Find first non-zero bin
	int i = 0;
	while (!hist[i]) ++i;

	int total = (int)_src.total();
	if (hist[i] == total) 
	{
		//for (int j = 0; j < total; ++j) {
		//	pdata[j] = i;
		//}
		_dst.setTo(i);
		return;
	}

	// Initialize lut
	//std::vector<int> lut(hist_sz, 0);
	// Compute scale
	float scale = (hist_sz - 1.f) / (total - hist[i]);
	int sum = 0;

	i++;

	for (; i < hist.size(); ++i) 
	{
		sum += hist[i];
		// the value is saturated in range [0, max_val]
		//lut[i] = std::max(0, std::min(int(round(sum * scale)), hist_sz-1));
		lut[i] = cv::saturate_cast<ushort>(sum * scale);
	}

	// Apply equalization
	// can be parallel
	//for (int i = 0; i < total; ++i) {
	//	pdata[i] = lut[pdata[i]];
	//}
	for (int y = 0; y < _src.rows; y++)
		for (int x = 0; x < _src.cols; x++)
		{
			_dst.at<unsigned short int>(y, x) = lut[(int)_src.at<unsigned short int>(y, x)];
		}
	//		_dst.at<unsigned short int>(y, x) = cv::saturate_cast<unsigned short int>(Sk[_src.at<unsigned short int>(y, x)]);

	//for (int y = 0; y < _src.rows; y++)
	//	for (int x = 0; x < _src.cols; x++)
	//		_dst.at<unsigned short int>(y, x) = cv::saturate_cast<unsigned short int>(Sk[_src.at<unsigned short int>(y, x)]);

}

//void equalizeHistogram(int* pdata, int width, int height, int max_val = 255)
//{
//	int total = width * height;
//	int n_bins = max_val + 1;
//
//	// Compute histogram
//	vector<int> hist(n_bins, 0);
//	for (int i = 0; i < total; ++i) {
//		hist[pdata[i]]++;
//	}
//
//	// Build LUT from cumulative histrogram
//
//	// Find first non-zero bin
//	int i = 0;
//	while (!hist[i]) ++i;
//
//	if (hist[i] == total) {
//		for (int j = 0; j < total; ++j) {
//			pdata[j] = i;
//		}
//		return;
//	}
//
//	// Compute scale
//	float scale = (n_bins - 1.f) / (total - hist[i]);
//
//	// Initialize lut
//	vector<int> lut(n_bins, 0);
//	i++;
//
//	int sum = 0;
//	for (; i < hist.size(); ++i) {
//		sum += hist[i];
//		// the value is saturated in range [0, max_val]
//		lut[i] = max(0, min(int(round(sum * scale)), max_val));
//	}
//
//	// Apply equalization
//	for (int i = 0; i < total; ++i) {
//		pdata[i] = lut[pdata[i]];
//	}
//}
