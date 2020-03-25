# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/25 15:01
desc:
'''


# 发送端
import socket
import sys
# 调用实验1 中的CRC
sys.path.append('../实验1/python')
UDPPort=8888
FilterError=10
FilterLost=10


client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print("开始发送")
print("----------------------------------------")

while 1:
    se = input('>>>').encode('utf-8')
    client.sendto(se, ('127.0.0.1', UDPPort))

