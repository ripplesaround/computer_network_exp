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


HOST = input("ip地址>>> ")
PORT = int(input("端口号>>> "))
#创建socket
skt  = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
#创建发送消息和发送目标
msg = input("输入字符串>>> ")
addr = (HOST,PORT)
skt.sendto(msg.encode("utf-8"),addr)
#接受回复
rst = skt.recvfrom(1024)
print(rst[0].decode("utf-8"))
print('client done')
#关闭链接
skt.close()

print("----------------------------------------")
print("结束发送")
