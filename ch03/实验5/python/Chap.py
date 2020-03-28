# coding=UTF-8
'''
Author: Harry
Email: bit_hanshijie@163.com
date: 2020/3/27 21:54
desc: 将口令与生成的随机数拼接并用MD5算法加密后输出
'''

import chardet
import string
import re
import codecs
import csv
import pandas as pd
import xlwt
import hashlib
import random

# 读取txt内容加密
# file_path = 'C:/Users/Dell/Desktop/Ex5/code.txt'

# with codecs.open(file_path,'rb','utf-8') as f:
    # line = f.readlines() 
    # for message in line:
        # strmes = str(message)
        # m = hashlib.md5()
        # randnum = random.randint(100,999)
        # wrand = str(randnum)
        # m.update(b'123')
        # print(m.hexdigest())

codes = input("请输入要验证的口令:")
randnum = random.randint(100,999)
wrand = str(randnum)
print("生成的随机数为：{}".format(wrand))
print("当前需验证的口令：{}".format(codes))
m = hashlib.md5()
m.update(codes.encode('UTF-8'))
m.update(wrand.encode('UTF-8'))
print("当前生成的MD5摘要值：{}".format(m.hexdigest()))