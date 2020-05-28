#include<stdio.h>
#define MAX 100    			//������·�ɵ������Ŀ
#define INF 999 			//����Ϊ��·�ɾ���Ϊ�����

int RouterNum;                  //������·�ɵĸ���

//����һ��·�ɵ���Ϣ
struct Router
{
	int dis;               //ÿ����·�ľ���
	int neighbor;           //���ڽڵ����Ϣ,�����ڵ���������Ϊ1������Ϊ0
	int nexthop;           //���սڵ����һ���ĵ�ַ
};


Router routertable[MAX][MAX];   //·�ɱ���Ϣ
int init[MAX][MAX]={
	{0, 7, 99, 99, 10},
	{7, 0, 1, 99, 8},
	{99, 1, 0, 2, 99},
	{99, 99, 2, 0, 2},
	{10, 8, 99, 2, 0}
};

void InitialNodeState(); 				//��ʼ���ڵ�ֲ������
void PrintNetwork(); 					//��������ͼ
void ExchangeInformation();             //���ڽڵ�佻����·��Ϣ
void DV(int send,int recv);             //DV�㷨

int main()                    
{
      int src,dst;
    int temp=0,temp1,temp2,cost;
      printf("��������ѧ���������ʵ�������\n");
      
		InitialNodeState();

		printf("����ĳ�ʼ���룺\n");
		PrintNetwork();

	// ��Ϊ������᲻ͣ�ĸ����������Ϣ�����������ѭ���������Ը�����������˶�����һ�����м����ڵ��ѭ�����ٴ�
	for (int i=0; i < RouterNum; i++){
		ExchangeInformation();
	}

	printf("������Ϣ��ľ��룺\n");
	PrintNetwork();             //��ʾѰ�������·�ɵ�����ͼ
	
	printf("Ҫ���µ�������������\n");
  	scanf("%d",&temp);
  	while(temp--){
  		printf("������Ҫ���µ��������������ڵ�\n");
  		scanf("%d",&temp1);scanf("%d",&temp2);
  		printf("������Ҫ���µ��������������ڵ�֮��ķ���\n");
  		scanf("%d",&cost);
  		routertable[temp1][temp2].dis = routertable[temp2][temp1].dis = cost;
  		for (int i=0; i < RouterNum; i++){
		ExchangeInformation();
	}
		printf("������Ϣ��ľ��룺\n");
		PrintNetwork(); 
	  }
	
}

void InitialNodeState()
{

    printf("������·�ɵ���Ŀ��");
  	scanf("%d",&RouterNum);
 
  	//printf("������·�ɵ��������\n");
  	for(int i=0; i < RouterNum-1; i++){
		for (int j=i+1; j< RouterNum ; j++){
			routertable[j][i].dis = routertable[i][j].dis =init[i][j];  
		}
  	}
/*  	printf("Ҫ���µ�������������\n");
  	scanf("%d",&temp);
  	while(temp--){
  		printf("������Ҫ���µ��������������ڵ�\n");
  		scanf("%d",&temp1);scanf("%d",&temp2);
  		printf("������Ҫ���µ��������������ڵ�֮��ķ���\n");
  		scanf("%d",&cost);
  		routertable[temp1][temp2].dis = routertable[temp2][temp1].dis = cost;
	  }

  	for(int i=0; i < RouterNum-1; i++){
		for (int j=i+1; j< RouterNum ; j++){
			printf("�ӵ�%d���ڵ㵽��%d���ڵ�ķ���:", i,j);
			scanf("%d",&routertable[i][j].exp);
			routertable[j][i].exp = routertable[i][j].exp;     //�������ص�·�ɵķ�����ͬ
		}
  	}
*/
  //ͨ��·�ɵķ�����ȷ������֮������ڹ�ϵ
  for(int m=0; m < RouterNum; m++){
  	for(int n=0; n < RouterNum; n++){
		if(m==n){
			routertable[m][n].dis = -1;
			routertable[m][n].neighbor = 0;
			routertable[m][n].nexthop = -1;
			}
		else{
			if (routertable[m][n].dis != -1){             //������Ϊ-1˵�����ڵ������ӣ���Ϊ���ڵĽڵ�
				routertable[m][n].neighbor = 1;       //˵�������ڵ������ڵ�
				routertable[m][n].nexthop = -1;        //��ʼ��ʱ������һ���ڵ�   
			}
		else{
			routertable[m][n].neighbor = 0;
			routertable[m][n].nexthop = -1;
			}
		}
	}
  }
}

void PrintNetwork()    //���������ʽ��ӡ�������и��ڵ�����ӹ�ϵ
{
	printf("\n--------------BIT-WEB------------------\n");

	for(int i=0 ; i <= RouterNum ; i++){
		for(int j=0 ; j <= RouterNum ; j++){
		if(i==0){
			if(j==0){
			printf("��XGNB��");
			}
			else{
			printf(" ��%d�� ", j-1);
			}
		}
		else{
			if(j==0){
			printf(" ��%d�� ", i-1);
			}
			else{
			if(routertable[i-1][j-1].dis != -1){
				printf("  [%d]  ",routertable[i-1][j-1].dis);
			}
			else{
			printf("   /   ");
				}
			}
		}
	  }
	printf("\n");
	printf("\n");
	}	
}

void ExchangeInformation()
{
	for (int m=0 ; m < RouterNum ; m++){
		for (int n=0 ; n < RouterNum ; n++){
			if(routertable[m][n].neighbor == 1){  //���ڽڵ�֮�佻����·��Ϣ
					DV(m,n); //ͨ�����ڽڵ����Ϣ���·��ͽڵ�ľ�������
				}
			}
		}
}

void DV(int send,int recv)  //���սڵ�ͨ�����յ�����Ϣ�������Լ��ľ�������
{
	for (int i=0 ; i < RouterNum ; i++){
		if( routertable[send][i].dis >  0)   //���ͽڵ㵽��i�ڵ����·��
		{
			if(routertable[recv][i].dis >  0)  //���յĽڵ㵽��i�ڵ����·��
				{
				//���¼����������
				if(routertable[recv][i].dis > routertable[recv][send].dis + routertable[send][i].dis)
					{
						//������սڵ㵽�����ڵ��������������Ϣ�б��ѵ�i�ڵ�����·������һ������Ϊ���ͽڵ�
						routertable[recv][i].dis = routertable[recv][send].dis + routertable[send][i].dis;
						routertable[recv][i].nexthop = send;
					}//�������κεĸĶ�
				
				}
			else
			{
				if(recv != i)  //��ͬ�ڵ㲻���κδ���
					{
						//���յĽڵ㵽��i�ڵ��·�������ڣ��ѽ��սڵ㵽i·����ӵ����սڵ㵽i�ڵ����·��
						routertable[recv][i].dis = routertable[recv][send].dis + routertable[send][i].dis;
						routertable[recv][i].nexthop = send;
					}
			}
		}//�����ͽڵ㵽��i�ڵ�·�������ڣ��Բ����κεĸĶ�

	}
}

