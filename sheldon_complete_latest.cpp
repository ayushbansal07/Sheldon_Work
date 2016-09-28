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

Mat src; Mat src_gray;
//int thresh = 130;
int max_thresh = 255;
RNG rng(12345);
tesseract::TessBaseAPI tess;

vector<Rect> boundRect;
/// Function header
void thresh_callback(int,void*);

int fd;

int priority(char ch);
float angle (int, int, int, int);
float calslope(int, int, int, int);

int count_char;

struct values{
char ctr;
int x;
int y;
//int x2;
//int y2;
};

vector<values> data;
vector<pair<char, pair<int, int> > > expr;
values temp_data;

void sendCommand(const char* command) {
	write(fd, command, 1);
	cout << "sending " << command[0] << endl;
}

void predictChar(Rect r, vector<values>& data) {
    Mat timg = src_gray(r);

   tess.SetImage((uchar*)timg.data, timg.size().width, timg.size().height, timg.channels(), timg.step1());
    //tess.Recognize(0);

    // Get the text
    char* out = tess.GetUTF8Text();
    temp_data.ctr=out[0];
                                                                                                                                                                                                if(temp_data.ctr==46)
                                                                                                                                                                                                    temp_data.ctr=54;
                                                                                                                                                                                                if(temp_data.ctr==85)
                                                                                                                                                                                                    temp_data.ctr=56;
                                                                                                                                                                                                //if(temp_data.ctr==72)
                                                                                                                                                                                                    //temp_data.ctr=54;
                                                                                                                                                                                                if(temp_data.ctr==95)
                                                                                                                                                                                                    temp_data.ctr=45;
                                                                                                                                                                                                if(temp_data.ctr==88)
                                                                                                                                                                                                    temp_data.ctr=120;
                                                                                                                                                                                                if(temp_data.ctr==32)
                                                                                                                                                                                                    temp_data.ctr=45;
                                                                                                                                                                                                if(temp_data.ctr==58)
                                                                                                                                                                                                    temp_data.ctr=56;
    temp_data.x=((r.tl().x)+(r.br().x))/2;
    temp_data.y=((r.tl().y)+(r.br().y))/2;
    data.push_back(temp_data);

    cout<<temp_data.ctr<<"  "<<temp_data.x<<"   "<<temp_data.y<<endl;
    cout << data.size() << " current size" << endl;
    expr.push_back(make_pair(temp_data.ctr, make_pair(temp_data.x, temp_data.y)));
}

