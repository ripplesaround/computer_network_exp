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



print("开始传送")
print("---------------------------------")
while 1:
    re = host.recvfrom(1024)
    json_str = re[0].decode('utf-8')
    s = json.loads(json_str,object_hook=dict2frame)
    print(f"来自{re[1]}的消息：{s.seq}")
