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
from CRC import *
UDPPort=8888
FilterError=10
FilterLost=10

client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
f = open('config.txt', 'r')
messages = f.read().split('\n')
messages = [message for message in messages if message != '']
test = CRC()
print("开始发送")
print("----------------------------------------")

already_send = 0
finish = len(messages)

while 1:
    se = []
    # todo 序号需要修改
    seq = 0

    se.append(test.send_cal(messages[already_send]))
    se.append(seq)
    se = se.encode('utf-8')
    client.sendto(se, ('127.0.0.1', UDPPort))
    # todo 计时器

    client.recvfrom(1024)
    print('成功发送第',already_send,"条数据" )
    already_send += 1
    if finish == already_send:
        client.sendto('stop'.encode('utf-8'), ('127.0.0.1', UDPPort))
        break

print("----------------------------------------")
print("发送结束")