int main()
{

    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);

    int useless,value,maxi=-9999,mini=9999,a[20],b[20],x1,y1,x2,y2,coordinates[20][2];
    values temp;
    char expression[20],max_expression[20],min_expression[20];
    char postfix[20];
    vector<values> data,nos,operators,nos_temp,operators_temp,max_nos,min_nos,max_operators,min_operators;
    //fstream fobj;
    stack<char> s;
    stack<int> s2;


    //TessBaseAPI.setVariable("tessedit_char_whitelist", "123456789+-/x");
    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    //tess.SetVariable("tessedit_char_whitelist", "123456789+-/x");
    tess.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    /// Load source image and convert it to gray
    //src = imread( "snap4.jpg" );

     VideoCapture cap(1);
     if(!cap.isOpened())  // check if we succeeded
        return -1;
     int check = 0;
     while(check <= 50)
     {
     cap>>src;
     //printf("\n%d%d\n\n\n",src.rows,src.cols);
     check++;
     }
     //cvWaitKey();
     cvNamedWindow("source",CV_WINDOW_AUTOSIZE);
     imshow("source",src);
     cvWaitKey();
     /*src = imread( "snap4.jpg" );
     cvNamedWindow("source2",CV_WINDOW_AUTOSIZE);
     imshow("source2",src);*/
   /* int stopr=241,stopg=64,stopb=48,count_stop=0,xs=0,ys=0, thresh=20;
    for(int i=0;i<src.rows;i++)
    {
        for(int j=0;j<src.cols;j++)
        {
            if(src.at<Vec3b>(i,j)[0]>stopb-thresh && src.at<Vec3b>(i,j)[0]<stopb+thresh && src.at<Vec3b>(i,j)[1]>stopg-thresh && src.at<Vec3b>(i,j)[1]<stopg+thresh && src.at<Vec3b>(i,j)[2]>stopr-thresh && src.at<Vec3b>(i,j)[2]<stopr+thresh)
            {
                xs+=i;
                ys+=j;
                count_stop++;
            }
        }
    }
    xs=xs/count_stop;
    ys=ys/count_stop;
    coordinates[count_char][0]=xs;
    coordinates[count_char][1]=ys;
*/
    /// Convert image to gray and blur it
    cvtColor( src, src_gray, CV_BGR2GRAY );
    imshow("Graysacle",src_gray);
    blur( src_gray, src_gray, Size(3,3) );
    imshow("Grayscale after blur",src_gray);
    cv::threshold(src_gray, src_gray, 230, 255,0);
    imshow("After threshold",src_gray);
    cvWaitKey();
   //createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
   thresh_callback(0,0);


    count_char=expr.size();
    for (int i = 0; i < expr.size() ; i++) {
        cout << expr[i].first << " -> " << expr[i].second.first << ", " << expr[i].second.second << endl;
    }
    for(int i=0;i<count_char;i++)
    {
        temp_data.ctr=expr[i].first;
        temp_data.x=expr[i].second.first;
        temp_data.y=expr[i].second.second;
        data.push_back(temp_data);
    }
    cout<<data.size()<<"size of data from main"<<endl;

    for(int i=0;i<(count_char+1)/2;i++)
    {
        a[i]=i;
    }
    for(int i=0;i<(count_char-1)/2;i++)
    {
        b[i]=i;
    }

    //SEGREGATION to "nos" and "operators"
    for(int i=0;i<count_char;i++)
    {
        if(data[i].ctr==85)
            data[i].ctr=56;
        //if(data[i].ctr==72)
          //  data[i].ctr=54;
        if(data[i].ctr>=49 && data[i].ctr<=57)
        {
            nos_temp.push_back(data[i]);
        }
        else
        {
            if(data[i].ctr==95)
                data[i].ctr=45;
            if(data[i].ctr==88)
                data[i].ctr=120;
            operators_temp.push_back(data[i]);
        }
    }

    do
    {
    //PERMUTATION in OPERATORS from OPERATORS_TEMP
    for(int i=0;i<(count_char-1)/2;i++)
    {
        operators.push_back(operators_temp[b[i]]);
    }

    do
    {
    //PERMUTATION in NOS from NOS_TEMP
    for(int i=0;i<(count_char+1)/2;i++)
    {
        nos.push_back(nos_temp[a[i]]);
    }

    //MAKING of the EXPRESSION in INFIX
    for(int i=0;i<count_char-1;i++)
    {
        expression[i]=nos[i/2].ctr;
        expression[++i]=operators[i/2].ctr;
    }
    expression[count_char-1]=nos[(count_char-1)/2].ctr;
    expression[count_char]='\0';
    //cout<<expression<<endl;

    //CONVERTING TO POSTFIX
    int j=0;
    for(int i=0;i<count_char;i++)
    {
        if(priority(expression[i])==0)
        {
            postfix[j]=expression[i];
            j++;
        }
        else
        {
            if(s.empty())
            {
                s.push(expression[i]);
            }
            else
            {
                while(!s.empty() && priority(expression[i])<=priority(s.top()))
                {
                    postfix[j]=s.top();
                    s.pop();
                    j++;
                }
                s.push(expression[i]);
            }
        }
    }
    while(!s.empty())
    {
        postfix[j]=s.top();
        s.pop();
        j++;
    }
    postfix[j]='\0';

    //cout<<postfix<<endl;
    //EVALUATING POSTFIX
    for(int i=0;i<count_char;i++)
    {
        if(postfix[i]>=49 && postfix[i]<=57)
        {
            int num = postfix[i]-'0';
            s2.push(num);
        }
        else
        {
            int a = s2.top();
            s2.pop();
            int b = s2.top();
            s2.pop();
            switch(postfix[i])
            {
                case '+': s2.push(a+b);
                        break;
                case '-': s2.push(b-a);
                        break;
                case 'x': s2.push(a*b);
                        break;
                case '/': s2.push(b/a);
                        break;
            }

        }

    }
    value=s2.top();
    s2.pop();
    //cout<<value<<endl;
    if(value>maxi)
    {
        maxi=value;
        max_nos=nos;
        max_operators=operators;
        for(int i=0;i<count_char+1;i++)
        {
            max_expression[i]=expression[i];
        }

    }
    if(value<mini)
    {
        mini=value;
        min_nos=nos;
        min_operators=operators;
        for(int i=0;i<count_char+1;i++)
        {
            min_expression[i]=expression[i];
        }

    }
    nos.clear();
    operators.clear();
    }while(std::next_permutation(a,a+(count_char+1)/2));
    }while(std::next_permutation(b,b+(count_char-1)/2));

    //cout<<maxi<<"     "<<mini<<endl;
    cout<<max_expression<<endl;
    //PRINTING THE COORDINATES
    for(int i=0;i<count_char;i++)
    {
        if(i%2==0)
            cout<<max_nos[i/2].x<<"    "<<max_nos[i/2].y<<endl;
        else
            cout<<max_operators[(i-1)/2].x<<"     "<<max_operators[(i-1)/2].y<<endl;
    }

    for(int i=0;i<count_char;i++)
    {
        if(i%2==0)
        {
            coordinates[i][0]=max_nos[i/2].x;
            coordinates[i][1]=max_nos[i/2].y;
        }
        else
        {
            coordinates[i][0]=max_operators[(i-1)/2].x;
            coordinates[i][1]=max_operators[(i-1)/2].y;
        }
    }


   //VIDEO CAPTURING
    Mat frame;
    int headr=230,headg=230,headb=90,tailr=88,tailg=130,tailb=190,thresh=40;

    int t=0;
    //cap>>frame;
     //VideoCapture cap(0);
    cvWaitKey();
    while(1)
    {
     long long int xh=0,yh=0,xt=0,yt=0,count_head=1,count_tail=1;
        cap>>frame;
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

            //cout<<count_head<<"   "<<count_tail<<endl;

            xh=xh/count_head;
            yh=yh/count_head;
            xt=xt/count_tail;
            yt=yt/count_tail;


        //yh=frame.rows-yh;
        //yt=frame.rows-yt;

        int xbot = (xh+xt)/2;
        int ybot = (yh+yt)/2;

        int side_sq=40;    //It is length of square/2
        if(xbot<=coordinates[t][0]+side_sq && xbot>=coordinates[t][0]-side_sq && ybot<=coordinates[t][1]+side_sq && ybot>=coordinates[t][1]-side_sq)
        {
            //CODE TO BLINK LED
            sendCommand("l");
            sendCommand("b");
            t++;
        }
        if(t==count_char+1)
            break;


        //add stop condition

        float diff = angle(xh,yh,xt,yt)-angle(xbot, ybot,coordinates[t][0],coordinates[t][1]);
        cout<<diff<<endl;
        if(coordinates[t][0]>=xbot)
        {
            if (xh>=xt)
            {
                if (diff>=-7 && diff<=7)
                    sendCommand("w");
                else if (diff>0)
                    sendCommand("d");
                else if (diff<0)
                    sendCommand("a");
            }
            else if (xh<xt)
            {
                if (diff>=-7 && diff<=7)
                    sendCommand("s");
                else if (diff>0)
                    sendCommand("d");  //Shouldn't it be a and d (reverse)
                else if(diff<0)
                    sendCommand("a");
            }

        }

        else if (coordinates[t][0]<xbot)
        {
            if (xh>xt)
            {
                if (diff>=-7 && diff<=7)
                    sendCommand("s");
                else if (diff>0)
                    sendCommand("d");
                else if(diff<0)
                    sendCommand("a");
            }

            else if (xh<=xt)
            {
                if (diff>=-7&& diff<=7)
                    sendCommand("w");
                else if (diff>0)
                    sendCommand("d");   //Shouldn't it be reversed
                else if(diff<0)
                    sendCommand("a");
            }
        }

        }

        //LOCOMOTION COMMANDS COMPLETED




    sendCommand("b");



}

