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
    ser = serial.Serial(portx, bps, timeout=timex,parity=serial.PARITY_EVEN,stopbits=1,bytesize=8)
    print("串口属性")
    print("串口名称\t", ser.name)
    print("字节大小\t", ser.bytesize)
    print("设置波特率\t", ser.baudrate)
    print("奇偶校验\t", ser.parity)
    print("设置停止位\t", ser.stopbits)
    print("超时设置\t", ser.timeout)
    print("------------------")
    # 十六进制的读取
    result = ser.readline().strip(b'\n')
    print('收到的数据：\t',str(result, encoding = "gbk"))
    print("------------------")
    ser.close()  # 关闭串口

except Exception as e:
    print("error!", e)
