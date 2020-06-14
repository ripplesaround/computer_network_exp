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
s = socket.socket(socket.AF_INET,socket.SOCK_RAW,socket.IPPROTO_RAW)

ip = impacket.ImpactPacket.IP()
tcp = impacket.ImpactPacket.TCP()

rec = s.recvfrom(1024)
print(rec[0])


print("----------------------------------------")
print("程序结束")