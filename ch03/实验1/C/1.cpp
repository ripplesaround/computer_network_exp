#include <stdio.h>
#define Mlen 32
#define Glen 17
#define Clen 48
int main(void){
	char msg[Clen],rec[Clen];
	//Ϊ��������֡���CRC�� 
	for(int i=0;i<Clen;i++) msg[i]='0';
	printf("���������������֡��");
	scanf("%s",msg);
	msg[Mlen]='0';
	char msg_0[Clen];
	for(int i=0;i<Clen;i++) msg_0[i]=msg[i];
	printf("���������ݣ�");
	for(int i=0;i<Mlen;i++) printf("%c",msg[i]);
	printf("\n");
	//char gen[Glen]={'1','0','0','1','1'};
	char gen[Glen]={'1','0','0','0','1','0','0','0','0','0','0','1','0','0','0','0','1'};
	printf("���ɶ���ʽ��");
	for(int i=0;i<Glen;i++) printf("%c",gen[i]);
	printf("\n");
	//CRC���� 
	for(int i=0;i<Mlen;i++){
		if(msg[i]=='1'){
			for(int j=0;j<Glen;j++){
				msg[i+j]=(msg[i+j]-48)^(gen[j]-48)+48;
				//printf("%c",msg[i+j]);
			}
			//printf("\n");
		}
	}
	printf("CRC-Code��");
	int flag=1,i;
	for(i=0;i<Clen;i++){
		if(msg[i]!='0'){
			flag=0;
			break;
		}
	}
	if(flag==0){
		for(;i<Clen;i++) printf("%c",msg[i]);
		printf("\n");
	}
	else printf("0\n");
	for(int i=Mlen-1;i<Clen;i++) msg_0[i]=(msg_0[i]-48)+(msg[i]-48)+48;
	printf("��У��͵ķ���֡��");
	for(int i=0;i<Clen;i++) printf("%c",msg_0[i]);
	printf("\n");
	//�Խ�������֡CRC��֤ 
	printf("�������������֡��");
	scanf("%s",rec);
	for(int i=0;i<Mlen;i++){
		if(rec[i]=='1'){
			for(int j=0;j<Glen;j++){
				rec[i+j]=(rec[i+j]-48)^(gen[j]-48)+48;
				//printf("%c",rec[i+j]);
			}
			//printf("\n");
		}
	}
	flag=1;
	for(i=0;i<Clen;i++){
		if(rec[i]!='0'){
			flag=0;
			break;
		}
	}
	if(flag==0){
		for(;i<Clen;i++) printf("%c",rec[i]);
		printf("\n");
	}
	else printf("0\n");
}
