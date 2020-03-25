# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/25 15:01
desc:
'''

# 接收端
import socket
import sys
# 调用实验1 中的CRC
sys.path.append('../实验1/python')
from CRC import *
import time
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

UDPPort=8888
server.bind(('127.0.0.1',UDPPort))
test = CRC()



from_client = server.recvfrom(1024)
print(f"来自{from_client[1]}的消息：{from_client[0].decode('utf-8')}")
server.sendto(str(from_client[1][1]).encode('utf-8'),from_client[1])

test1 = 0
print("开始接收")
print("----------------------------------------")

frame_expected = 0
while 1:
    from_client_data = server.recvfrom(1024)
    if from_client_data[0].decode('utf-8') =='stop':
        break
    seq = int(from_client_data[0].decode('utf-8')[0])
    mess = from_client_data[0].decode('utf-8')[1:]
    # 帧出错
    if test1 == 2:
        s1 = list(mess)
        s1[10] = str((int(s1[10])+1)%2)
        mess = ''.join(s1)
    re = test.rec_cal(mess)

    if re =='error':
        ack = (seq+1)%2
        se = str(ack).encode('utf-8')
        server.sendto(se, from_client_data[1])
        test1 += 1
        print("此次传输中帧损坏")
        continue
    print(f"来自{from_client_data[1]}的消息：{re}，seq为{seq}")

    if seq == frame_expected:
        # pass it to network layer
        frame_expected += 1
        frame_expected %= 2
    ack = 1-frame_expected

    se = str(ack).encode('utf-8')  # 其实可以没有内容
    if test1 != 0 and test1!= 6:
        server.sendto(se, from_client_data[1])  # 可以直接实现阻塞的功能
        # print("send ack_pack")
    # server.sendto(se, from_client_data[1])
    test1 += 1

print("----------------------------------------")
print("接收结束")