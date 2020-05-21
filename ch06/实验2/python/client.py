# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/5/21 17:04
desc:
'''

import socket
import sys
import time
import _thread
import threading
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
UDPPort=8888

se = 'start'.encode('utf-8')
client.sendto(se, ('127.0.0.1', UDPPort))
re = client.recvfrom(1024)
print(f"来自{re[1]}的消息:{re[0].decode('utf-8')}")
selfport = int(re[0].decode('utf-8'))
re = ''

print("开始发送")
print("----------------------------------------")


while 1:
    mess = input(">>>")
    client.sendto(mess.encode("utf-8"), ('127.0.0.1', UDPPort))

