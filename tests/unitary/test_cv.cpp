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
        Mat descriptors = Mat::ones(100, 16, CV_8UC1);

        ASSERT_EQ(allDescriptors.rows, 0);
        allDescriptors.push_back(descriptors);
        ASSERT_EQ(allDescriptors.rows, 100);
        allDescriptors.push_back(descriptors);
        ASSERT_EQ(allDescriptors.rows, 200);
}
