#计算机网络 chap3 lab2 帧数据信息的字节填充处理
# Author: 韩世杰
# Final Edit：2020.4.8
# Beijing Institute of Technology
#

case = {}
case[1] = "347D7E807E40AA7D"
case[2] = "727E7E58432A7D7D"
case[3] = "7D7F72737E7D7E7"
start_signal = "7E"
end_signal = "7F"
convert_signal = "7D"
#默认起始、终止、转义字符都是两位

def byte_fill(ini_mes): #字节填充
    ini_len = len(ini_mes)
    res_mes = start_signal #添加帧起始标志
    flag = 0
    for i in range(0,ini_len):
        if flag == 1:
            flag = 0
            continue
        if i==ini_len-1:
            res_mes = res_mes+ini_mes[i]
            break
        tmp = ini_mes[i] + ini_mes[i+1]
        if tmp == start_signal or tmp == end_signal or tmp == convert_signal:
            res_mes = res_mes + convert_signal
            res_mes = res_mes + tmp
            flag = 1
        else:
            res_mes = res_mes + ini_mes[i]
    res_mes = res_mes + end_signal #添加帧结束标志
    return res_mes

def byte_delete(res_mes): #字节删除
    res_len = len(res_mes)
    res_mes = res_mes[2:res_len-2] #去掉帧起始和帧结束标志
    res_len = len(res_mes)
    res_ini = ""
    flag = 0
    for i in range(0,res_len):
        if flag > 2:
            flag = flag - 1 
            continue
        elif flag > 0:
            flag = flag - 1
            res_ini = res_ini + res_mes[i]
            continue
        if i == res_len-1:
            res_ini = res_ini + res_mes[i]
            break
        tmp = res_mes[i] + res_mes[i+1]
        if tmp == convert_signal:
            flag = 3
            continue
        else:
            res_ini = res_ini + res_mes[i]
    # print(res_ini)
    return(res_ini)

res_mes = byte_fill(case[2])
byte_delete(res_mes)

print("帧起始标志：{}".format(start_signal))
print("帧结束标志：{}".format(end_signal))
for i in range(1,4):
    print("------测试用例{}------".format(i))
    print("本次测试帧数据信息：{}".format(case[i]))
    print("字节填充后的发送帧：{}".format(byte_fill(case[i])))
    print("字节删除后的接受帧：{}".format(byte_delete(byte_fill(case[i]))))
