# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/3/25 16:46
desc:
'''
import sys
# 调用实验1 中的CRC
sys.path.append('../实验1/python')
from CRC import *
f = open('../config.txt', 'r')

messages = f.read().split('\n')
messages = [message for message in messages if message != '']
test = CRC()
# test.show(messages)
print(test.send_cal('100100'))