from winpcapy import WinPcapDevices
from winpcapy import WinPcapUtils

import dpkt
from dpkt.compat import compat_ord
import time
import datetime
print("---------------------------------")
print("当前的网络配置器列表")
list_device = WinPcapDevices.list_devices()
for i,device in enumerate(list_device):
    print(i,device)


print("---------------------------------")
print("选择捕获适配器的编号")
flag = int(input(">>>"))
print(f"正在捕获{flag}号网络适配器")
print("---------------------------------")

def mac_addr(address):
    """Convert a MAC address to a readable/printable string

       Args:
           address (str): a MAC address in hex form (e.g. '\x01\x02\x03\x04\x05\x06')
       Returns:
           str: Printable/readable MAC address
    """
    return ':'.join('%02x' % compat_ord(b) for b in address)

def packet_callback(win_pcap, param, header, pkt_data):
    print("---------分析数据包----------")
    print("------ethernet packet-------")
    eth = dpkt.ethernet.Ethernet(pkt_data)

    print("the source address of the ethernet packet:",mac_addr(eth.src))
    print("the destination address of the ethernet packet:", mac_addr(eth.dst))
    # more types see http://www.iana.org/assignments/ethernet-numbers/ethernet-numbers.xhtml
    if eth.type ==2048: # 16进制：0x0800
        print("the Ethernet type:",eth.type,'means: IPv4')
    elif eth.type == 34525: #0x86DD
        print("the Ethernet type:",eth.type, 'means: IPv6')


    print("----------IP packet---------")

    # 判断是否为IP数据报
    if not isinstance(eth.data, dpkt.ip.IP):
        print("Non IP packet type not supported ", eth.data.__class__.__name__)
        return
    # 抓IP数据包
    packet = eth.data
    # 取出分片信息
    df = bool(packet.off & dpkt.ip.IP_DF)
    mf = bool(packet.off & dpkt.ip.IP_MF)
    offset = packet.off & dpkt.ip.IP_OFFMASK

    # 输出数据包信息：time,src,dst,protocol,length,ttl,df,mf,offset,checksum
    output1 = {'time':time.strftime('%Y-%m-%d %H:%M:%S',(time.localtime()))}
    output2 = {'src':'%d.%d.%d.%d'%tuple(packet.src) , 'dst':'%d.%d.%d.%d'%tuple(packet.dst)}
    output3 = {'protocol':packet.p, 'len':packet.len, 'ttl':packet.ttl}
    output4 = {'df':df, 'mf':mf, 'offset':offset, 'checksum':packet.sum}
    print()
    print(output1)
    print(output2)
    print(output3)
    print(output4)
    print("----------------------------\n")

for i,key in enumerate(list_device.keys()):
    if i== flag:
        WinPcapUtils.capture_on_device_name(key, callback=packet_callback)
        break