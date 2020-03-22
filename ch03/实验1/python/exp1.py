# coding=UTF-8
import numpy as np
import copy
class CRC:
    def __init__(self,k=25):
        # temp = np.random.randint(2, size=k)
        # self.InfoString1 = '0110'
        # for i in temp:
        #     self.InfoString1 =  self.InfoString1 + str(i)
        # self.InfoString1 =  self.InfoString1 + "110"
        # # self.InfoString1 = '01100101001000011001010010100110'
        f = open('../config.txt', 'r')
        self.messages = f.read().split('\n')
        self.messages = [message for message in self.messages if message != '']
        self.InfoString1 = ''
        self.P = 0b10001000000100001
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

    def send_cal(self):
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

    def rec_cal(self,i):
        # temp = self.send_cal()
        temp = self.messages[i*3+2][-32:] + self.crc
        print("接收端收到的发送帧为\t\t\t",temp)
        print("接收数据为\t\t\t\t\t",temp[:32])
        r = self.cal(temp)
        print("余数为\t\t\t\t\t\t",bin(r)[2:])
        if r==0:
            print("本次传输没有错误")
        else:
            print("本次实验出现错误！")

    def show(self,K=5):
        for i in range(K):
            self.InfoString1 = self.messages[i*3][-32:]
            print('----------------------------------------------------------')
            print("这是发送的第",i+1,"组数据")
            print("发送数据为\t\t\t\t\t",self.InfoString1)
            print('CRC为\t\t\t\t\t\t',bin(self.P)[2:])
            print("带校验和的发送帧为\t\t\t",self.send_cal())
            print("发送端循环冗余校验码为\t\t", self.crc)
            test.rec_cal(i)


test = CRC()
test.show()