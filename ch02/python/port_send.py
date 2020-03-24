# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/24 11:36
desc: 配合port_rec使用
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
    portx = "COM2"
    bps = 9600
    timex = 5
    ser = serial.Serial(portx, bps, timeout=timex,stopbits=1,parity=serial.PARITY_EVEN,bytesize=8)
    # 写数据
    print("串口属性")
    print("串口名称\t",ser.name)
    print("字节大小\t",ser.bytesize)
    print("波特率\t",ser.baudrate)
    print("奇偶校验\t",ser.parity)
    print("停止位\t",ser.stopbits)
    print("超时设置\t",ser.timeout)
    print("------------------")
    message = input("输入要发送的数据：")
    message = message + '\n'
    result = ser.write(message.encode("gbk"))
    print("写总字节数：", result) #多一位'\n'作为结尾的识别
    print("------------------")
    ser.close()  # 关闭串口
except Exception as e:
    print("error!", e)

