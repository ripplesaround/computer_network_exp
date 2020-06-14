# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/6/14 11:28
desc: 
'''

import socket
HOST='192.168.1.101'
PORT=50007
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)      #定义socket类型，网络通信，TCP
i=s.connect((HOST,PORT))
if i==-1:               #要连接的IP与端口
    print("连接失败")
else:
    print("连接成功")

while 1:
        cmd=input("Pleaseinput cmd:")       #与人交互，输入命令
        s.sendall(cmd.encode("utf-8"))      #把命令发送给对端
        data=s.recv(1024)     #把接收的数据定义为变量
        print( data)         #输出变量
s.close()   #关闭连接
