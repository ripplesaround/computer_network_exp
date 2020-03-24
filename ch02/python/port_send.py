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
    # 端口：CNU； Linux上的/dev /ttyUSB0等； windows上的COM3等
    portx = "COM2"
    # 波特率，标准值有：50,75,110,134,150,200,300,600,1200,1800,2400,4800,9600,19200,38400,57600,115200
    bps = 9600
    # 超时设置，None：永远等待操作；
    #         0：立即返回请求结果；
    #        其他：等待超时时间（单位为秒）
    timex = 5
    # 打开串口，并得到串口对象
    ser = serial.Serial(portx, bps, timeout=timex)

    # 写数据
    result = ser.write("HELLO WORLD".encode("gbk"))
    print("写总字节数：", result)

    ser.close()  # 关闭串口

except Exception as e:
    print("error!", e)

