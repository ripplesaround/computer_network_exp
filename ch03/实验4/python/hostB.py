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
import numpy as np

# done 捎带确认

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
def dict2frame(d):
    return frame(d['info'],d['seq'],d['ack'])


def send():
    s = frame(test.send_cal(buffer[next_frame_to_send]),next_frame_to_send,(frame_expected+MAX_SEQ)%(MAX_SEQ+1))
    模拟传输丢失
    if np.random.randint(0, FilterLost) > 0:
        host.sendto(json.dumps(obj=s.__dict__, ensure_ascii=False).encode('utf-8'), ('127.0.0.1', tarPort))
    else:
        print("     传输丢失")
    # host.sendto(json.dumps(obj=s.__dict__, ensure_ascii=False).encode('utf-8'), ('127.0.0.1', tarPort))
    # start_timer
    time_table.append([time.time(), next_frame_to_send])

flag_control = 0
time_table = []

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

k = host.sendto('start'.encode('utf-8'), ('127.0.0.1', tarPort))
print("开始传送")
print("---------------------------------")
def to_network_layer():
    pass

def rec(arg):
    global re
    global flag_control
    re = host.recvfrom(1024)
    flag_control = 2

def wait_for_event():
    global re
    global flag_control
    orgin = flag_control
    if flag_control == 2 or flag_control == 3 or flag_control == 4:
        orgin = 0
    re = ''
    t1 = threading.Thread(target=rec, args=(3,))
    t1.start()
    t1.join(0.01)
    if re == '':
        host.sendto('run'.encode('utf-8'), ('127.0.0.1', selfPort))
        if len(time_table)>0:
            now = time.time()
            if now - time_table[0][0]>3:
                orgin = 4
                # print(now, time_table)
        t1.join(0.00001)
        # print("此时未接收到frame")
        flag_control = orgin
    # print("flag_control",flag_control)


enable_network_layer()
already_send = 0

re_error = -1    #记录需要重传的帧的序号

while 1:
    wait_for_event()
    if flag_control ==1:
        print("发送一帧")
        if into_buffer == len(messages):
            host.sendto('stop'.encode('utf-8'), ('127.0.0.1', tarPort))
            print("发送结束")
            break
        from_network_layer()
        into_buffer += 1
        nbuffered += 1
        send()
        if (next_frame_to_send < MAX_SEQ):
            next_frame_to_send += 1
        else:
            next_frame_to_send = 0
        # next_frame_to_send += 1
        # next_frame_to_send %= MAX_SEQ
    elif flag_control == 2:
        if re[0].decode('utf-8') =='run':
            continue
        if re[0].decode('utf-8') == 'stop':
            print("接收结束")
            break
        print(f"收到一帧,frame_expected:{frame_expected},ack_expend:{ack_expend},next_frame_to_send:{next_frame_to_send},nbuffer:{nbuffered}")
        # 将json转换成frame
        json_str = re[0].decode('utf-8')
        s = json.loads(json_str, object_hook=dict2frame)
        s.info = test.rec_cal(s.info)
        if s.info == 'error':
            print("crc校验未通过")
            continue
        print(f"来自{re[1]}的消息：{s.info},s.seq:{s.seq},s.ack:{s.ack}")
        re = ''

        # 自动可以保证按序接收，不要的直接丢弃，相当于接收窗口未移动
        if (s.seq == frame_expected):
            to_network_layer()
            # frame_expected += 1
            # frame_expected %= MAX_SEQ
            if(frame_expected<MAX_SEQ):
                frame_expected+=1
            else:
                frame_expected = 0

        while (between(ack_expend, s.ack, next_frame_to_send)):
            nbuffered -= 1
            # todo stop_time
            print(f"确认第{ack_expend}帧已送达")

            # 按照顺序送达，所以根据有序性，直接按照顺序pop
            time_table.pop(0)

            already_send +=1
            # ack_expend += 1
            # ack_expend %= MAX_SEQ
            if (ack_expend < MAX_SEQ):
                ack_expend += 1
            else:
                ack_expend = 0
    elif flag_control == 3:
        print("check sum error")
    elif flag_control == 4:
        print("time out")
        next_frame_to_send = ack_expend
        time_table = []  # 洗掉time_table

        for i in range(nbuffered):
            send()
            # next_frame_to_send += 1
            # next_frame_to_send %= MAX_SEQ
            print(f"重传第{next_frame_to_send}帧")
            if (next_frame_to_send < MAX_SEQ):
                next_frame_to_send += 1
            else:
                next_frame_to_send = 0

    if nbuffered < MAX_SEQ:
        enable_network_layer()
    else:
        disable_network_layer()
    print("-------------------")
print("end")
print("into_buffer",into_buffer)
print(len(messages))
print("already_send",already_send)

