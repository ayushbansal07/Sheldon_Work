#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main()
{


    // Load image
    cv::Mat im = cv::imread("image1.png");
    if (im.empty())
    {
        std::cout << "Cannot open source image!" << std::endl;
        return -1;
    }

    cv::Mat gray, dst;
    cv::cvtColor(im, gray, CV_BGR2GRAY);
     cv::threshold( gray, dst, 130, 255,1 );
     namedWindow("revbin", CV_WINDOW_AUTOSIZE);
      cv::imshow("revbin", gray);
    // ...other image pre-processing here...

    // Pass it to Tesseract API
    tesseract::TessBaseAPI tess;
    baseAPI.setVariable("tessedit_char_whitelist", "123456789+-/x");

    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    tess.SetImage((uchar*)dst.data, dst.cols, dst.rows, 1, dst.cols);

    // Get the text
    char* out = tess.GetUTF8Text();
    std::cout << out << std::endl;

    return 0;
}
