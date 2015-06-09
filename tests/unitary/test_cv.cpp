#include "cv/core/core.hpp"

#include <gtest/gtest.h>

using namespace cv;

TEST(MatTest, shouldCreateAndDestroyInstancesCorrectly_onlyUsingConstructors)
{
        Mat img;
        Mat imgRowsColsType  (480, 640, CV_32FC1);
        Mat imgRowsColsType2 (480, 640, CV_8UC1);
        ASSERT_TRUE(img.empty());
        ASSERT_FALSE(imgRowsColsType.empty());
        ASSERT_FALSE(imgRowsColsType2.empty());
}

TEST(MatTest, shouldCreateUsingCreateFunction)
{
        Mat img;
        img.create(480, 640, CV_8UC1);
        ASSERT_EQ(img.depth(), CV_8U);

        img.create(480, 640, CV_32FC1);
        ASSERT_EQ(img.depth(), CV_32F);
}

TEST(MatTest, shouldAccessData)
{
        Mat imgRowsColsBinary (480, 640, CV_8UC1);
        Mat imgRowsColsFloat  (480, 640, CV_32FC1);

        ASSERT_FALSE(imgRowsColsBinary.empty());
        ASSERT_FALSE(imgRowsColsFloat.empty());

        uchar *dataBinary = imgRowsColsBinary.ptr(0);
        float *dataFloat =  imgRowsColsFloat.ptr<float>(0);

        Mat imgRowsColsBinary2 (480, 640, CV_8UC1, dataBinary);
        Mat imgRowsColsFloat2  (480, 640, CV_32FC1, dataFloat);

        ASSERT_FALSE(imgRowsColsBinary2.empty());
        ASSERT_FALSE(imgRowsColsFloat2.empty());
}

TEST(MatTest, shouldAppendRows)
{
        Mat allDescriptors;
        Mat descriptors(100, 16, CV_8UC1);

        ASSERT_EQ(allDescriptors.rows, 0);
        allDescriptors.push_back(descriptors);
        ASSERT_EQ(allDescriptors.rows, 100);
        allDescriptors.push_back(descriptors);
        ASSERT_EQ(allDescriptors.rows, 200);
}

TEST(MatTest, shouldCloneMats)
{
        Mat source(100, 16, CV_8UC1);
        Mat target = source.clone();
        Mat target2 (source);

        ASSERT_EQ(source.size(), target.size());
        ASSERT_EQ(source.size(), target2.size());
}

TEST(MatTest, shouldAssignScalarToMat)
{
        Mat source(100, 16, CV_8UC1);
        source = Scalar(5);

        ASSERT_EQ(source.at<uchar>(5,5), 5);
}

TEST(MatTest, shouldFindMinMaxElements)
{
        Mat source(100, 16, CV_8UC1, Scalar(2));

        source.at<uchar>(0,0) = 0;
        source.at<uchar>(99,2) = 3;

        Point minPoint, maxPoint;
        double minVal, maxVal;

        cv::minMaxLoc(source, &minVal, &maxVal, &minPoint, &maxPoint);
        ASSERT_DOUBLE_EQ(minVal, 0.);
        ASSERT_DOUBLE_EQ(maxVal, 3.);
        ASSERT_EQ(minPoint, Point(0,0));
        ASSERT_EQ(maxPoint, Point(2,99));
}

TEST(MatTest, shouldReleaseMatrix)
{
        Mat source(100, 16, CV_8UC1, Scalar(2));

        source.at<uchar>(0,0) = 0;
        source.at<uchar>(99,2) = 3;

        ASSERT_FALSE(source.empty());
        source.release();
        ASSERT_TRUE(source.empty());
}
