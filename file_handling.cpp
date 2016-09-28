#include<iostream>
#include<fstream>

using namespace std;
struct point{
int x1;
int y1;
int x2;
int y2;
};

int main()
{
    char data[100];
    int i=0,useless;
    point coor[100];
    fstream fobj;
    fobj.open("abcd.box",ios::in);
    while(!fobj.eof())
    {
        fobj>>data[i]>>coor[i].x1>>coor[i].y1>>coor[i].x2>>coor[i].y2>>useless;
        i++;
    }
    for(int j=0;j<i-1;j++)
    {
        cout<<data[j]<<" "<<coor[j].x1<<" "<<coor[j].y1<<" "<<coor[j].x2<<" "<<coor[j].y2<<endl;
    }
}
