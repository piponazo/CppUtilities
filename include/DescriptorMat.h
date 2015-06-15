#pragma once

/// This class will have the same API than cv::Mat
class DescriptorMat
{
	public:
    	DescriptorMat(int rows, int cols, int type);
    	DescriptorMat(int rows, int cols, int type, const float& s);
    	DescriptorMat(const DescriptorMat& m);

		int rows;
		int cols;

};
