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
import time
import _thread
import threading
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

def print_time(threadName, delay):
   start_time = time.time()
   flag = True
   while flag:
       now = time.time()
       if now-start_time>2:
           flag = False

   print("time out")

def rec_ack(threadName, size):
    client.recvfrom(size)
    print('成功发送第', already_send, "条数据")
    already_send += 1

while 1:
    # todo 序号需要修改
    # seq放在第一位
    seq = 0
    se = str(seq)+ test.send_cal(messages[already_send])
    se = se.encode('utf-8')
    client.sendto(se, ('127.0.0.1', UDPPort))

    # todo 计时器

    t1 = threading.Thread(target=print_time, args=("Thread-1", 2,),daemon=False)
    # t2 = threading.Thread(target=rec_ack,args= ("Thread-2", 1024),daemon=True)
    t1.start()
    client.recvfrom(1024)
    print('成功发送第', already_send, "条数据")
    already_send += 1

    if finish == already_send:
        client.sendto('stop'.encode('utf-8'), ('127.0.0.1', UDPPort))
        break

print("----------------------------------------")
print("发送结束")
