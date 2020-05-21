# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/5/21 16:53
desc: 注意这里实际上是实现了PPT ch06-1 p24 的框图，每次都要建立了连接，如果每次都要建立连接的话需要在client端每次进行输入和发送，server端每次也都要确认
'''

import socket
import time
import numpy as np
print("  程序开始执行")
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

UDPPort=8888
server.bind(('192.168.1.106',UDPPort))

client_table = []
def connect_build():
    from_client = server.recvfrom(1024)
    print(f"来自{from_client[1]}的消息：{from_client[0].decode('utf-8')}，连接已建立")
    server.sendto(str(from_client[1][1]).encode('utf-8'),from_client[1])
    client_table.append(from_client[1])

print("----------------------------------------")
while 1:
    connect_build()
    re = server.recvfrom(1024)
    if re[0].decode('utf-8') == "stop":
        break
    print(f"来自{re[1]}的消息:{re[0].decode('utf-8')}")
    temp = re[0].decode('utf-8').upper()
    server.sendto(temp.encode('utf-8'), client_table[0])
    client_table.clear()
    print("----------------------------------------")

print("----------------------------------------")
print("程序结束")