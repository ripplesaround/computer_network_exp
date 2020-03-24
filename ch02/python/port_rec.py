# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/24 11:36
desc:
'''

import serial

import serial.tools.list_ports

port_list = list(serial.tools.list_ports.comports())
print(port_list)

if len(port_list) == 0:
    print("无可用串口！")
else:
    print("可用串口")
    for i in range(0, len(port_list)):
        print(port_list[i])
    print("------------------")

try:
    portx = "COM1"
    bps = 9600
    timex = None
    # 打开串口，并得到串口对象
    ser = serial.Serial(portx, bps, timeout=timex,stopbits=1,parity=0,bytesize=8)
    print("串口详情参数：", ser)
    # # 十六进制的发送
    # result = ser.write(chr(0x06).encode("utf-8")) # 写数据
    # print("写总字节数：", result)
    # 十六进制的读取
    print(ser.read(11))  # 读一个字节
    print("----------")
    ser.close()  # 关闭串口

except Exception as e:
    print("error!", e)
