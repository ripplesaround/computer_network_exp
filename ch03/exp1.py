import numpy as np

class CRC:
    def __init__(self,k=25):
        # temp = np.random.randint(2, size=k)
        # self.InfoString1 = '0110'
        # for i in temp:
        #     self.InfoString1 =  self.InfoString1 + str(i)
        # self.InfoString1 =  self.InfoString1 + "110"
        self.InfoString1 = '101001'
        self.P = 0b1101
        self.Plen = len(bin(self.P)[2:])

    def cal(self,temp_str):
        len_infostr_orgin = len(temp_str) - self.Plen + 1
        temp = temp_str[0:0+self.Plen]
        r = ''
        for i in range(len_infostr_orgin):
            if len(temp)==self.Plen:
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
        temp_str = self.InfoString1
        for i in range(len(bin(r)[2:]), self.Plen - 1):
            temp_str += '0'
        return temp_str + bin(r)[2:]

    def rec_cal(self):
        temp = send_cal()



test = CRC()
print(test.send_cal())