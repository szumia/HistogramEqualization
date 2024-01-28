#include "Column.h"

string savePath1 = "./UseLibrary.jpg";
string savePath2 = "./DontUseLibrary.jpg";
string savePath3=".jpg";

/*
 * 绘制直方图
 * type： 1-bgr 0-gray
 * if_equalized: 0-no 1-yes 是否均衡化
 */
void Solution::drawHist(int type,cv::Mat & img,int if_equalized)
{
    string str="";
    if(if_equalized)
    {
        str += "equalized";
    } else{;}
    int img_width,img_height;
    int hist_witdh,hist_height,hist_x;
    float scale = 1.0;
    img_width = img.cols;
    img_height = img.rows;
    hist_witdh = 800;
    hist_height =300;
    hist_x= 256;        //横轴
    scale = hist_witdh / hist_x;

    float range[]= {0,255};
    const float * Range = {range};



    cv::Mat dark= cv::Mat(hist_height,hist_witdh,CV_8UC3,cv::Scalar(0,0,0));
    cv::Mat b_dark,g_dark,r_dark;
    b_dark = dark.clone();
    g_dark = dark.clone();
    r_dark = dark.clone();


    if(type == 1)           //bgr
    {
        cv::Mat b_hist,g_hist,r_hist;
        vector<cv::Mat> hist_channels;
        cv::split(img,hist_channels);
        cv::calcHist(&hist_channels[0],1,0,cv::Mat(),b_hist,1,&hist_x,&Range,1,0);
        cv::calcHist(&hist_channels[1],1,0,cv::Mat(),g_hist,1,&hist_x,&Range,1,0);
        cv::calcHist(&hist_channels[2],1,0,cv::Mat(),r_hist,1,&hist_x,&Range,1,0);
        for(int i = 1; i < hist_x; i++)
        {
            cv::line(b_dark,cv::Point2f(scale*(i-1),hist_height - b_hist.at<uchar>(i-1)),
                     cv::Point2f(scale*(i),hist_height - b_hist.at<uchar>(i)),cv::Scalar(255,0,0));
            cv::line(g_dark,cv::Point2f(scale*(i-1),hist_height - g_hist.at<uchar>(i-1)),
                     cv::Point2f(scale*(i),hist_height - g_hist.at<uchar>(i)),cv::Scalar(0,255,0));
            cv::line(r_dark,cv::Point2f(scale*(i-1),hist_height - r_hist.at<uchar>(i-1)),
                     cv::Point2f(scale*(i),hist_height - r_hist.at<uchar>(i)),cv::Scalar(0,0,255));
        }
        cv::namedWindow("b Hist "+str,cv::WINDOW_NORMAL);
        cv::imshow("b Hist "+str,b_dark);
        cv::namedWindow("g Hist "+str,cv::WINDOW_NORMAL);
        cv::imshow("g Hist "+str,g_dark);
        cv::namedWindow("r Hist "+str,cv::WINDOW_NORMAL);
        cv::imshow("r Hist "+str,r_dark);

        cv::imwrite("./b Hist "+str+savePath3,b_dark);
        cv::imwrite("./g Hist "+str+savePath3,g_dark);
        cv::imwrite("./r Hist "+str+savePath3,r_dark);


        cv::waitKey(1);

    }
    else
    {
        cv::Mat hist;
        cv::calcHist(&img,1,0,cv::Mat(),hist,1,&hist_x,&Range,1,0);
        cv::normalize(hist,hist,0,img_height,cv::NORM_MINMAX,-1);
        for(int i = 1; i < hist_x; i++)
        {
            cv::line(dark,cv::Point2f(scale*(i-1),hist_height - hist.at<uchar>(i-1)),
                     cv::Point2f(scale*(i),hist_height - hist.at<uchar>(i)),cv::Scalar(0,255,0));
        }
        cv::namedWindow("gray Hist "+str,cv::WINDOW_NORMAL);
        cv::imshow("gray Hist "+ str,dark);
        cv::imwrite("./gray Hist "+str+savePath3,dark);
        cv::waitKey(1);
    }

}




void Solution::showOriginImg()
{
    cv::namedWindow("origin",cv::WINDOW_NORMAL);
    //cv::cvtColor(this->src, this->gray,cv::COLOR_BGR2GRAY);
    cv::imshow("origin", this->src);
    cv::waitKey(1);
}

void Solution::UseLibrary()
{
    cv::Mat b,g,r;
    cv::Mat dst(this->src.rows, this->src.cols,CV_8UC3);
    cv::equalizeHist(this->channels[0],b);
    cv::equalizeHist(this->channels[1],g);
    cv::equalizeHist(this->channels[2],r);
    vector<cv::Mat> new_channels;
    new_channels.push_back(b);
    new_channels.push_back(g);
    new_channels.push_back(r);
    cv::merge(new_channels, dst);
//    cv::equalizeHist(this->gray, this->src_columned);

    cv::imwrite(savePath1,dst);
    cv::namedWindow("useLibrary",cv::WINDOW_NORMAL);
    cv::imshow("useLibrary", dst);
    cv::waitKey(1);

    this->drawHist(1, this->src,0);
    this->drawHist(1,dst,1);

    new_channels.clear();
}

void Solution::DontUseLibrary()
{
    cv::Mat b,g,r;
    cv::Mat dst(this->src.rows, this->src.cols,CV_8UC3);
    uint32_t rows,cols,size;
    rows = this->src.rows;
    cols = this->src.cols;
    size = rows*cols;

    //统计直方图分布
    uint64_t bgr[3][256];
    memset(bgr,0, sizeof(bgr));
    for(unsigned int i = 0; i < rows; i++)
    {
        for (unsigned int j = 0; j < cols; j++)
        {
            bgr[0][channels[0].at<uchar >(i,j)]++;
            bgr[1][channels[1].at<uchar >(i,j)]++;
            bgr[2][channels[2].at<uchar >(i,j)]++;
        }
    }

    //构建映射累计分布(积分）
    double val[3];
    memset(val,0, sizeof(val));
    for(unsigned int i = 0 ; i < 256; i++)
    {
        val[0] += bgr[0][i];
        val[1] += bgr[1][i];
        val[2] += bgr[2][i];

//        bgr[0][i] = val[0];
//        bgr[0][i] = val[1];
//        bgr[0][i] = val[2];

        bgr[0][i] = val[0] * 255 / size;
        bgr[1][i] = val[1] * 255 / size;
        bgr[2][i] = val[2] * 255 / size;
    }

    //归一化直方图
    for(unsigned i = 0; i < rows; i++)
    {
        for(unsigned j = 0; j < cols; j++)
        {
            dst.at<cv::Vec3b>(i,j)[0] = bgr[0][channels[0].at<uchar >(i,j)];
            dst.at<cv::Vec3b>(i,j)[1] = bgr[1][channels[1].at<uchar >(i,j)];
            dst.at<cv::Vec3b>(i,j)[2] = bgr[2][channels[2].at<uchar >(i,j)];
        }
    }

    cv::imwrite(savePath2,dst);
    cv::namedWindow("DontUseLibrary",cv::WINDOW_NORMAL);
    cv::imshow("DontUseLibrary", dst);

    char key = cv::waitKey(0);
    if(key == 27)
    {
        cout<<"ESC"<<endl;
    } else
    {
        return;
    }


}