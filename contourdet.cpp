#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <tesseract/baseapi.h>

using namespace cv;
using namespace std;
Mat src; Mat src_gray;
//int thresh = 130;
int max_thresh = 255;
RNG rng(12345);
tesseract::TessBaseAPI tess;
/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
{

  //TessBaseAPI.setVariable("tessedit_char_whitelist", "123456789+-/x");
    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    //tess.SetVariable("tessedit_char_whitelist", "123456789+-/x");
    tess.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
  /// Load source image and convert it to gray
  src = imread( "snap2.jpg" );

  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );
cv::threshold(src_gray, src_gray, 220, 255,0);
//REMOVE NOISE
/*for(int i=1;i<src_gray.rows-1;i++)
{
    for(int j=1;j<src_gray.cols-1;j++)
    {
        count_noise=0;
        if(src_gray.at<uchar>(i,j)==255)
        {
            for(int i1=i-1;i1<=i+1;i1++)
            {
                for(int j1=j-1;j1<=j+1;j1++)
                {
                    if(src_gray.at<uchar>(i1,j1)==0)
                        count_noise++;
                }
            }
            if(count_noise>4)
                src_gray.at<uchar>(i,j)=0;
        }
    }
}*/
  /// Create Window
  char* source_window = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  imshow( source_window, src );
  imshow("binary", src_gray);


  //createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );


  waitKey(0);
  return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
int flag;
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using Threshold
  threshold( src_gray, threshold_output, 220, 255, THRESH_BINARY );
  /// Find contours
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Approximate contours to polygons + get bounding rects and circles
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect;
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );

  for( int i = 0; i < contours.size(); i++ )
     { approxPolyDP( Mat(contours[i]), contours_poly[i], 1, true );
     double a = contourArea(contours[i]);

     if (a >= 100 && a <= 1000)

       {boundRect.push_back(boundingRect( Mat(contours_poly[i])) );
       cout<<a<<endl;}
      // minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }




  // Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
       //circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
     }


    //cv::cvtColor(src_gray(boundRect[i]),src_gray(boundRect[i]),CV_RGB2GRAY);

      //cv::threshold(src_gray, src_gray, 220, 255,0);
      /*for(int i=0;i<src_gray.rows;i++)
     {
        for(int j=0;j<src_gray.cols;j++)
        {
            flag=0;
            for(int k=0;k<contours.size();k++)
            {
                if(i<boundRect[k].tl().x || j<boundRect[k].tl().y || i>boundRect[k].br().x || j>boundRect[k].br().y)
                    flag++;
            }
            if (flag==contours.size())
                src_gray.at<uchar>(i,j)=255;
        }
     }*/
      imshow("binary", src_gray);



  /// Show in a window


 for(int i = 0; i < boundRect.size() ; i++) {
    imshow("samole" + i, src_gray(boundRect[i]));
Mat timg = src_gray(boundRect[i]);

   tess.SetImage((uchar*)timg.data, timg.size().width, timg.size().height, timg.channels(), timg.step1());
    //tess.Recognize(0);

    // Get the text
    char* out = tess.GetUTF8Text();


    std::cout << out << std::endl;
    }




namedWindow( "Contours", CV_WINDOW_NORMAL);
  imshow( "Contours", drawing );

}
