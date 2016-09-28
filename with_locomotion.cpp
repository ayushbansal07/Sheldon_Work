
#include<fstream>
#include<vector>
#include<stack>
#include<algorithm>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <math.h>
#include <stdio.h>
#include <iostream>

int priority(char ch);
int fd;

float angle (int, int, int, int);
float calslope(int, int, int, int);
void sendCommand(const char*);

void sendCommand(const char* command) {
    write(fd, command, 1);
    //cout<<"sending "<<command<<endl;
    printf("sending %c\n",command[0]);
}

using namespace std;
struct values{
char ctr;
int x;
int y;
//int x2;
//int y2;
};

int main(int argc, char const *argv[])
{
    int count_char=0,useless,value,maxi=-9999,mini=9999,a[20],b[20],x1,y1,x2,y2,coordinates[20][2];
    values temp;
    char expression[20],max_expression[20],min_expression[20];
    char postfix[20];
    vector<values> data,nos,operators,nos_temp,operators_temp,max_nos,min_nos,max_operators,min_operators;
    fstream fobj;
    stack<char> s;
    stack<int> s2;

    //FILE HANDLING-storing to "data"
    fobj.open("ou2.box",ios::in);
    while(!fobj.eof())
    {
        fobj>>temp.ctr>>x1>>y1>>x2>>y2>>useless;
        temp.x=(x1+x2)/2;
        temp.y=(y1+y2)/2;
        data.push_back(temp);
        count_char++;
    }

    count_char--;
    /*for(int j=0;j<count_char;j++)
    {
        cout<<data[j].ctr<<" "<<data[j].x1<<" "<<data[j].y1<<" "<<data[j].x2<<" "<<data[j].y2<<endl;
    }*/

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
    /*for(int i=0;i<count_char;i++)
    {
        if(i%2==0)
            cout<<max_nos[i/2].x<<"    "<<max_nos[i/2].y<<endl;
        else
            cout<<max_operators[(i-1)/2].x<<"     "<<max_operators[(i-1)/2].y<<endl;
    }*/

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

    //LOCOMOTION
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);  //Opening device file
    printf("fd opened as %i\n", fd);
       usleep(2500000);
       sendCommand("w");
    return 0;

float x,y,la,lb,xh,yh,xt,yt;
/*la=coordinates[0][0];
lb=coordinates[0][1];
xh=15;  yh=15;
xt=5;   yt=5;
x=(xh+xt)/2;
y=(yh+yt)/2;*/
char arr[1];
while(1)
{
    float diff,ang;
    diff=angle(xh, yh, xt, yt)-angle(x, y, la, lb);
    ang=angle(x,y,la,lb);
    if(lb>=y)
    {
        if (yh>=yt)
        {

            if (diff>=-5 && diff<=5)
                arr[0]='w';//sendCommand('w');
            else if (angle(xh, yh, xt, yt)-angle(x, y, la, lb)>0)
               arr[0]='d';// sendCommand('d');
            else if (angle(xh, yh, xt, yt)-angle(x, y, la, lb)<0)
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

        else if(lb<=y)
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
    }
    sendCommand(arr);
}


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

