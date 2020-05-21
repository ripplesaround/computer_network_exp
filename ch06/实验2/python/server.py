# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/5/21 16:53
desc:
'''

import socket
import time
import numpy as np
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

UDPPort=8888
server.bind(('127.0.0.1',UDPPort))

from_client = server.recvfrom(1024)
print(f"来自{from_client[1]}的消息：{from_client[0].decode('utf-8')}")
server.sendto(str(from_client[1][1]).encode('utf-8'),from_client[1])

print("开始接收")
print("----------------------------------------")
while 1:
    re = server.recvfrom(1024)
    print(f"来自{re[1]}的消息:{re[0].decode('utf-8')}")
