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
import threading
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

# flagA = 0
# flagB = 6

print("---------------------------------")
print("选择捕获适配器的编号")
flagA = int(input(">>>"))
print(f"正在捕获{flagA}号网络适配器")
flagB = int(input(">>>"))
print(f"正在捕获{flagB}号网络适配器")
print("---------------------------------")



hostA = mac_address[flagA]
hostB = mac_address[flagB]
boardcast = b"\xff\xff\xff\xff\xff\xff"

Add_table = []


s1 = b"\xf4\x96\x34\x7f\xa1\xa7" \
    b"\x00\x50\x56\xc0\x00\x01" \
    b"\x08\x00" \
    b"\x45\x74\x00(\xb3 @\x00.\x06m|" \
    b"\xc0\xa8\xba\x01" \
    b"\xc0\xa8\x01i" \
    b"\x01\xbb\x11\x84\xa4\xbf0\xef\xeaI*\xa1P\x11\x03\x94\x82\xa7\x00\x00'"

s2 =  b"\x00\x50\x56\xc0\x00\x01" \
    b"\xf4\x96\x34\x7f\xa1\xa7" \
    b"\x08\x00" \
    b"\x45\x74\x00(\xb3 @\x00.\x06m|" \
    b"\xc0\xa8\xba\x01" \
    b"\xc0\xa8\x01i" \
    b"\x01\xbb\x11\x84\xa4\xbf0\xef\xeaI*\xa1P\x11\x03\x94\x82\xa7\x00\x00'"

eth1 = dpkt.ethernet.Ethernet(s1)
eth2 = dpkt.ethernet.Ethernet(s2)

def flooding():
    pass

def tran():
    pass

def rec(eth):
    # 无论后续如何处理，都应该先更新表
    current_port = backward_learning(eth)
    dst_port = find_port(eth)
    if dst_port>-1:
        # print(dst_port,current_port)
        if dst_port == current_port:
            print("此帧丢弃")
            return
        print("---转发")
        print(f"从{current_port}到{dst_port}端口")
        tran()
    else:
        print("---泛洪")
        flooding()
        return

def find_port(eth):
    dst_port = -1
    for item in Add_table:
        if eth.dst == item[1]:
            dst_port = item[0]
            break
    return dst_port

def backward_learning(eth):
    print("\n-----网桥收到一帧")
    print("the source address of the ethernet packet:", mac_addr(eth.src))
    print("the destination address of the ethernet packet:", mac_addr(eth.dst))
    # print(eth.dst)
    # print(mac_address[flagB])
    current_port = -1
    if eth.src == hostA:
        current_port = 0
    elif eth.src == hostB:
        current_port = 1

    for item in Add_table:
        if current_port== item[0]:
            return current_port
    temp = []
    temp.append(current_port)
    temp.append(eth.src)
    temp.append(time.time())
    Add_table.append(temp)
    return current_port

class rec_eth:
    def __init__(self):
        self._running = True

    def terminate(self):
        self._running = False

    def run(self,eth):
        rec(eth)

c1 = rec_eth()

class table_clean:
    def __init__(self):
        self._running = True

    def terminate(self):
        self._running = False

    def run(self,eth):
        global Add_table
        current_time = time.time()
        while(1):
            now_time = time.time()
            if now_time-current_time>1:
                # 扫描的时间是1，超时的时间是2
                print("\n-----扫描并清除超时表项")
                temp = []
                for item in Add_table:
                    if now_time - item[2] > 2:
                        temp.append(item)
                Add_table.clear()
                Add_table = temp
                current_time = now_time

c1 = rec_eth()

c2 = table_clean()
t3 = threading.Thread(target=c2.run, args=(eth1,))
t3.start()

while(1):
    start_time = time.time()
    flag = True
    while (1):
        now = time.time()
        # print(now,start_time)
        if now-start_time > 1 and flag:
            t1 = threading.Thread(target=c1.run, args=(eth1,))
            t1.start()
            flag = False
        if now-start_time>=1.5:
            t2 = threading.Thread(target=c1.run, args=(eth2,))
            t2.start()
            break

    print(Add_table)