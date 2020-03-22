import numpy as np

class CRC:
    def __init__(self,k=25):
        # temp = np.random.randint(2, size=k)
        # self.InfoString1 = '0110'
        # for i in temp:
        #     self.InfoString1 =  self.InfoString1 + str(i)
        # self.InfoString1 =  self.InfoString1 + "110"
        self.InfoString1 = '101001'
        self.P = '1100'

    def cal(self):
        len_infostr_orgin = len(self.InfoString1)
        for i in range(len(self.P)-1):
            self.InfoString1 += '0'
        # 应该先计算过去一次
        # for i in range(len_infostr_orgin):
        #     temp = self.InfoString1[i:i+len(self.P)]



test = CRC()
test.cal()