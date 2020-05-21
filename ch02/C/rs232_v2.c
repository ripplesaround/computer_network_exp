#include <stdio.h> /*标准输入输出定义*/ 
#include <string.h>
#include <stdlib.h> /*标准函数库定义*/ 
#include <unistd.h> /*Unix 标准函数定义*/ 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> /*文件控制定义*/ 
#include <termios.h> /*PPSIX 终端控制定义*/ 
#include <errno.h> /*错误号定义*/
#define MAX_BUFFER_SIZE512

int fd0;
char dev0[10];//my dev
char rbuff[50];
char sbuff[100];
int open_serial(char dev[])
 {
     fd0 = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
     //printf("The dev is %s",dev);
     if(fd0 == -1)
     {
         perror("open serial porterror! %s\n");
         return -1;
     }
     printf("Open serial portsuccess! %s",dev);
     return 0;
}

static void set_baudrate (struct termios *opt, unsigned int baudrate)
{
	cfsetispeed(opt, baudrate);
	cfsetospeed(opt, baudrate);
}

static void set_data_bit (struct termios *opt, unsigned int databit)
{
    opt->c_cflag &= ~CSIZE;
    switch (databit) {
    case 8:
        opt->c_cflag |= CS8;
        break;
    case 7:
        opt->c_cflag |= CS7;
        break;
    case 6:
        opt->c_cflag |= CS6;
        break;
    case 5:
        opt->c_cflag |= CS5;
        break;
    default:
        opt->c_cflag |= CS8;
        break;
    }
}

static void set_parity (struct termios *opt, char parity)
{
    switch (parity) {
    case 'N':                  /* no parity check */
        opt->c_cflag &= ~PARENB;
        opt->c_iflag |= INPCK;
        break;
    case 'E':                  /* even */
        opt->c_cflag |= PARENB;
        opt->c_cflag &= ~PARODD;
        opt->c_iflag |= INPCK;
        break;
    case 'O':                  /* odd */
        opt->c_cflag |= PARENB;
        opt->c_cflag |= ~PARODD;
        opt->c_iflag |= INPCK;
        break;
    default:                   /* no parity check */
        printf("Wrong parity!\n");
        break;
    }
}

static void set_stopbit (struct termios *opt, const int stopbit)
{
    // 设置停止位   
    switch (stopbit)  
    {    
		case 1:     
                 opt->c_cflag &= ~CSTOPB; break;   
		case 2:     
                 opt->c_cflag |= CSTOPB; break;  
		default:     
                printf("Unsupported stop bits\n");   
    }
}


int Set_serial(int fd,int baudrate,int databit,char parity,int stopbit){

    struct termios option;
   	/*tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1. 
    */  
	if( tcgetattr(fd,&option)  !=  0)  
	{  
		perror("SetupSerial 1");      
		return(-1);   
	} 
    cfmakeraw(&option);//调用cfmakeraw()函数可以将终端设置为原始模式，在后面的实例中，采用原始模式进行串口数据通信。
    set_baudrate(&option,baudrate);//设置波特率有专门的函数，用户不能直接通过位掩码来操作。设置波特率的主要函数有cfsetispeed()和cfsetospeed()
    set_data_bit(&option, databit);
    //set_parity(&option,parity);
    set_stopbit(&option,stopbit);

    //激活配置 (将修改后的termios数据设置到串口中）  
    if (tcsetattr(fd,TCSANOW,&option) != 0)    
	{  
		perror("com set error!\n");    
		return (-1);   
	} 
    return (0);
}

 int main(int argc, char*argv[])
 {
    printf("Please enter device serial!\n");
    char num[3];
    scanf("%s",num);
    strcpy(dev0, "/dev/pts/");
    strcat(dev0, num);
    int retv,len;
    char sbuf[50] = {"Hello, this is a serial port test from\n"};
    strcat(sbuf,dev0);
    
    retv =open_serial(dev0);
    if(retv <0)
    {
        perror("open serial porterror!\n");
        return -1;
    }
    
    printf("Ready\n");
    // struct termios option;
    // tcgetattr(fd0,&option);
    // cfmakeraw(&option);
    // cfsetispeed(&option,B9600);
    // cfsetospeed(&option,B9600);
    // tcsetattr(fd0, TCSANOW,&option);
    if(Set_serial(fd0,B9600,8,'E',1)<0){
        perror("Set serial error!\n");
        return -1;
    }
    if(0 == strcmp(argv[1],"0")){
        printf("Please enter the message!\n");
        while(1){
            memset(sbuff,'0',sizeof(char));
            gets(sbuff);
            sbuff[strlen(sbuff)]='\n';
            //puts(sbuff);
            int length = strlen(sbuff);
            retv = write(fd0, sbuff,length);
            if(retv == -1)
            {perror("Write data error!\n");return -1;}
            sleep(2);
        }
    }  
    else{
        while(1)
        {
            while((retv = read(fd0, rbuff, 1)) > 0)
                printf( "%c",*rbuff);
            len = read(fd0,rbuff,99);
            if(len>0){
                rbuff[len]='\0';
                printf("receive data is %s\n",rbuff); 
                printf("len = %d\n",len); 
            }
            sleep(1);
        }
    }



    printf("\n");
    int flag_close =close(fd0);

    return 0;

 }