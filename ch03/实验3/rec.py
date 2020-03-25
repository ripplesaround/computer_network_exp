# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/25 15:01
desc:
'''

# 服务端
import socket

server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # 这里的餐宿已经改成UDP格式了

server.bind(('127.0.0.1', 8888))

while 1:
    from_client_data = server.recvfrom(1024)
    if from_client_data == 'stop':
        server.sendto('stop', from_client_data[1])
        break
    print(f"来自{from_client_data[1]}的消息：{from_client_data[0].decode('utf-8')}")
    se = input('>>>').encode('utf-8')
    if se=='stop':
        break
    server.sendto(se, from_client_data[1])