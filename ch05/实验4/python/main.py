# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/5/14 20:26
desc:
'''


IPHeader="457400d6e4e3400067063c34348bfafdc0a80165"
IPHeader="45"
IPHeader_hex = bytes.fromhex(IPHeader)
IPHeader_hex = IPHeader & "1111"
print(IPHeader_hex  )
