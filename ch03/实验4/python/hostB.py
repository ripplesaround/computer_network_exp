# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/31 2:30
desc:
'''
import socket
import sys
# 调用实验1 中的CRC
sys.path.append('../../实验1/python')
from CRC import *
import time
import _thread
import threading
import math
import json


HostAPort = 8888
HostBPort = 9999
FilterError=10
FilterLost=10

selfPort = HostBPort
tarPort = HostAPort

MAX_SEQ = 7
Len = 3 # ack,seq编码长度 [log7+1]
test = CRC()

f = open('config.txt', 'r')
messages = f.read().split('\n')
messages = [message for message in messages if message != '']

def between(a,b,c):
    if(a<=b and b<c) or (c<a and a<=b) or (b<c and c<a):
        return True
    else:
        return False

host = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
host.bind(('127.0.0.1',selfPort))


next_frame_to_send = 0
ack_expend = 0
frame_expected = 0
nbuffered = 0
seq_nr_i = 0
buffer = []
into_buffer = 0
for i in range(MAX_SEQ+1):
    buffer.append('')
re = ''

class frame:
    def __init__(self,a='',b=-1,c=-1):
        self.info = a
        self.seq = b
        self.ack = c

def send():
    s = frame(test.send_cal(buffer[next_frame_to_send]),next_frame_to_send,frame_expected)
    host.sendto(json.dumps(obj=s.__dict__, ensure_ascii=False).encode('utf-8'), ('127.0.0.1', tarPort))

flag_control = 0
def enable_network_layer():
    global flag_control
    flag_control = 1

def disable_network_layer():
    global flag_control
    flag_control = 0

def wait_for_event():
    pass

def from_network_layer():
    # 从message中取数组，模拟从网络层取包
    buffer[next_frame_to_send] = messages[into_buffer]


print("开始传送")
print("---------------------------------")
def to_network_layer():
    pass

def wait_for_event():
    global re
    global flag_control
    re = host.recvfrom(1024)
    flag_control = 2

enable_network_layer()

while 1:
    wait_for_event()
    if flag_control ==1:
        print("发送一帧")
        from_network_layer()
        into_buffer += 1
        nbuffered += 1
        send()
        next_frame_to_send += 1
        next_frame_to_send %= MAX_SEQ
    elif flag_control == 2:
        print("收到一帧")

    if nbuffered < MAX_SEQ:
        enable_network_layer()
    else:
        disable_network_layer()

