# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/5/21 16:53
desc: 注意这里实际上是实现了PPT ch06-1 p31 的框图，只是把实验2的连接去掉了，没有组装包checksum之类的
个人感觉实验里没必要写这些，这并不是这个实验的重点，重点应该在与是否进行了连接。
还需要进一步讨论
'''

import socket
import time
import numpy as np
import impacket.ImpactPacket
import warnings
warnings.filterwarnings("ignore")
print("  程序开始执行")
s = socket.socket(socket.AF_INET,socket.SOCK_RAW,socket.IPPROTO_RAW)

ip = impacket.ImpactPacket.IP()
tcp = impacket.ImpactPacket.TCP()

ip.set_ip_src("192.168.1.101")#你的ip
ip.set_ip_dst("192.168.1.100")#目标ip
ip.set_ip_ttl(255)#ttl

tcp.set_th_flags(0b00000010)#将syn标志位设为1
tcp.set_th_sport(12228)#源端口
tcp.set_th_dport(80)#目标端口
tcp.set_th_ack(0)
tcp.set_th_seq(22903)
tcp.set_th_win(20000)#设置Window Size
temp = "hellp"
tcp.set_bytes_from_string(temp)

ip.contains(tcp)
ip.calculate_checksum()
s.sendto(ip.get_packet(),("192.168.1.100",80))#两个参数分别为要发送的数据，类型为bytes与包含目标ip与端口的元祖

print("----------------------------------------")
print("程序结束")