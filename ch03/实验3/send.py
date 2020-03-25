# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/25 15:01
desc:
'''


# 客户端
import socket

client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while 1:
    se = input('>>>').encode('utf-8')
    client.sendto(se, ('127.0.0.1', 8888))

