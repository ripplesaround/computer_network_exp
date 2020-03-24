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
    for i in range(0, len(port_list)):
        print(port_list[i])


try:
    portx = "COM2"
    bps = 9600
    timex = 5
    ser = serial.Serial(portx, bps, timeout=timex,stopbits=1,parity=serial.PARITY_EVEN,bytesize=8)
    # 写数据
    message = input("输入要发送的数据：")
    message = message + '\n'
    result = ser.write(message.encode("gbk"))
    print("写总字节数：", result)
    ser.close()  # 关闭串口
except Exception as e:
    print("error!", e)

