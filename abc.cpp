#include<iostream>
#include<stdio.h>
 using namespace std;
 int main ()
 {
    char a[50];
    int i=0;
    FILE *fptr;
    fptr = fopen("output.txt","r");
    while(!feof(fptr))
    {
        fscanf(fptr,"%c",&a[i]);
        i++;
    }
    for (int j=0;j<i;j++)
    {
        printf("%c",a[i]);
    }
 }
