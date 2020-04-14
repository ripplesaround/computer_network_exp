# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/4/9 14:17
desc:
'''

from winpcapy import WinPcapDevices
from winpcapy import WinPcapUtils

import dpkt
from dpkt.compat import compat_ord
import time
import datetime
print("---------------------------------")
print("当前的网络配置器列表")
with WinPcapDevices() as devices:
    for device in devices:
        print(device.name, device.description, device.flags ,device.addresses.contents.netmask.contents.sa_family)

mac_address = [b"\x00\x50\x56\xc0\x00\x01",[],[],[],[],[],b"\xf4\x96\x34\x7f\xa1\xa7"]
def mac_addr(address):
    """Convert a MAC address to a readable/printable string

       Args:
           address (str): a MAC address in hex form (e.g. '\x01\x02\x03\x04\x05\x06')
       Returns:
           str: Printable/readable MAC address
    """
    return ':'.join('%02x' % compat_ord(b) for b in address)
#
print("---------------------------------")

# Example Callback function to parse IP packets
def packet_callback(win_pcap, param, header, pkt_data):
    # Assuming IP (for real parsing use modules like dpkt)
    ip_frame = pkt_data[14:]
    # Parse ips
    src_ip = ".".join([str(b) for b in ip_frame[0xc:0x10]])
    dst_ip = ".".join([str(b) for b in ip_frame[0x10:0x14]])
    print("%s -> %s" % (src_ip, dst_ip))

# WinPcapUtils.capture_on("*Ethernet*", packet_callback)

flagA = 0
flagB = 6

# print("---------------------------------")
# print("选择捕获适配器的编号")
# flagA = int(input(">>>"))
# print(f"正在捕获{flagA}号网络适配器")
# flagB = int(input(">>>"))
# print(f"正在捕获{flagB}号网络适配器")
# print("---------------------------------")



hostA = mac_address[flagA]
hostB = mac_address[flagB]
boardcast = b"\xff\xff\xff\xff\xff\xff"

Add_table = []


s = b"\xf4\x96\x34\x7f\xa1\xa7" \
    b"\x00\x50\x56\xc0\x00\x01" \
    b"\x08\x00" \
    b"\x45\x74\x00(\xb3 @\x00.\x06m|" \
    b"\xc0\xa8\xba\x01" \
    b"\xc0\xa8\x01i" \
    b"\x01\xbb\x11\x84\xa4\xbf0\xef\xeaI*\xa1P\x11\x03\x94\x82\xa7\x00\x00'"

eth = dpkt.ethernet.Ethernet(s)

def rec(eth):
    # 无论后续如何处理，都应该先更新表
    current_port = backward_learning(eth)
    dst_port = find_port(eth)
    if dst_port>-1:
        if dst_port == current_port:
            print("此帧丢弃")
            return
        pass
    else:
        print("---泛洪")


def find_port(eth):
    dst_port = -1
    for item in Add_table:
        if eth.src == item[1]:
            dst_port = item[0]
            break
    return dst_port





def backward_learning(eth):
    print("---网桥收到一帧")
    print("the source address of the ethernet packet:", mac_addr(eth.src))
    print("the destination address of the ethernet packet:", mac_addr(eth.dst))
    # print(eth.dst)
    # print(mac_address[flagB])
    current_port = -1
    if eth.src == hostA:
        current_port = 0
    elif eth.src == hostB:
        current_port = 1
    temp = []
    temp.append(current_port)
    temp.append(eth.src)
    temp.append(time.time())
    return current_port

def cache_clean():
    print("---清空表项")
    Add_table.clear()


backward_learning(eth)
print(Add_table)