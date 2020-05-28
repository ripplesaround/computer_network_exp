  #include   <string>   
  #include   <bitset>   
  #include   <iterator>   
  #include   <iostream>   
  #include   <list>   
  #include   <cstdlib>   
  #include   <fstream>   
  using   namespace   std;   
  class   Addr//��ַ�࣬4������   
		{   
		 public:   
		 Addr(string   str);//���캯��   
		 Addr(const   Addr   &address);//�������캯��   
		 bool   operator==   (const   Addr&   object);   
		 //Ϊ��Ч�ʣ���Ȳ�����ֻ�Ƚ�����IP��ַ�����Ƚ�bitset��   
		 Addr&   operator&=   (const   Addr&   object);   
		 void   output();//���IP��ַ   
		 ~Addr();   
		 private:   
		 int   str_to_int(string   str); //������������ַ�������������ת����������ֻ��˽�к��������Ժ���ʵ����ֻ�����˻���   
                                      ///�Ĵ����⣬�Ժܶ��������û�н��м��   
		 inline   void   set(string   str);//��IP��ַ�浽������,��ΪЧ�ʣ������������Ϊinline   
		 bitset<8>   *bin_addr1,   *bin_addr2,   *bin_addr3,   *bin_addr4;   
		 
		int   int_addr1,   int_addr2,   int_addr3,   int_addr4;//�����ֱ��IP��ַ   
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
         Addr*   dest;//Ŀ�ĵ�   
         Addr*   mask;//����   
         int   nextdump;//��һ��   
		};   
  inline   void   Addr::set(string   str)//��IP��ַ�浽������,��ΪЧ�ʣ������������Ϊinline   
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
    int   Addr::str_to_int(string   str)//������������ַ�������������ת��   
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
			 return   -1;//���������ַ�������   
			}   
		  if(i   ==   3)   
		  result   =   temp*100   +   result;   
		  else   if(i   ==   2)   
		  result   =   temp*10   +   result;   
		  else   if(i   ==   1)   
		  result   =   temp   +   result;   
		  else   
		  return   -1;//��ֹ������   
		  ++pos;//����������   
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
	 cerr   <<   "���ļ�ʧ�ܣ�"   <<   endl;   
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
HAHA:	 printf("������config�ļ�·����\n");
	 char temp[100];
	 scanf("%s",temp);//
		string   filename   =   temp;   
