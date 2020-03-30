# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/28 1:25
desc:
'''
import socket
import time

client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)# 这里的餐宿已经改成UDP格式了

while 1:
    se = input('>>>').encode('utf-8')
    client.sendto(se, ('127.0.0.1', 8888))
# 虽然可以实现能够与多个人进行数据交换，但实际上是在发送完数据后关闭了链接，并不是真正意义上的并行运行