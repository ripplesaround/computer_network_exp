/*   
#   Network experiment 5.2
#   LS algorithm
#   Author: Lixiang
#   Email:  1075099620@qq.com
*/
#include <iostream>
#include<stdio.h>
#include<stdlib.h>

#define MAXNUM 100
#define N 5
int D[MAXNUM];
int P[MAXNUM];
int que[MAXNUM];
int tot;
int TOPO[N][N]={
                0,  7,  99, 99, 10,
                7,  0,  1,  99, 8,
                99, 1,  0,  2,  99,
                99, 99, 2,  0,  2,
                10, 8,  99, 2,  0,
                };

void printDP();

void dijkstra(int n, int v){

    for(int i=0;i<N;i++)
        D[i]=99;

    bool S[MAXNUM]={0};
    for(int i=0;i<n;i++){
        D[i]=TOPO[v][i];
        S[i]=0;
        if(D[i]==99)
            P[i]=-1;
        else
            P[i]=v;
    }
    S[v]=1;

    for(int i=1;i<n;i++){
        //std::cout<<"i="<<i<<std::endl;
        //printDP();
        int tmp=99;
        int u=v;
        for(int j=0;j<n;j++){
            if(!S[j] && D[j]<tmp){
                u=j;
                tmp=D[j];
            }
        }
        S[u]=1;

        for(int j=0;j<n;j++){
            if(!S[j] && TOPO[u][j]<99){
                int newd = D[u]+TOPO[u][j];
                if(newd<D[j]){
                    D[j]=newd;
                    P[j]=u;
                }
                
            }
        }
    }
}

void searchPath(int v, int u)
{
    for(int i=0;i<MAXNUM;i++)
        que[i]=0;
    tot = 0;
    que[tot] = u;
    tot++;
    int tmp = P[u];
    while(tmp != v)
    {
        que[tot] = tmp;
        tot++;
        tmp = P[tmp];
    }
    que[tot] = v;
    // for(int i=tot; i>=0; --i)
    //     if(i != 0)
    //         std::cout << que[i] << " -> ";
    //     else
    //         std::cout << que[i] << std::endl;
}

void printRouterSST(){
    std::cout<<"The SST between every two router:"<<std::endl;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            dijkstra(N,i);
            printf("%4d",D[j]);
        }
        printf("\n");
    }
}

void printDP(){
    for(int i=0;i<N;i++)
        printf("%4d ",D[i]);
    printf("\n");
    for(int i=0;i<N;i++)
        printf("%4d ",P[i]);
    printf("\n");
}

void printRouterTab(int x){
    dijkstra(N,x);
    std::cout<<"Table of "<<x<<std::endl;
    std::cout<<"Dest NextHip"<<std::endl;
    for(int i=0;i<N;i++){
        if(i==x) continue;
        searchPath(x,i);

        std::cout<<i<<"        "<<que[tot-1]<<std::endl;
    }
    
}

int main(){

    //print TOPO
    std::cout<<"TOPO MATRIX:"<<std::endl;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%4d", TOPO[i][j]);
        }
        printf("\n");
    }

    printRouterSST();

    int x;
    printf("Enter the router number to check the table!\n");
    scanf("%d",&x);
    printRouterTab(x);

}