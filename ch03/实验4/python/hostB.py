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

class frame:
    def __init__(self,a='',b=0,c=0):
        self.info = a
        self.seq = b
        self.ack = c



print("开始传送")
print("---------------------------------")
for i in range(10):
    s = frame()
    s.seq = i
    s.ack = 1-i
    s.info = 'test'+str(i)
    host.sendto(json.dumps(obj=s.__dict__,ensure_ascii=False).encode('utf-8'), ('127.0.0.1', tarPort))
    i+=1