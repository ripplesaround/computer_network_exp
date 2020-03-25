# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/25 15:01
desc:
'''

# 接收端
import socket
import sys
# 调用实验1 中的CRC
sys.path.append('../实验1/python')
from CRC import *
import time
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

UDPPort=8888
server.bind(('127.0.0.1',UDPPort))
test = CRC()
print("开始接收")
print("----------------------------------------")
while 1:
    from_client_data = server.recvfrom(1024)
    if from_client_data[0].decode('utf-8') =='stop':
        break
    seq = from_client_data[0].decode('utf-8')[0]
    re = test.rec_cal(from_client_data[0].decode('utf-8')[1:])

    print(f"来自{from_client_data[1]}的消息：{re}，seq为{seq}")
    se = 'ack'.encode('utf-8')  # 其实可以没有内容

    time.sleep(3)
    server.sendto(se, from_client_data[1])  # 可以直接实现阻塞的功能

print("----------------------------------------")
print("接收结束")