#include<stdio.h>
#define MAX 100    			//网络中路由的最大数目
#define INF 999 			//假设为此路由距离为无穷大

int RouterNum;                  //网络中路由的个数

//定义一个路由的信息
struct Router
{
	int dis;               //每条链路的距离
	int neighbor;           //相邻节点的信息,若两节点相邻设置为1，否则为0
	int nexthop;           //接收节点的上一跳的地址
};


Router routertable[MAX][MAX];   //路由表信息
int init[MAX][MAX]={
	{0, 7, 99, 99, 10},
	{7, 0, 1, 99, 8},
	{99, 1, 0, 2, 99},
	{99, 99, 2, 0, 2},
	{10, 8, 99, 2, 0}
};

void InitialNodeState(); 				//初始化节点分布的情况
void PrintNetwork(); 					//网络生成图
void ExchangeInformation();             //相邻节点间交换链路信息
void DV(int send,int recv);             //DV算法

int main()                    
{
      int src,dst;
    int temp=0,temp1,temp2,cost;
      printf("北京理工大学计算机网络实验第五章\n");
      
		InitialNodeState();

		printf("网络的初始距离：\n");
		PrintNetwork();

	// 因为该网络会不停的更新网络的信息，所以这里的循环次数可以根据网络的拓扑而定，一般设有几个节点就循环多少次
	for (int i=0; i < RouterNum; i++){
		ExchangeInformation();
	}

	printf("更新信息后的距离：\n");
	PrintNetwork();             //显示寻找完最短路由的网络图
	
	printf("要更新的网络连接数量\n");
  	scanf("%d",&temp);
  	while(temp--){
  		printf("请输入要更新的网络连接两个节点\n");
  		scanf("%d",&temp1);scanf("%d",&temp2);
  		printf("请输入要更新的网络连接两个节点之间的费用\n");
  		scanf("%d",&cost);
  		routertable[temp1][temp2].dis = routertable[temp2][temp1].dis = cost;
  		for (int i=0; i < RouterNum; i++){
		ExchangeInformation();
	}
		printf("更新信息后的距离：\n");
		PrintNetwork(); 
	  }
	
}

void InitialNodeState()
{

    printf("网络中路由的数目：");
  	scanf("%d",&RouterNum);
 
  	//printf("网络中路由的连接情况\n");
  	for(int i=0; i < RouterNum-1; i++){
		for (int j=i+1; j< RouterNum ; j++){
			routertable[j][i].dis = routertable[i][j].dis =init[i][j];  
		}
  	}
/*  	printf("要更新的网络连接数量\n");
  	scanf("%d",&temp);
  	while(temp--){
  		printf("请输入要更新的网络连接两个节点\n");
  		scanf("%d",&temp1);scanf("%d",&temp2);
  		printf("请输入要更新的网络连接两个节点之间的费用\n");
  		scanf("%d",&cost);
  		routertable[temp1][temp2].dis = routertable[temp2][temp1].dis = cost;
	  }

  	for(int i=0; i < RouterNum-1; i++){
		for (int j=i+1; j< RouterNum ; j++){
			printf("从第%d个节点到第%d个节点的费用:", i,j);
			scanf("%d",&routertable[i][j].exp);
			routertable[j][i].exp = routertable[i][j].exp;     //假设来回的路由的费用相同
		}
  	}
*/
  //通过路由的费用来确认他们之间的相邻关系
  for(int m=0; m < RouterNum; m++){
  	for(int n=0; n < RouterNum; n++){
		if(m==n){
			routertable[m][n].dis = -1;
			routertable[m][n].neighbor = 0;
			routertable[m][n].nexthop = -1;
			}
		else{
			if (routertable[m][n].dis != -1){             //若费用为-1说明两节点无连接，不为相邻的节点
				routertable[m][n].neighbor = 1;       //说明两个节点是相邻的
				routertable[m][n].nexthop = -1;        //初始化时，无上一跳节点   
			}
		else{
			routertable[m][n].neighbor = 0;
			routertable[m][n].nexthop = -1;
			}
		}
	}
  }
}

void PrintNetwork()    //用网格的形式打印出网络中各节点的连接关系
{
	printf("\n--------------BIT-WEB------------------\n");

	for(int i=0 ; i <= RouterNum ; i++){
		for(int j=0 ; j <= RouterNum ; j++){
		if(i==0){
			if(j==0){
			printf("！XGNB！");
			}
			else{
			printf(" 第%d列 ", j-1);
			}
		}
		else{
			if(j==0){
			printf(" 第%d行 ", i-1);
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
			if(routertable[m][n].neighbor == 1){  //相邻节点之间交换链路信息
					DV(m,n); //通过相邻节点的信息更新发送节点的距离向量
				}
			}
		}
}

void DV(int send,int recv)  //接收节点通过接收到的信息来调整自己的距离向量
{
	for (int i=0 ; i < RouterNum ; i++){
		if( routertable[send][i].dis >  0)   //发送节点到第i节点存在路径
		{
			if(routertable[recv][i].dis >  0)  //接收的节点到第i节点存在路径
				{
				//重新计算距离向量
				if(routertable[recv][i].dis > routertable[recv][send].dis + routertable[send][i].dis)
					{
						//如果接收节点到其它节点更近，更新其信息列表，把到i节点的最短路径的下一条设置为发送节点
						routertable[recv][i].dis = routertable[recv][send].dis + routertable[send][i].dis;
						routertable[recv][i].nexthop = send;
					}//否则不做任何的改动
				
				}
			else
			{
				if(recv != i)  //相同节点不做任何处理
					{
						//接收的节点到第i节点的路径不存在，把接收节点到i路径添加到接收节点到i节点最短路径
						routertable[recv][i].dis = routertable[recv][send].dis + routertable[send][i].dis;
						routertable[recv][i].nexthop = send;
					}
			}
		}//若发送节点到第i节点路径不存在，仍不做任何的改动

	}
}

