#include<stdio.h>
#include<string>
#include<iostream>
#include<string.h>
using namespace std;
#define MAX_LINE 60
struct frame
{
    string Gen;
    string Info;
};

string ZeroAppend(string str, int n)
{
	string StrZero = "";
	for (int i = 0;i < n;i++)
		StrZero = StrZero + "0";
	return str + StrZero;
}

string GetRemainder(string FrameWithZero, string Gen, int n)
{
	string Remainder = "";
	string Quotient = "";
	int length = FrameWithZero.length() - n;
	Remainder = FrameWithZero.substr(0, n + 1);
	for (int i = 0;i < length;i++)
	{
		if (Remainder[0] == '1')
		{
			Quotient = Quotient + "1";
			for (int j = 1;j <= n;j++)
			{
				if (Remainder[j] == Gen[j])
					Remainder[j] = '0';
				else
					Remainder[j] = '1';
			}
		}
		else if (Remainder[0] == '0')
		{
			Quotient = Quotient + "0";
		}
        if(i != length-1)
        {
            Remainder.erase(0, 1);
			Remainder = Remainder + FrameWithZero[i + n + 1];
        }
	}
	return Remainder;
}

string GetCRC(string Gen, string Frame)
{
    struct frame fra;
	fra.Gen = Gen;
	fra.Info = Frame;
	int n = fra.Gen.length() - 1;
	string FrameWithZero = ZeroAppend(fra.Info, n);
    string SendStr = FrameWithZero;
	string Remainder = GetRemainder(FrameWithZero, fra.Gen, n);
    int length = SendStr.length();
    for(int i=1;i<=n;i++)
    {
        if(SendStr[length-i] == Remainder[n+1-i])
            SendStr[length-i] = '0';
        else
            SendStr[length-i] = '1';
    }
    Remainder.erase(0, 1);
   return Remainder;
}
