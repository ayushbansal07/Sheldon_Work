#include<iostream>
#include<fstream>
#include<vector>
#include<stack>
#include<algorithm>

int priority(char ch);

using namespace std;
struct values{
char ctr;
int x1;
int y1;
int x2;
int y2;
};

int main()
{
    int count_char=0,useless,value,max=-9999,min=9999;
    values temp;
    char expression[20];
    char postfix[20];
    vector<values> data,nos,operators;
    fstream fobj;
    stack<char> s;
    stack<int> s2;

    //FILE HANDLING-storing to "data"
    fobj.open("abcd.box",ios::in);
    while(!fobj.eof())
    {
        fobj>>temp.ctr>>temp.x1>>temp.y1>>temp.x2>>temp.y2>>useless;
        data.push_back(temp);
        count_char++;
    }

    count_char--;
    /*for(int j=0;j<count_char;j++)
    {
        cout<<data[j].ctr<<" "<<data[j].x1<<" "<<data[j].y1<<" "<<data[j].x2<<" "<<data[j].y2<<endl;
    }*/

    do
    {
    //SEGREGATION to "nos" and "operators"
    for(int i=0;i<count_char;i++)
    {
        if(data[i].ctr>=49 && data[i].ctr<=57)
        {
            nos.push_back(data[i]);
        }
        else
        {
            operators.push_back(data[i]);
        }
    }

    //MAKING of the EXPRESSION in INFIX
    for(int i=0;i<count_char-1;i++)
    {
        expression[i]=nos[i/2].ctr;
        expression[++i]=operators[i/2].ctr;
    }
    expression[count_char-1]=nos[(count_char-1)/2].ctr;
    expression[count_char]='\0';
    cout<<expression<<endl;

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

    cout<<postfix<<endl;
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
                case '-': s2.push(a-b);
                        break;
                case 'x': s2.push(a*b);
                        break;
                case '/': s2.push(a/b);
                        break;
            }

        }

    }
    value=s2.top();
    s2.pop();
    cout<<value<<endl;
    if(value>max)
    {
        max=value;
    }
    if(value<min)
    {
        min=value;
    }
    }while(std::next_permutation(nos.ctr.begin(),nos.ctr.end());


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

