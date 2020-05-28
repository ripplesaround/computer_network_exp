  #include   <string>   
  #include   <bitset>   
  #include   <iterator>   
  #include   <iostream>   
  #include   <list>   
  #include   <cstdlib>   
  #include   <fstream>   
  using   namespace   std;   
  class   Addr//地址类，4个分量   
		{   
		 public:   
		 Addr(string   str);//构造函数   
		 Addr(const   Addr   &address);//拷贝构造函数   
		 bool   operator==   (const   Addr&   object);   
		 //为了效率，相等操作符只比较整型IP地址，不比较bitset型   
		 Addr&   operator&=   (const   Addr&   object);   
		 void   output();//输出IP地址   
		 ~Addr();   
		 private:   
		 int   str_to_int(string   str); //这个函数进行字符串到整形数的转换，由于它只是私有函数，所以函数实现中只进行了基本   
                                      ///的错误检测，对很多输入错误没有进行检测   
		 inline   void   set(string   str);//把IP地址存到对象中,因为效率，这个函数声明为inline   
		 bitset<8>   *bin_addr1,   *bin_addr2,   *bin_addr3,   *bin_addr4;   
		 
		int   int_addr1,   int_addr2,   int_addr3,   int_addr4;//整数分别存IP地址   
		};   
  Addr::Addr(std::string   str) {   set(str);}   
  Addr::Addr(const   Addr   &address)   
			{     
			 int_addr1   =   address.int_addr1;   
			 int_addr2   =   address.int_addr2;   
			 int_addr3   =   address.int_addr3;   
			 int_addr4   =   address.int_addr4;   
			 bin_addr1   =   new   bitset<8>(*(address.bin_addr1));   
			 bin_addr2   =   new   bitset<8>(*(address.bin_addr2));   
			 bin_addr3   =   new   bitset<8>(*(address.bin_addr3));   
			 bin_addr4   =   new   bitset<8>(*(address.bin_addr4));   
			}   
   Addr::~Addr()
			{       
             delete   bin_addr1;   
             delete   bin_addr2;   
             delete   bin_addr3;   
             delete   bin_addr4;   
			}   
  struct   RouteTable   
		{       
         Addr*   dest;//目的地   
         Addr*   mask;//掩码   
         int   nextdump;//下一跳   
		};   
  inline   void   Addr::set(string   str)//把IP地址存到对象中,因为效率，这个函数声明为inline   
				{           
				 string   strtemp;   
				 int   count   =   1;   
				 int   size   =   int(str.size());     
				 for(string::iterator   pos   =   str.begin();   pos   !=   str.end();   ++pos)   
					{   
					  if(*pos   ==   '.'||   *pos   ==   '/')   
					// if(*pos   ==   '.')   
						{   
                          switch(count)
								{   
								 case   1:   
								 int_addr1   =   str_to_int(strtemp);   
								 bin_addr1   =   new   bitset<8>   (unsigned   long(int_addr1));   
								 break;   
								 case   2:   
								 int_addr2   =   str_to_int(strtemp);   
								 bin_addr2   =   new   bitset<8>   (unsigned   long(int_addr2));   
								 break;   
								 case   3:   
								 int_addr3   =   str_to_int(strtemp);   
								 bin_addr3   =   new   bitset<8>   (unsigned   long(int_addr3));   
								 break;   
								 case   4:   
								 int_addr4   =   str_to_int(strtemp);   
								 bin_addr4   =   new   bitset<8>   (unsigned   long(int_addr4));   
								 break;   
								}   
                         ++count;   
						 strtemp.erase();   
						}   
				//	 else if(*pos   ==   '/') {


					 
				//	 }
					 else     
						{
						 strtemp   +=   *pos;
						}   
				 }   
                 return   ;   
				}   
    int   Addr::str_to_int(string   str)//这个函数进行字符串到整形数的转换   
		{       
		 string::iterator   pos   =   str.begin();   
		 char   chtemp;   
		 int   temp,   result   =   0;   
		 int   size   =   int(str.size());   
		 if(size   ==   1   &&   *pos   ==   '0')   
		 return   0;   
		 for(int   i   =   size;   i   >=   1;   --i)   
			{   
			 chtemp   =   *pos;   
			 switch   (chtemp){   
			 case   '0':   
			 temp   =   0;   
		     break;   
			 case   '1':   
			 temp   =   1;   
			 break;   
			 case   '2':   
			 temp   =   2;   
			 break;   
			 case   '3':   
			 temp   =   3;   
			 break;   
			 case   '4':   
			  temp   =   4;   
			 break;   
			 case   '5':   
			 temp   =   5;   
			 break;   
			 case   '6':   
			 temp   =   6;   
			 break;   
			 case   '7':   
			 temp   =   7;   
			 break;   
			 case   '8':   
			 temp   =   8;   
			 break;   
			 case   '9':   
			 temp   =   9;   
			 break;   
			 default:   
			 return   -1;//不是数字字符，出错   
			}   
		  if(i   ==   3)   
		  result   =   temp*100   +   result;   
		  else   if(i   ==   2)   
		  result   =   temp*10   +   result;   
		  else   if(i   ==   1)   
		  result   =   temp   +   result;   
		  else   
		  return   -1;//防止错误发生   
		  ++pos;//迭代器后移   
		 }   
        return   result;   
	}   
    void   Addr::output()   
		{   
		 cout   <<   int_addr1   <<"."<<   int_addr2   <<"."   
		 <<   int_addr3   <<"."<<   int_addr4   <<   endl;   
		 return;   
		}   
  bool   Addr::operator==   (const   Addr   &object)   
		{       return   (object.int_addr1   ==   int_addr1   &&   object.int_addr2   ==   int_addr2   
		 &&object.int_addr3   ==   int_addr3   &&   object.int_addr4   ==   int_addr4);   
		}   
  Addr&   Addr::operator&=   (const   Addr   &object)   
		{   
		 *bin_addr1   &=   *object.bin_addr1;   
		 *bin_addr2   &=   *object.bin_addr2;   
		 *bin_addr3   &=   *object.bin_addr3;   
		 *bin_addr4   &=   *object.bin_addr4;   
		 int_addr1   =   int(bin_addr1->to_ulong());   
		 int_addr2   =   int(bin_addr2->to_ulong());   
		 int_addr3   =   int(bin_addr3->to_ulong());   
		 int_addr4   =   int(bin_addr4->to_ulong());   
		 return   *this;   
		}   
  int   loadrt(string   filename,   list<RouteTable>   &rt)   
	{       
	 ifstream   infile;   
	 infile.open(filename.c_str());   
	 if(!infile){   
	 cerr   <<   "打开文件失败！"   <<   endl;   
	 system("PAUSE");   
	 return   -1;   
	}   
  string   dest,   mask;   
  int   nextdump;   
  RouteTable*   temprt;   
  while(!infile.eof())   
		{           
		 infile   >>   dest >>   mask   >>   nextdump;   
		 temprt   =   new   RouteTable;   
		 (*temprt).dest   =   new   Addr(dest);   
		 (*temprt).mask   =   new   Addr(mask);   
		 (*temprt).nextdump   =   nextdump;   
		 rt.push_back(*temprt);   
		}   
  return   0;   
  }   
  int   main()   
	{     
	 printf("BIT-NET-Experiment5-Mid_five\n");
	 printf("Learn from teacher Su's instruction : just Baidu and learn it at will !\n");
	 printf("XGNB FGNB\n");
HAHA:	 printf("请输入config文件路径！\n");
	 char temp[100];
	 scanf("%s",temp);//
		string   filename   =   temp;   
//	 string   filename   =   "C:\\Users\\Timmy\\Desktop\\config.txt";   
	 list<RouteTable>   rt;   
	 if(loadrt(filename,   rt)==-1)//从文件载入路由表//destAddr   ->   output();//输出路由表内容   
	 {	printf("请检查并调试路径稍后再次输入！\n");
	 goto HAHA;}
	 cout   <<   "当前路由表内容:\n"   <<   endl; 
	  printf("默认路由:\n下一跳为Router2\n");
	  printf("特殊说明:\n下一跳为0-99号代表路由器router，大于等于100代表端口号interface\n\n");
	 list<RouteTable>::iterator   rtlist   =   rt.begin();
	 int   count   =   1;
	 cout   <<   "第"   <<   count   <<   "条记录：\n";
	// cout<<"与本路由直接相连的网络地址:";
	 (*rtlist).dest->output();
	 cout   <<   "子网掩码:";
	 (*rtlist).mask->output();
	 cout<<"下一跳："<< (*rtlist).nextdump<<endl;
	 cout<<endl;
	 ++count;
	 ++rtlist;

	 while(rtlist   !=   rt.end()&&count<=4&&count>1)   
		{
	     cout   <<   "第"   <<   count   <<   "条记录：\n";     
         cout   <<   "目的地址：";   
         (*rtlist).dest->output();   
         cout   <<   "子网掩码：";   
         (*rtlist).mask->output();   
         cout   <<   "下一跳："   <<   (*rtlist).nextdump   <<"\n"<<   endl;   
         ++count;   
         ++rtlist;   
		}

     cout<<"第"<<count<<"条记录:\n";
	 cout<<"默认路由：";
	 (*rtlist).dest->output();
	 cout<<"子网掩码：";
	 (*rtlist).mask->output();
	 cout   <<   "下一跳："   <<   (*rtlist).nextdump   <<"\n"<<   endl;
  cout   <<   "开始模拟路由器转发分组过程：";   
  // system("PAUSE");   
  cout   <<   "\n";   
  cout   <<   "请输入目的地址以‘/’结束：\n";   
  string   destnation;   
  cin   >>   destnation;   
  cout   <<   "\n";
  char destnationturn[100];
  strcpy(destnationturn,destnation.begin());
  char ch[]=".|";
  char *token,*dot[4];
  int *dota[4];
  int iplen=0;
  token=strtok(destnationturn,ch);
  while(token!=NULL)
		{
		 dot[iplen]=token;
		 iplen++;
		 token=strtok(NULL,ch);
		}
  if(iplen!=4)
  {
	cout<<"输入的IP地址段数不合法！"<<endl;
	system("PAUSE");   
    return   0;
  }
  (int) dot[0];
  (int) dot[1];
  (int) dot[2];
  (int) dot[3];
  int len0,len1,len2,len3,a,b,c,d;
  len0=strlen(dot[0]);
  len1=strlen(dot[1]);
  len2=strlen(dot[2]);
  len3=strlen(dot[3]);
  if(dot[0][0]>57||dot[0][1]>57||dot[0][2]>57||dot[1][0]>57||dot[1][1]>57||dot[1][2]>57||dot[2][0]>57||dot[2][1]>57||dot[2][2]>57||dot[3][0]>57||dot[3][1]>57||dot[3][2]>57)
  {
//	  cout<<"输入的IP地址中含有非法符号！"<<endl;
	  system("PAUSE");   
//	  return   0;
  }
  switch   (len0)
		{   
		 case   1:   
		 a=dot[0][0]-48;  
		 break;
		 case   2:   
		 a=10*(dot[0][0]-48)+dot[0][1]-48;  
		 break;
  	     case   3:   
		 a=100*(dot[0][0]-48)+10*(dot[0][1]-48)+dot[0][2]-48;  
		 break;
		 default:   
		 cout<<"输入的IP地址第一段的位数不合法！"<<endl;
		 system("PAUSE");   
		 return   0;
		}
  switch   (len1)
		{   
		 case   1:   
		 b=dot[1][0]-48;  
		 break;
		 case   2:   
		 b=10*(dot[1][0]-48)+dot[1][1]-48;  
		 break;
  	     case   3:   
		 b=100*(dot[1][0]-48)+10*(dot[1][1]-48)+dot[1][2]-48;  
		 break;
		 default:   
		 cout<<"输入的IP地址第二段的位数不合法！"<<endl;
		 system("PAUSE");   
		 return   0;
		}
  switch   (len2)
		{   
		 case   1:   
		 c=dot[2][0]-48;  
		 break;
		 case   2:   
		 c=10*(dot[2][0]-48)+dot[2][1]-48;  
		 break;
  	     case   3:   
		 c=100*(dot[2][0]-48)+10*(dot[2][1]-48)+dot[2][2]-48;  
		 break;
		 default:   
		 cout<<"输入的IP地址第三段的位数不合法！"<<endl;
		 system("PAUSE");   
		 return   0;
		}
switch   (len3)
		{   
		 case   1:   
		 d=dot[3][0]-48;  
		 break;
		 case   2:   
		 d=10*(dot[3][0]-48)+dot[3][1]-48;  
		 break;
  	     case   3:   
		 d=100*(dot[3][0]-48)+10*(dot[3][1]-48)+dot[3][2]-48;  
		 break;
		 default:   
		 cout<<"输入的IP地址第四段的位数不合法！"<<endl;
		 system("PAUSE");   
		 return   0;
		}
  if(a>255||b>255||c>255||d>255)
	{
	  cout<<"输入的IP地址范围不合法！"<<endl;
	  system("PAUSE");   
	  return   0;
	}
  cout   <<   "输入的IP地址合法！\n";
  if(a>0&&a<127)
	  cout<<"这是一个A类地址"<<endl;
  if(a==127)
	  cout<<"这是一个测试类地址"<<endl;
  if(a>127&&a<192)
      cout<<"这是一个B类地址"<<endl;
  if(a>191&&a<224)
	  cout<<"这是一个C类地址"<<endl;
  if(a>223)
	  cout<<"这是一个D类或E类地址"<<endl;
  cout<<endl;
 // printf("默认路由下一跳为Router2\n");
  Addr*   destAddr   =   new   Addr(destnation);//用子网掩码确定子网   
  Addr*   tempAddr   =   new   Addr(*destAddr);   
  count   =   1;
  rtlist   =   rt.begin();
  *tempAddr   &=   *((*rtlist).mask);   
 // cout   <<   "与子网掩码与运算后的结果：";   
 // tempAddr   ->   output();   
//  cout   <<   "与路由表第"<<   count   <<"条记录记录的网络号相比较：\n";
  if(*tempAddr   ==   *((*rtlist).dest))
	{
//		cout<<"目的网络与本路由相连，数据直接交付。"<<endl;
//		system("PAUSE");   
//		return   0;   
	}
 // cout<<"目的网络不与本路由相连。"<<endl;
//  ++rtlist;
 // ++count;
//  system("PAUSE");
  cout<<"begin:"<<endl;
  for   (rtlist;   rtlist   !=   rt.end()&&count<=5&&count>0;   ++rtlist)   
	 {
	  
	  *tempAddr   &=   *((*rtlist).mask);   
      cout   <<   "与子网掩码与运算后的结果：";   
      tempAddr   ->   output();   
      cout   <<   "与路由表第"<<   count   <<"条记录记录的网络号相比较：\n";   
	  if(*tempAddr   ==   *((*rtlist).dest))
		{
		  cout   <<   "找到目的网络地址：";   
         (*rtlist).dest->output();   
		// printf("%d\n",(*rtlist).nextdump);
		 if((*rtlist).nextdump <100){
				cout   <<   "下一跳交由路由器"   <<   (*rtlist).nextdump   <<   "进行处理"   <<   endl;   
		 }else
         cout   <<   "将数据从端口(interface)"   <<   (*rtlist).nextdump-100   <<   "转发"   <<   endl;   
         cout   <<   "转发完毕！将结束程序\n"   <<   endl;   
         system("PAUSE");   
         return   0;   
		}   
       cout   <<   "第"   <<   count   <<   "次匹配完毕，没有找到目的网络，";   
       system("PAUSE");   
       cout   <<   "\n";
	   ++count;
	 }   
  cout   <<   "\n数据将从默认路由转发:"   <<   endl;
 // cout   <<   "将数据从转发"   <<   endl;
  cout<<"转发完毕，结束！"<<endl;
  system("PAUSE");   
  return   0;   
	}   