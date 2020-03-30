# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/28 1:25
desc:
'''
import socket
import time
import json

server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # 这里的餐宿已经改成UDP格式了

server.bind(('127.0.0.1', 9000))


print("开始传送")
print("---------------------------------")
while 1:
    from_client_data = server.recvfrom(1024)
    print(f"来自{from_client_data[1]}的消息：{from_client_data[0].decode('utf-8')}")
    server.sendto('from me'.encode('utf-8'), ('127.0.0.1', 9000))
