# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/5/28 21:34
desc: TCPcc和实验1的结合？
'''
import matplotlib.pyplot as plt

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


class TCP_congestion_control():
    # 采用加性增长，而不是翻倍增长，参考PPT ch06-2 p94
    def __init__(self,MSS= 1024,
                 Threshold = 32768,
                 TriACKRound = 16,
                 TimeoutRound = 22,
                 EndRound = 26):
        self.MSS =MSS
        self.Threshold = Threshold
        self.TriACKRound = TriACKRound
        self.TimeoutRound = TimeoutRound
        self.EndRound = EndRound
        self.cnt = 0  # 传输轮数
        self.CongWin = 0
        self.state = "SS"
        self.event = "ACK"
        #plot
        self.X = []
        self.Y = []
        print(f"MSS：{self.MSS},初始门限值：{self.Threshold}")
    def send(self):
        for i in range(self.EndRound):
            self.event = "ACK"
            if i == self.TriACKRound-1:
                self.event = "triple duplicate ACK"
            elif i == self.TimeoutRound-1:
                self.event = "Timeout"
            self.action()
            print(f"轮次数：{i+1}，当前拥塞窗口值：{self.CongWin}")
            self.X.append(i+1)
            self.Y.append(self.CongWin)
            self.tcpsend()

    def tcpsend(self):
        server_ip, Port = connect_build()
        mess = self.CongWin
        client.sendto(mess.encode("utf-8"), (server_ip, Port))
        t1 = threading.Thread(target=rec_from_server, args=(1,))
        t1.setDaemon(True)
        t1.start()
        time.sleep(0.5)
        print("----------------------------------------")

    def tcpccplot(self):
        plt.title("TCP sender congestion control")
        plt.xlabel("conut")
        plt.ylabel("CongWin")
        plt.plot(self.X,self.Y)
        plt.show()

    def action(self):
        if self.event == "ACK":
            if self.state == "CA":
                self.CongWin += int(self.MSS * self.MSS / self.CongWin)
            else:
                self.CongWin += self.MSS
                if self.CongWin > self.Threshold:
                    self.state = "CA"
        elif self.event == "triple duplicate ACK":
            self.Threshold = int(self.CongWin / 2)
            self.CongWin = self.Threshold
            self.state = "CA"
        elif self.event == "Timeout":
            self.Threshold = int(self.CongWin / 2)
            self.CongWin = self.MSS
            self.state = "SS"



if __name__ == "__main__":
    cc = TCP_congestion_control()
    cc.send()
    cc.tcpccplot()