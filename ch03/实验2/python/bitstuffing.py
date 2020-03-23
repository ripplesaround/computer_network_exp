# coding = UTF-8

class bitstuffing:
    def __init__(self):
        f = open('../config.txt', 'r')
        self.messages = f.read().split('\n')
        self.messages = [message for message in self.messages if message != '']
        self.messages = self.messages[:3]
        self.InfoSrting1 = ''
        self.FlagString = '01111110'

    def stuffing(self,temp_str):
        flag = 0
        len_str = len(temp_str)
        ans = ''
        for i in range(len_str):
            if temp_str[i] == '1':
                flag += 1
                ans += '1'
            else :
                flag = 0
                ans += '0'
            if flag == 5:
                ans+='0'
                flag = 0
        return self.FlagString+ans+self.FlagString

    def decode(self,temp_str):
        flag = 0
        len_str = len(temp_str)
        ans = ''
        for i in range(len_str):
            if temp_str[i] == '1':
                flag += 1
                ans += '1'
            else:
                flag = 0
                ans += '0'
            if flag == 5:
                i+=1
                print('hellp')
                flag = 0
        return ans

    def show(self):
        i = 1
        for temp_str in self.messages:
            print('--------------------------------')
            print("填充第",i,"组数据")
            i+=1
            print("帧起始标志",self.FlagString)
            print("帧数据信息",temp_str)
            print("帧结束标志",self.FlagString)
            temp = self.stuffing(temp_str)
            print("比特填充后的发送帧",temp)
            print("比特删除后的发送帧",self.decode(temp))


test = bitstuffing()
test.show()