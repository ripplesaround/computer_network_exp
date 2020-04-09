'''
dkpt的文档：http://www.commercialventvac.com/dpkt.html
'''

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
    # 协议17：UDP，4：特殊IP报，41：IPv6，6:tcp
    output3 = {'protocol':packet.p, 'ttl':packet.ttl}
    # 注意flag只有两位有意义，mf和df mf：还有分片，df不能分片
    output4 = {'df':df, 'mf':mf, 'offset':offset, 'checksum':packet.sum}
    output5 = {'the length of the internet header':packet.hl}
    output6 = {'the version of IP':packet.v}
    output7 = {'Terms of Service':packet.tos}
    output8 = {'total length':packet.len}
    output10 = {'identification':packet.id}
    print(output6)
    print(output5)
    print(output7)
    print(output8)
    print(output10)
    print(output4)
    print(output1)
    print(output3)
    print(output2)


    if packet.p == 17:
        if(len(packet.data)>0):
            print("this packet is an UDP packet")
            print("---------UDP packet---------")
            udp = packet.data
            output1 = {"The source port":udp.sport,"The destination port":udp.dport}
            output2 = {"length": udp.ulen}
            output3 = {"checksum":udp.sum}
            print(output1)
            print(output2)
            print(output3)

    elif packet.p == 6:
        print("this packet is a TCP packet")
        print("---------TCP packet---------")
        tcp = packet.data
        output1 = {"The source port":tcp.sport,"The destination port":tcp.dport}
        output2 = {"seq":tcp.seq}
        output3 = {"ack_seq":tcp.ack}
        output4 = {"offset":tcp.off}
        output5 = {"checksum":tcp.sum}
        fin_flag = (tcp.flags & dpkt.tcp.TH_FIN)
        syn_flag = (tcp.flags & dpkt.tcp.TH_SYN)
        rst_flag = (tcp.flags & dpkt.tcp.TH_RST)
        psh_flag = (tcp.flags & dpkt.tcp.TH_PUSH)
        ack_flag = (tcp.flags & dpkt.tcp.TH_ACK)
        urg_flag = (tcp.flags & dpkt.tcp.TH_URG)
        ece_flag = (tcp.flags & dpkt.tcp.TH_ECE)
        cwr_flag = (tcp.flags & dpkt.tcp.TH_CWR)
        output6 = {"fin_flag":fin_flag,
                   "syn_flag":syn_flag,
                   "rst_flag":rst_flag,
                   "psh_flag":psh_flag,
                   "ack_flag":ack_flag,
                   "urg_flag":urg_flag,
                   "ece_flag":ece_flag,
                   "cwr_flag":cwr_flag
                   }

        print(output1)
        print(output2)
        print(output3)
        print(output4)
        print(output5)
        print(output6)

        print("---------HTTP packet--------")
        try:
            http_req = dpkt.http.Request(tcp.data)
            print('HTTP request: %s\n' % repr(http_req))
        except (dpkt.dpkt.NeedData, dpkt.dpkt.UnpackError):
            print("nothing")



    print("----------------------------\n")

for i,key in enumerate(list_device.keys()):
    if i== flag:
        WinPcapUtils.capture_on_device_name(key, callback=packet_callback)
        break