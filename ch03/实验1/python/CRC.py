# coding=UTF-8
'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/25 15:01
desc: 真实的用rec_cal 展示用rec_cal_show
'''
import numpy as np
import copy
class CRC:
    def __init__(self):
        self.InfoString1 = ''
        self.P = 0b10001000000100001
        # self.P = 0b1101
        self.crc = ''
        self.Plen = len(bin(self.P)[2:])

    def cal(self,temp_str):
        len_infostr_orgin = len(temp_str) - self.Plen + 1
        temp = temp_str[0:0+self.Plen]
        r = ''
        for i in range(len_infostr_orgin):
            if len(temp)==self.Plen and temp[0]!='0':
                r = int(temp,2)^self.P
            else:
                r = int(temp,2)
            if i< len_infostr_orgin-1:
                temp = bin(r)[2:] + temp_str[self.Plen+i]
        return r

    def send_cal(self,temp = None):
        if temp!=None:
            self.InfoString1 = temp
        len_infostr_orgin = len(self.InfoString1)
        temp_str = self.InfoString1
        for i in range(self.Plen - 1):
            temp_str += '0'
        r = self.cal(temp_str)
        self.crc = copy.copy(bin(r)[2:])
        temp_str = self.InfoString1
        for i in range(len(bin(r)[2:]), self.Plen - 1):
            temp_str += '0'
            self.crc = '0'+self.crc
        return temp_str + bin(r)[2:]

    def rec_cal(self,temp):
        # temp = self.send_cal()
        r = self.cal(temp)
        if r==0:
            return temp[:32]
        else:
            print("余数不为0")
            return 'error'


    def rec_cal_show(self,i,messages):
        # temp = self.send_cal()
        temp = messages[i*3+2][-32:] + self.crc
        print("接收端收到的发送帧为\t\t\t",temp)
        print("接收数据为\t\t\t\t\t",temp[:32])
        r = self.cal(temp)
        print("余数为\t\t\t\t\t\t",bin(r)[2:])
        if r==0:
            print("本次传输没有错误")
        else:
            print("本次实验出现错误！")

    def show(self,messages):
        K = len(messages)//3
        for i in range(K):
            self.InfoString1 = messages[i*3][-32:]
            print('----------------------------------------------------------')
            print("这是发送的第",i+1,"组数据")
            print("发送数据为\t\t\t\t\t",self.InfoString1)
            print('CRC为\t\t\t\t\t\t',bin(self.P)[2:])
            print("带校验和的发送帧为\t\t\t",self.send_cal())
            print("发送端循环冗余校验码为\t\t", self.crc)

