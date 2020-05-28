#include <jsoncpp/json/json.h>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
#define MAX 10


void writeJson(){
    Json::Value root;
    Json::Value root2;
    // 组装json内容
    root["BigIPTotalLen"]  = Json::Value(4000);
    root["ID"]        = Json::Value(666);
    root["MTU"]     = Json::Value(1500);

    root2["FragNum"] = 3;
    root2["TotalLen"].append(1500);
    root2["TotalLen"].append(1500);
    root2["TotalLen"].append(1500);
    root2["ID"].append(888);
    root2["ID"].append(888);
    root2["ID"].append(888);
    root2["FragMF"].append(1);
    root2["FragMF"].append(1);
    root2["FragMF"].append(0);
    root2["FragOffset"].append(0);
    root2["FragOffset"].append(185);
    root2["FragOffset"].append(370);

 
    // 将json内容（缩进格式）输出到文件
    Json::StyledWriter writer;
    ofstream os;
    os.open("divide.json");
    os << writer.write(root);
    os.close();
    os.open("assemble.json");
    os << writer.write(root2);
    os.close();
}

void readJson(int &len, int &id, int &mtu){
    Json::Reader reader;
    Json::Value value;
    ifstream divide("divide.json");
    if(!divide.is_open()){
        cout<<"open json file failed,"<<endl;
        return;
    }

    if(reader.parse(divide, value)){
        //cout<<"MTU: "<<value["MTU"]<<endl;
        len = value["BigIPTotalLen"].asInt();
        id  = value["ID"].asInt();
        mtu = value["MTU"].asInt();
    }
    divide.close();
}

void readJson2(int &fragNum, int* MF, int* OFFSET, int* ID, int* LEN){
    Json::Reader reader;
    Json::Value value;

    ifstream assemble("assemble.json");
    if(!assemble.is_open()){
        cout<<"open json file failed,"<<endl;
        return;
    }
    if(reader.parse(assemble, value)){
        fragNum=value["FragNum"].asInt();
        for(int i=0;i<fragNum;i++){
            MF[i]=value["FragMF"][i].asInt();
            OFFSET[i]=value["FragOffset"][i].asInt();
            ID[i]=value["ID"][i].asInt();
            LEN[i]=value["TotalLen"][i].asInt();
        }

    }
    assemble.close();
}


int main()
{
    //writeJson();
    int totalLen,id,mtu;
    int fragNum,len;
    int df,mf,offset;

    readJson(totalLen,id,mtu);
    cout<<"----Fragmentation Information----"<<endl;
    cout<<"Total Length: "<<totalLen<<endl;
    cout<<"ID          : "<<id<<endl;
    cout<<"DF: 0     MF: 0   OFFSET: 0"<<endl;

    fragNum = totalLen/mtu;
    if(totalLen%mtu) fragNum++;
    cout<<"Fragment Num: "<<fragNum<<endl;
    for(int i=0;i<fragNum;i++){
        if(i<fragNum-1){
            len=mtu;
            mf=1;
            df=0;
        }
        else {
            len=totalLen%mtu;
            mf=0;
            df=0;
        }
        offset=i*mtu;
        cout<<"--Fragment"<<i<<"--"<<endl;
        cout<<"Length      : "<<len<<endl;
        cout<<"ID          : "<<id<<endl;
        cout<<"DF: "<<df<<" MF: "<<mf<<" OFFSET: "<<offset<<endl;
    }

    cout<<endl;
    totalLen=0;
    int num;
    int MF[MAX],OFFSET[MAX],ID[MAX],LEN[MAX];
    cout<<"----Fragmentation Assemble Information----"<<endl;
    readJson2(num,MF,OFFSET,ID,LEN);
    for(int i=0;i<num;i++){
        totalLen+=LEN[i];
        cout<<"--Fragment"<<i<<"--"<<endl;
        cout<<"Length      : "<<LEN[i]<<endl;
        cout<<"ID          : "<<ID[i]<<endl;
        cout<<"DF: "<<0<<" MF: "<<MF[i]<<" OFFSET: "<<OFFSET[i]<<endl;
    }
    cout<<"--After Assemble--"<<endl;
    cout<<"Total Length: "<<totalLen<<endl;
    cout<<"ID          : "<<ID[0]<<endl;
    cout<<"DF: 0     MF: 0   OFFSET: 0"<<endl;
    return 0;
}
 

