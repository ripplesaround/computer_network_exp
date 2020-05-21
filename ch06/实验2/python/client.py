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
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
selfport = 0

def connect_build():
    '''
    每次发送前都要建立连接
    :return: 返回要发送到的ip地址和端口地址
    '''
    server_ip = input("server_ip >>>")
    Port = int(input("Port >>>"))
    #'192.168.1.106'
    se = 'start'.encode('utf-8')
    client.sendto(se, (server_ip, Port))
    re = client.recvfrom(1024)
    print(f"来自{re[1]}的消息:{re[0].decode('utf-8')}，连接已建立")
    selfport = int(re[0].decode('utf-8'))
    return server_ip,Port

def rec_from_server(n):
    local_re = ""
    local_re = client.recvfrom(1024)
    print(f"来自{local_re[1]}的消息:{local_re[0].decode('utf-8')}")





while 1:
    server_ip, Port = connect_build()
    mess = input(">>>")
    client.sendto(mess.encode("utf-8"), (server_ip, Port))
    t1 = threading.Thread(target=rec_from_server, args=(1,))
    t1.setDaemon(True)
    t1.start()
    time.sleep(0.5)
    if mess == "stop":
        break
    print("----------------------------------------")

print("----------------------------------------")
print("结束发送")
