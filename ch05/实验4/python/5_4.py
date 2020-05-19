import numpy as np

def conv(ch):
    if ch == 'A' or ch =='a':
        return 10
    elif ch == 'B' or ch =='b':
        return 11
    elif ch == 'C' or ch =='c':
        return 12
    elif ch =='D' or ch =='d':
        return 13
    elif ch == 'E' or ch =='e':
        return 14
    elif ch == 'F' or ch =='f':
        return 15
    else:
        # print(int(ch))
        return int(ch)

with open('C:\\Users\\Dell\\Desktop\\IPHeader.txt',"r") as f:    #设置文件对象
    str = f.read()
    print("IP数据报首部的十六进制值：{}".format(str))
    
    total = 0
    start = 0
    end = 1
    for i in range(start,end):
        ch = conv(str[i])
        total = pow(16,end-i-1)*ch + total
        # print("total:{}".format(int('4')))
    # print("版本字段十六进制：{}".format(str[start:end]))
    print("版本字段十进制：{}".format(total))

    total = 0
    start = 1
    end = 2
    for i in range(start,end):
        ch = conv(str[i])
        total = pow(16,end-i-1)*ch + total
    # print("IHL字段十六进制：{}".format(str[start:end]))
    print("IHL字段十进制：{}".format(total))

    total = 0
    start = 2
    end = 4
    for i in range(start,end):
        ch = conv(str[i])
        total = pow(16,end-i-1)*ch + total
    # print("区分服务字段十六进制：{}".format(str[start:end]))
    print("区分服务字段十进制：{}".format(total))


    total = 0
    start = 4
    end = 8
    for i in range(start,end):
        ch = conv(str[i])
        total = pow(16,end-i-1)*ch + total
    # print("总长度字段十六进制：{}".format(str[start:end]))
    print("总长度字段十进制：{}".format(total))

    total = 0
    start = 8
    end = 12
    for i in range(start,end):
        ch = conv(str[i])
        total = pow(16,end-i-1)*ch + total
    # print("标识字段十六进制：{}".format(str[start:end]))
    print("标识字段十进制：{}".format(total))

    # DF MF
    tmp = conv(str[12])
    # print(tmp)
    # 去掉未使用位
    tmp = int(tmp%8)
    # print(tmp)
    print("DF字段十进制：{}".format(int(tmp/4)))
    # 去掉DF位
    tmp = int(tmp%4)
    print("MF字段十进制：{}".format(int(tmp/2)))
    # 去掉MF位
    tmp = tmp%2

    # 分段偏移量
    total = pow(16,12)*tmp
    start = 13
    end = 16
    for i in range(start,end):
        ch = conv(str[i])
        total = pow(16,end-i-1)*ch + total
    print("分段偏移量字段十进制：{}".format(total))

    total = 0
    start = 16
    end = 18
    for i in range(start,end):
        ch = conv(str[i])
        total = pow(16,end-i-1)*ch + total
    print("生存期字段十进制：{}".format(total))

    total = 0
    start = 18
    end = 20
    for i in range(start,end):
        ch = conv(str[i])
        total = pow(16,end-i-1)*ch + total
    print("协议字段十进制：{}".format(total))

    total = 0
    start = 20
    end = 24
    for i in range(start,end):
        ch = conv(str[i])
        total = pow(16,end-i-1)*ch + total
    print("头校验和字段十进制：{}".format(total))

    total = 0
    start = 24
    end = 32
    t = np.zeros((4))
    for i in range(0,4):
        t[i] = 0
        for j in range(0,2):
            ch = conv(str[start+2*i+j])
            t[i] = t[i] + pow(16,1-j)*ch
    print("源地址字段十进制：{}.{}.{}.{}".format(int(t[0]),int(t[1]),int(t[2]),int(t[3])))

    total = 0
    start = 32
    end = 40
    t = np.zeros((4))
    for i in range(0,4):
        t[i] = 0
        for j in range(0,2):
            ch = conv(str[start+2*i+j])
            t[i] = t[i] + pow(16,1-j)*ch
    print("目标地址字段十进制：{}.{}.{}.{}".format(int(t[0]),int(t[1]),int(t[2]),int(t[3])))


    total = 0
    start = 0
    end = 40
    for i in range(0,10):
        for j in range(0,4):
            total = total + pow(16,3-j)*conv(str[4*i+j])
    left = total - (total%(16**4))
    right = total%(16**4)
    print(16**4)
    left = int(left/pow(16,4))
    print(right)
    print(left)
    x = right +left
    print(type(bin(x)))
    print('转换为二进制数的补码为：',bin(2**16+(x)))