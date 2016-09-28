/////loco implementation


#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <math.h>
#include<stdio.h>

using namespace std;
int fd;

float angle (int, int, int, int);
float calslope(int, int, int, int);
void sendCommand(const char*);

void sendCommand(const char* command) {
    write(fd, command, 1);
    cout << "sending " << command << endl;
}

int main(int argc, char const *argv[])
{
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);  //Opening device file
    printf("fd opened as %i\n", fd);
       usleep(2500000);
       sendCommand("L");
    return 0;

float x,y,a,b,xh,yh,xt,yt;
char arr[1];
while(1)
{
    float diff,ang;
    diff=angle(xh, yh, xt, yt)-angle(x, y, a, b);
    ang=angle(x,y,a,b);
    if(b>=y)
    {
        if (yh>=yt)
        {

            if (diff>=-5 && diff<=5)
                arr[0]='w';//sendCommand('w');
            else if (angle(xh, yh, xt, yt)-angle(x, y, a, b)>0)
               arr[0]='d';// sendCommand('d');
            else if (angle(xh, yh, xt, yt)-angle(x, y, a, b)<0)
               arr[0]='a';// sendCommand('a');
        }

        else if (yh<yt)
        {
            if (diff>=-5 && diff<=5)
               arr[0]='s';// sendCommand('s');
            else if (diff>0)
               arr[0]='d';// sendCommand('d');
            else if(diff<0)
               arr[0]='a';// sendCommand('a');
        }
    }

    else if(b<=y)
    {
        if (yh>yt)
        {
            if (diff>=-5 && diff<=5)
                arr[0]='s';//  sendCommand('s');
            else if (diff>0)
                arr[0]='d';//  sendCommand('d');
            else if(diff<0)
                arr[0]='a';//  sendCommand('a');
        }
        else if (yh<=yt)
        {
            if (diff>=-5 && diff<=5)
                arr[0]='w';// sendCommand('w');
            else if (diff>0)
                arr[0]='d';//  sendCommand('d');
            else if(diff<0)
                arr[0]='a';//  sendCommand('a');
        }
    }
    sendCommand(arr);
}
}

/////loco algo


//slope
float calslope(int a, int b, int c, int d)
{  float m;
m=(d-b)/(c-a);
return m;
}
//angle between lines
float angle (int a, int b, int c, int d)
{
float m,atandeg; m=calslope(a,b,c,d);
atandeg=atan(m)*180/3.14;
if (m>0)
return atandeg;
else return 180-atandeg;
}















