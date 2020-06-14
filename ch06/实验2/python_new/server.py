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
import datetime
import time
import numpy as np
import impacket.ImpactPacket
import warnings

print("  程序开始执行")

HOST='192.168.1.101'
PORT=50007

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind((HOST,PORT))
s.listen(1)

while True:
    conn,addr=s.accept()
    print('Client %s connected!'%str(addr))
    message = conn.recv(1024)
    message = message.decode("utf-8").upper()
    conn.send(message.encode("utf-8"))
    print("Sent:",message)
    conn.close()
print("----------------------------------------")
print("程序结束")