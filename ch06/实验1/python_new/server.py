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
print("  程序开始执行")

#创建socket
skt = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
#绑定地址和端口
skt.bind(('192.168.1.101',50007))
#循环
while True:
    #调用接受消息
    data,addr = skt.recvfrom(1024)
    #接受成功回复消息
    rst = data.decode("utf-8").upper().encode("utf-8")
    skt.sendto(rst,addr)
    print('server Done')
#关闭链接
skt.close()

print("----------------------------------------")
print("程序结束")