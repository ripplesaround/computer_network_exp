#include<stdio.h>
#include<string>
#include<iostream>
#define N 2020

using namespace std;

string FlagString_1 ("01111110");
string FlagString_2 ("7E");

string Send_1(string SendString_1){
	int count=0;
	string temp;
	temp.append(FlagString_1);
	for(int i=0;i<SendString_1.size();i++){
		if(count==5){
			temp.append("0");
			count=0;
			}
		if(SendString_1[i]=='0'){
				temp.append("0");
				count=0;	
		}
		else {
			count++;
			temp.append("1");
		}
	}
	temp.append(FlagString_1);
//	cout<<"Send_Message:"<<temp<<endl;
	return temp;
}
string receive_1(string r){
	string temp;
	int count=0;
	int begin = r.find(FlagString_1)+FlagString_1.size();
	int end = r.rfind(FlagString_1);
//	printf("%d %d\n",begin,end);
	for(int i=begin;i<end;i++){
		if(count==5) {
			count=0;
			continue;
		}
		if(r[i]=='0'){
				temp.append("0");
				count=0;	
		}
		else {
			count++;
			temp.append("1");
		}
	}
//	cout<<"Receive_Message:"<<temp<<endl;
	return temp;
}
string Send_2(string s){
	string temp;
	int t,i=0,j;
	string HaHa = ("7D");
	temp.append(FlagString_2);
	j=temp.size();
	while(i<s.size()){
		t = min(s.find(FlagString_2,i),s.find(HaHa,i));
		while(i<t){
			temp.append("!");
			temp[j]=s[i];
			i++;j++;
		}
		temp.append(HaHa);j++;j++;
		while(i<t+HaHa.size()){
			temp.append("!");
			temp[j]=s[i];
			i++;j++;
		}
//		cout<<"Send_Message:"<<temp<<endl;
	}
	temp.append(FlagString_2);
//	cout<<"Send_Message:"<<temp<<endl;
	return temp;
}
string receive_2(string r){
	string temp;
	string f1 = ("7D7D");
	string f2 = ("7D");f2.append(FlagString_2);
	int t,i=0,j=0,tt;
	i = r.find(FlagString_2)+FlagString_2.size();
	while(i<r.size()){
		t=min(r.find(f1,i),r.find(f2,i));
		while(i<t){
			temp.append("!");
			temp[j]=r[i];
			i++;j++;
		}i++;i++;
		while(i<t+2*FlagString_2.size()){
			temp.append("!");
			temp[j]=r[i];
			i++;j++;
		}		
	}
	return temp;
}
int main(){
	string SendString_1 ("00111110011111100111111000111110");
	string SendString_2 ("347D7E807E40AA7D");
//	string str; 
//	getline(cin,str); 
//	cout<<str<<endl; 
//	string SendString_2(str);
	cout<<"FlagString_1:"<<FlagString_1<<endl;
	cout<<"FlagString_2:"<<FlagString_2<<endl;
	cout<<"SendString_1:"<<SendString_1<<endl;
	cout<<"SendString_2:"<<SendString_2<<endl;
	string Send1 = Send_1(SendString_1);
	cout<<"Send_Message_1:"<<Send1<<endl;
	string receive1 = receive_1(Send1);
	cout<<"Receive_Message_1:"<<receive1<<endl;
	string Send2 = Send_2(SendString_2);
	cout<<"Send_Message_2:"<<Send2<<endl;
	string receive2 = receive_2(Send2);
	cout<<"Receive_Message_2:"<<receive2<<endl;
	printf("XG brings us fly!\n");
	return 0;
} 
