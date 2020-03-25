# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/25 15:01
desc:
'''


# 客户端
import socket

client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # 这里的餐宿已经改成UDP格式了

while 1:
    se = input('>>>').encode('utf-8')
    if se=='stop':
        client.sendto(se, ('127.0.0.1', 8888))
        break
    client.sendto(se, ('127.0.0.1', 8888))
    re = client.recvfrom(1024)
    print(f"来自{re[1]}的消息:{re[0].decode('utf-8')}")
# 虽然可以实现能够与多个人进行数据交换，但实际上是在发送完数据后关闭了链接，并不是真正意义上的并行运行