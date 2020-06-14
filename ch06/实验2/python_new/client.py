# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/5/21 16:53
desc: 注意这里实际上是实现了PPT ch06-1 p31 的框图，只是把实验2的连接去掉了，没有组装包checksum之类的
个人感觉实验里没必要写这些，这并不是这个实验的重点，重点应该在与是否进行了连接。
还需要进一步讨论
'''

import socket
import time
import numpy as np
import impacket.ImpactPacket
import warnings
warnings.filterwarnings("ignore")
print("  程序开始执行")


HOST = input("ip地址>>> ")
PORT = int(input("端口号>>> "))
# HOST='192.168.1.101'
# PORT= 50007
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((HOST,PORT))

data = input("输入字符串>>> ")
print("Connect %s:%d ok"%(HOST,PORT))
s.send(data.encode("utf-8"))
data=s.recv(1024)
print("Received:",data.decode("utf-8"))
s.close()

print("----------------------------------------")
print("程序结束")