# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/25 15:01
desc:
'''

# 接收端
import socket

server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

UDPPort=8888
server.bind(('127.0.0.1',UDPPort))

print("开始接收")
print("----------------------------------------")
while 1:
    from_client_data = server.recvfrom(1024)
    print(f"来自{from_client_data[1]}的消息：{from_client_data[0].decode('utf-8')}")