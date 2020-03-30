# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/31 2:30
desc:
'''

# 发送端
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

selfPort = HostAPort
tarPort = HostBPort

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


class frame:
    def __init__(self,a='',b=0,c=0):
        self.info = a
        self.seq = b
        self.ack = c
def dict2frame(d):
    return frame(d['info'],d['seq'],d['ack'])


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

def send():
    s = frame(test.send_cal(buffer[next_frame_to_send]),next_frame_to_send,frame_expected)
    host.sendto(json.dumps(obj=s.__dict__, ensure_ascii=False).encode('utf-8'), ('127.0.0.1', tarPort))

flag_control = 0        #相当于switch case
# 0-不准许发送 1-发送 2-收到帧 3-cksum_error 4-timeout
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

    if flag_control == 1:
        pass
    elif flag_control == 2:
        print("收到一帧")
        # 将json转换成frame
        json_str = re[0].decode('utf-8')
        s = json.loads(json_str, object_hook=dict2frame)
        print(f"来自{re[1]}的消息：{s.info}")

        if(s.seq == frame_expected):
            to_network_layer()
            frame_expected += 1
            frame_expected %= MAX_SEQ

        while(between(ack_expend,s.ack,next_frame_to_send)):
            nbuffered -= 1
            # stop_time
            ack_expend += 1
            ack_expend %= MAX_SEQ