int priority(char ch)
{
    if(ch=='/'||ch=='x')
        return 2;
    else if(ch=='+'|| ch=='-')
        return 1;
    else
        return 0;
}

float angle (int a, int b, int c, int d)
{
    float m,atandeg;
    m=calslope(a,b,c,d);
    atandeg=atan(m)*180/3.14;
    if (m>0)
    return atandeg;
    else return 180-atandeg;
}

float calslope(int a, int b, int c, int d)
{  float m;
    m=(d-b)/(c-a);
    return m;
}

void thresh_callback(int , void*)
{

  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using Threshold
  threshold( src_gray, threshold_output, 230, 255, THRESH_BINARY );
  /// Find contours
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Approximate contours to polygons + get bounding rects and circles
  vector<vector<Point> > contours_poly( contours.size() );

  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );

  for( int i = 0; i < contours.size(); i++ )
     { approxPolyDP( Mat(contours[i]), contours_poly[i], 1, true );
     double a = contourArea(contours[i]);

     if (a >= 30 && a <= 550)

       {boundRect.push_back(boundingRect( Mat(contours_poly[i])) );
       cout<<a<<endl;}
      // minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }
     cout<<boundRect.size()<<endl;




  // Draw polygonal contour + bonding rects + circles
 /* Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
       //circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
     }*/

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
    //  imshow("binary", src_gray);



  /// Show in a window

cout << boundRect.size() << " < -- size" << endl;
 for(int i = 0; i < boundRect.size() ; i++) {

    predictChar(boundRect[i], data);

/*namedWindow( "Contours", CV_WINDOW_NORMAL);
  imshow( "Contours", drawing );*/


}
}
