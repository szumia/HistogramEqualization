#include <iostream>
#include "Column.h"

string img_path = "../szu6.jpg";
int main() {
    cv::Mat src = cv::imread(img_path);
    Solution mySolution(src);
    mySolution.showOriginImg();
    mySolution.UseLibrary();
    mySolution.DontUseLibrary();



    return 0;
}
