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
        global flag_ack
        flag_ack = False
        global re
        while self._running :
            re = client.recvfrom(1024)
            if re[0].decode('utf-8')!=None:
                flag_ack = True
                break

se = 'start'.encode('utf-8')
client.sendto(se, ('127.0.0.1', UDPPort))
re = client.recvfrom(1024)
print(f"来自{re[1]}的消息:{re[0].decode('utf-8')}")
selfport = int(re[0].decode('utf-8'))
re = ''
flag_re = False
print("开始发送")
print("----------------------------------------")

next_frame_to_send = 0

while 1:
    # todo 序号需要修改
    # seq放在第一位
    seq = next_frame_to_send
    se = str(seq)+ test.send_cal(messages[already_send])
    se = se.encode('utf-8')
    client.sendto(se, ('127.0.0.1', UDPPort))
    print('已经发送第', already_send, "条数据")

    # done 计时器

    c = rec_ack()
    t = threading.Thread(target=c.run, args=(10,))
    t.start()
    current_time = time.time()
    while(1):
        now = time.time()
        if flag_ack:
            # print("接收ack")
            break
        if now - current_time > 2:
            print("time out")
            client.sendto('timeout'.encode('utf-8'), ('127.0.0.1',selfport))
            flag_re = True
            break
    c.terminate()
    t.join()
    if flag_re :
        flag_re = False
        print('重新发送第', already_send, "条数据，原因：帧丢失")
        continue
    ack = int(re[0].decode('utf-8'))
    if ack == next_frame_to_send:
        print('成功发送第', already_send, "条数据")
        next_frame_to_send += 1
        next_frame_to_send %= 2
        print("----------------")
        already_send += 1
    else:
        print('重新发送第', already_send, "条数据，原因：帧出错")
        continue

    if finish == already_send:
        client.sendto('stop'.encode('utf-8'), ('127.0.0.1', UDPPort))
        break

print("----------------------------------------")
print("发送结束")
