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
sys.path.append('../../实验1/python')
from CRC import *
import time
import _thread
import threading
import math

UDPPort=8888
FilterError=10
FilterLost=10

MAX_SEQ = 7
Len = 3 # ack,seq编码长度 [log7+1]

def between(a,b,c):
    if(a<=b and b<c) or (c<a and a<=b) or (b<c and c<a):
        return True
    else:
        return False

client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

f = open('../config.txt', 'r')
messages = f.read().split('\n')
messages = [message for message in messages if message != '']

# CRC类声明
test = CRC()


already_send = 0
finish = len(messages)
re = ''
flag_ack = False

class rec_ack:
    def __init__(self):
        self._running = True

    def terminate(self):
        self._running = False

    def run(self,n):
        global flag_control
        global re
        global crc_test
        while self._running :
            re = client.recvfrom(1024)
            if re[0].decode('utf-8')!=None:
                print('接收到一个包！！！')
                flag_control = 2

                if re[0].decode('utf-8') == 'timeout':
                    flag_control = 4
                    break
                if re[0].decode('utf-8') == 'run':
                    flag_control = 1
                    break
                # todo 接收停止

                mess = re[0].decode('utf-8')[6:]
                crc_test = test.rec_cal(mess)
                if crc_test == 'error':
                    # crc校验未通过
                    flag_control = 3

                break

# 多个计时器
time_tabale = []
crc_test = ''

se = 'start'.encode('utf-8')
client.sendto(se, ('127.0.0.1', UDPPort))
re = client.recvfrom(1024)
print(f"来自{re[1]}的消息:{re[0].decode('utf-8')}")
selfport = int(re[0].decode('utf-8'))

re = ''
flag_re = False
print("开始发送")
print("----------------------------------------")

# todo 双工
# todo 稍带确认
# todo 计时器链表

next_frame_to_send = 0
ack_expend = 0
frame_expected = 0
nbuffered = 0
seq_nr_i = 0

flag_control = 0        #相当于switch case
# 0-不准许发送 1-发送 2-收到帧 3-cksum_error 4-timeout

def wait_for_event():
    print("等待事件")
    global flag_control
    c = rec_ack()
    t = threading.Thread(target=c.run, args=(10,))
    t.start()
    # 实际上每次只用检测time_table的第一个元素
    # todo 还有有一位判断，防止没有及时处理timeout而重叠
    if len(time_tabale) > 0:
        temp = time_tabale[0]
        if current_time - temp[0] > 3:
            client.sendto('timeout'.encode('utf-8'), ('127.0.0.1', selfport))
    print("flag_control",flag_control)
    if flag_control !=2:
        client.sendto('run'.encode('utf-8'), ('127.0.0.1', selfport))
    c.terminate()
    t.join()

def int2bin(num):
    ans = bin(num)[2:]
    while len(ans)<Len:
        ans = '0'+ans
    return ans




for i in range(10):
    wait_for_event()
    if flag_control == 1:
        print('发送一帧')
        # send
        seq = int2bin(next_frame_to_send)
        ack = int2bin((frame_expected + MAX_SEQ) % (MAX_SEQ + 1))
        se = seq + ack + test.send_cal(messages[already_send])
        se = se.encode('utf-8')
        if np.random.randint(0, FilterLost) > 0:
            client.sendto(se, ('127.0.0.1', UDPPort))
            current_time = time.time()
            time_tabale.append((current_time, (frame_expected + MAX_SEQ) % (MAX_SEQ + 1)))
            next_frame_to_send += 1
            next_frame_to_send %= MAX_SEQ
            print('已经发送第', already_send, "条数据")
        else:
            print('未发送第', already_send, "条数据")

    elif flag_control ==2:
        print('接收到一帧')
        print(f"来自{re[1]}的消息:{re[0].decode('utf-8')}")
        re_seq = int(re[0].decode('utf-8')[0:3],2)
        re_ack = int(re[0].decode('utf-8')[3:6],2)
        mess = re[0].decode('utf-8')[6:]
        re_info = crc_test
        if(re_seq == frame_expected):
            # 向网络层传输数据
            # inc(frame_expected)
            frame_expected += 1
            frame_expected %= MAX_SEQ
    elif flag_control == 3:
        print("check sum error")
    elif flag_control == 4:
        print('time out')
    flag_control = 1
    time.sleep(0.5)
    i+=1
print("----------------------------------------")
print("发送结束")
