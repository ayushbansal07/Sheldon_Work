#include<iostream>
#include<fstream>
#include<vector>
#include<stack>
#include<algorithm>
#include<math.h>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv/cv.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <tesseract/baseapi.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;

int main()

{Mat frame = imread( "snap4.jpg" );

 int headr=230,headg=230,headb=90,tailr=88,tailg=130,tailb=190,count_head,count_tail,thresh=40;

     long long int xh=0, yh=0, xt=0, yt=0; count_head=1; count_tail=1;
        //cap>>frame;
        if(frame.data)
        {
            //FINDING COORDINATES OF HEAD AND TAIL
            for(int i=0;i<frame.rows;i++)
            {
                for(int j=0;j<frame.cols;j++)
                {
                    if(frame.at<Vec3b>(i,j)[0]>headb-thresh && frame.at<Vec3b>(i,j)[0]<headb+thresh && frame.at<Vec3b>(i,j)[1]>headg-thresh && frame.at<Vec3b>(i,j)[1]<headg+thresh && frame.at<Vec3b>(i,j)[2]>headr-thresh && frame.at<Vec3b>(i,j)[2]<headr+thresh)
                    {
                        xh+=i;
                        yh+=j;
                        count_head++;
                    }
                    if(frame.at<Vec3b>(i,j)[0]>tailb-thresh && frame.at<Vec3b>(i,j)[0]<tailb+thresh && frame.at<Vec3b>(i,j)[1]>tailg-thresh && frame.at<Vec3b>(i,j)[1]<tailg+thresh && frame.at<Vec3b>(i,j)[2]>tailr-thresh && frame.at<Vec3b>(i,j)[2]<tailr+thresh)
                    {
                        xt+=i;
                        yt+=j;
                        count_tail++;
                    }

                }
            }
            }

            //cout<<count_head<<"   "<<count_tail<<endl;

            xh=xh/count_head;
            yh=yh/count_head;
            xt=xt/count_tail;
            yt=yt/count_tail;
            imshow("capture", frame);
            cvWaitKey();
int m=frame.rows; int n=frame.cols;
        cout<<xh<<endl<<yh<<endl<<xt<<endl<<yt<<endl<<m<<endl<<n<<endl;
        return 0;
        }
