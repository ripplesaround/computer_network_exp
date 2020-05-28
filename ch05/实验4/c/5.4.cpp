#include <iostream>
#include <cstring>
using namespace std;
int main(){
	char IPHeader[40];
	cin>>IPHeader;
	cout<<"IPHeader="<<IPHeader<<endl;
	int IPH[40];
	for(int i=0;i<40;i++){
		if(IPHeader[i]>=48&&IPHeader[i]<=57) IPH[i]=IPHeader[i]-48;
		else IPH[i]=IPHeader[i]-55;
	}
	
	//Êä³översion×Ö¶Î 
	int Version=IPH[0];
	cout<<"Version="<<Version<<endl;
	
	//Êä³öIHL×Ö¶Î
	int IHL=IPH[1];
	cout<<"IHL="<<IHL<<endl;
	
	//Êä³öDifferentiated services×Ö¶Î 
	int Differentiated_services=IPH[2]*16+IPH[3];
	cout<<"Differentiated services="<<Differentiated_services<<endl;	

	//Êä³öTotal length×Ö¶Î 
	int Total_length=IPH[4]*16*16*16+IPH[5]*16*16+IPH[6]*16+IPH[7];
	cout<<"Total length="<<Total_length<<endl;
	
	//Êä³öIdentification×Ö¶Î 
	int Identification=IPH[8]*16*16*16+IPH[9]*16*16+IPH[10]*16+IPH[11];
	cout<<"Identification="<<Identification<<endl;
	
	//Êä³öDF×Ö¶Î 
	int DF=IPH[12]/4;
	cout<<"DF="<<DF<<endl;
	
	//Êä³öMF×Ö¶Î 
	int MF=(IPH[12]%4)/2;
	cout<<"MF"<<MF<<endl;
	
	//Êä³öFragment offset×Ö¶Î 
	int Fragment_offset=IPH[12]%2*16*16*16+IPH[13]*16*16+IPH[14]*16+IPH[15];
	cout<<"Fragment offset="<<Fragment_offset<<endl;
	
	//Êä³öTime to live×Ö¶Î 
	int Time_to_live=IPH[16]*16+IPH[17];
	cout<<"Time to live="<<Time_to_live<<endl;
	
	//Êä³öProtocol×Ö¶Î 
	int Protocol=IPH[18]*16+IPH[19];
	cout<<"Protocol="<<Protocol<<endl;
	
	//Êä³öHeader checksum×Ö¶Î 
	int Header_checksum=IPH[20]*16*16*16+IPH[21]*16*16+IPH[22]*16+IPH[23];
	cout<<"Header checksum="<<Header_checksum<<endl;
	
	//Êä³öSource address×Ö¶Î 
	long long int Source_address=0;
	for(int i=24;i<32;i++){
		long long int temp=IPH[i];
		for(int j=1;j<32-i;j++) temp*=16;
		Source_address+=temp;
	}
	cout<<"Source address="<<Source_address<<endl;
	
	//Êä³öDestination address×Ö¶Î 
	long long int Destination_address=0;
	for(int i=32;i<40;i++){
		long long int temp=IPH[i];
		for(int j=1;j<40-i;j++) temp*=16;
		Destination_address+=temp;
	}
	cout<<"Destination address="<<Destination_address<<endl;
	
	//¼ÆËãÊ×²¿Ð£ÑéºÍ 
	int sum;
	for(int i=0;i<10;i++){
		int temp1=0;
		for(int j=0;j<4;j++){
			int temp2=IPH[i*4+j];
			for(int k=1;k<4-j;k++) temp2*=16;
			temp1+=temp2;
		}
//		cout<<temp1<<" ";
		sum+=temp1;
	}
//	cout<<"sum="<<sum<<endl;
	int num[50],m,n,carry=0;
	memset(num,0,sizeof(num));
	for(m=0;m<30;m++){
		num[m]=sum%2;
		sum-=num[m];
		sum/=2;
		if(sum==0) break;
	}
//	for(int i=0;i<30;i++) cout<<num[i];
//	cout<<endl;
	for(n=0;n<16;n++){
		int temp=num[n]+num[n+16]+carry;
		num[n]=temp%2;
		carry=temp/2;
		if(n+16>m&&carry==0) break;
//		cout<<"n="<<n<<" carry="<<carry<<endl;
	}
//	for(int i=0;i<30;i++) cout<<num[i];
//	cout<<endl;
	for(int i=0;i<16;i++) num[i]=1-num[i];
//	for(int i=0;i<30;i++) cout<<num[i];
//	cout<<endl;
	int check=0;
//	for(int i=0;i<16;i++){
//		int temp=num[i];
//		for(int j=0;j<i;j++) temp*=2;
//		check+=temp;
//	}
	cout<<"check=";
	for(int i=15;i>=0;i--) cout<<num[i];
	cout<<endl;
}