//	 string   filename   =   "C:\\Users\\Timmy\\Desktop\\config.txt";   
	 list<RouteTable>   rt;   
	 if(loadrt(filename,   rt)==-1)//���ļ�����·�ɱ�//destAddr   ->   output();//���·�ɱ�����   
	 {	printf("���鲢����·���Ժ��ٴ����룡\n");
	 goto HAHA;}
	 cout   <<   "��ǰ·�ɱ�����:\n"   <<   endl; 
	  printf("Ĭ��·��:\n��һ��ΪRouter2\n");
	  printf("����˵��:\n��һ��Ϊ0-99�Ŵ���·����router�����ڵ���100����˿ں�interface\n\n");
	 list<RouteTable>::iterator   rtlist   =   rt.begin();
	 int   count   =   1;
	 cout   <<   "��"   <<   count   <<   "����¼��\n";
	// cout<<"�뱾·��ֱ�������������ַ:";
	 (*rtlist).dest->output();
	 cout   <<   "��������:";
	 (*rtlist).mask->output();
	 cout<<"��һ����"<< (*rtlist).nextdump<<endl;
	 cout<<endl;
	 ++count;
	 ++rtlist;

	 while(rtlist   !=   rt.end()&&count<=4&&count>1)   
		{
	     cout   <<   "��"   <<   count   <<   "����¼��\n";     
         cout   <<   "Ŀ�ĵ�ַ��";   
         (*rtlist).dest->output();   
         cout   <<   "�������룺";   
         (*rtlist).mask->output();   
         cout   <<   "��һ����"   <<   (*rtlist).nextdump   <<"\n"<<   endl;   
         ++count;   
         ++rtlist;   
		}

     cout<<"��"<<count<<"����¼:\n";
	 cout<<"Ĭ��·�ɣ�";
	 (*rtlist).dest->output();
	 cout<<"�������룺";
	 (*rtlist).mask->output();
	 cout   <<   "��һ����"   <<   (*rtlist).nextdump   <<"\n"<<   endl;
  cout   <<   "��ʼģ��·����ת��������̣�";   
  // system("PAUSE");   
  cout   <<   "\n";   
  cout   <<   "������Ŀ�ĵ�ַ�ԡ�/��������\n";   
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
	cout<<"�����IP��ַ�������Ϸ���"<<endl;
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
//	  cout<<"�����IP��ַ�к��зǷ����ţ�"<<endl;
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
		 cout<<"�����IP��ַ��һ�ε�λ�����Ϸ���"<<endl;
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
		 cout<<"�����IP��ַ�ڶ��ε�λ�����Ϸ���"<<endl;
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
		 cout<<"�����IP��ַ�����ε�λ�����Ϸ���"<<endl;
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
		 cout<<"�����IP��ַ���Ķε�λ�����Ϸ���"<<endl;
		 system("PAUSE");   
		 return   0;
		}
  if(a>255||b>255||c>255||d>255)
	{
	  cout<<"�����IP��ַ��Χ���Ϸ���"<<endl;
	  system("PAUSE");   
	  return   0;
	}
  cout   <<   "�����IP��ַ�Ϸ���\n";
  if(a>0&&a<127)
	  cout<<"����һ��A���ַ"<<endl;
  if(a==127)
	  cout<<"����һ���������ַ"<<endl;
  if(a>127&&a<192)
      cout<<"����һ��B���ַ"<<endl;
  if(a>191&&a<224)
	  cout<<"����һ��C���ַ"<<endl;
  if(a>223)
	  cout<<"����һ��D���E���ַ"<<endl;
  cout<<endl;
 // printf("Ĭ��·����һ��ΪRouter2\n");
  Addr*   destAddr   =   new   Addr(destnation);//����������ȷ������   
  Addr*   tempAddr   =   new   Addr(*destAddr);   
  count   =   1;
  rtlist   =   rt.begin();
  *tempAddr   &=   *((*rtlist).mask);   
 // cout   <<   "�����������������Ľ����";   
 // tempAddr   ->   output();   
//  cout   <<   "��·�ɱ��"<<   count   <<"����¼��¼���������Ƚϣ�\n";
  if(*tempAddr   ==   *((*rtlist).dest))
	{
//		cout<<"Ŀ�������뱾·������������ֱ�ӽ�����"<<endl;
//		system("PAUSE");   
//		return   0;   
	}
 // cout<<"Ŀ�����粻�뱾·��������"<<endl;
//  ++rtlist;
 // ++count;
//  system("PAUSE");
  cout<<"begin:"<<endl;
  for   (rtlist;   rtlist   !=   rt.end()&&count<=5&&count>0;   ++rtlist)   
	 {
	  
	  *tempAddr   &=   *((*rtlist).mask);   
      cout   <<   "�����������������Ľ����";   
      tempAddr   ->   output();   
      cout   <<   "��·�ɱ��"<<   count   <<"����¼��¼���������Ƚϣ�\n";   
	  if(*tempAddr   ==   *((*rtlist).dest))
		{
		  cout   <<   "�ҵ�Ŀ�������ַ��";   
         (*rtlist).dest->output();   
		// printf("%d\n",(*rtlist).nextdump);
		 if((*rtlist).nextdump <100){
				cout   <<   "��һ������·����"   <<   (*rtlist).nextdump   <<   "���д���"   <<   endl;   
		 }else
         cout   <<   "�����ݴӶ˿�(interface)"   <<   (*rtlist).nextdump-100   <<   "ת��"   <<   endl;   
         cout   <<   "ת����ϣ�����������\n"   <<   endl;   
         system("PAUSE");   
         return   0;   
		}   
       cout   <<   "��"   <<   count   <<   "��ƥ����ϣ�û���ҵ�Ŀ�����磬";   
       system("PAUSE");   
       cout   <<   "\n";
	   ++count;
	 }   
  cout   <<   "\n���ݽ���Ĭ��·��ת��:"   <<   endl;
 // cout   <<   "�����ݴ�ת��"   <<   endl;
  cout<<"ת����ϣ�������"<<endl;
  system("PAUSE");   
  return   0;   
	}   