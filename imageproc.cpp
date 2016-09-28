 #include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <tesseract/baseapi.h>
#include <iostream>


using namespace std;
using namespace cv;


int main( )
{
  /// Load an image
  img = imread("snap2.jpg", CV_LOAD_IMAGE_COLOR);

  if( !src.data )
  { return -1; }

cv::Size size(3,3);
  cv::GaussianBlur(img,img,size,0);
   adaptiveThreshold(img, img,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,75,10);
  cv::bitwise_not(img, img);
  imshow("procim", img);

  return 0;
  }
