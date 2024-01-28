#ifndef LZT_HW02_HISTOGRAMEQUALIZATION_COLUMN_H
#define LZT_HW02_HISTOGRAMEQUALIZATION_COLUMN_H

#include <iostream>
#include <vector>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
using namespace std;


class Solution
{
public:
    vector<cv::Mat> channels;

    cv::Mat src, gray;

    Solution(cv::Mat img)
    {
        this->src = img.clone();
        if(img.channels() == 3 )
        {
            cv::split(this->src,channels);
        } else{;}
    }
    void showOriginImg();
    void UseLibrary();
    void DontUseLibrary();
    void drawHist(int type,cv::Mat & img,int if_equalized);

};


#endif //LZT_HW02_HISTOGRAMEQUALIZATION_COLUMN_H
