# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/4/10 1:40
desc:
实验中没有要求真正的实现以太网之间的发包，所有我们可以只用打包而不用发，更不用检测包有没有，只需要
'''

import binascii
from winpcapy import WinPcapUtils
import dpkt
from dpkt.compat import compat_ord

def mac_addr(address):
    """Convert a MAC address to a readable/printable string

       Args:
           address (str): a MAC address in hex form (e.g. '\x01\x02\x03\x04\x05\x06')
       Returns:
           str: Printable/readable MAC address
    """
    return ':'.join('%02x' % compat_ord(b) for b in address)


# s = (b'\x00\xb0\xd0\xe1\x80\x72'    # eth.dst
#      b'\x00\x11\x24\x8c\x11\xde'    # eth.src
#      b'\x08\x00'                    # eth.type
#      b'\x60\x00\x00\x00'
#      b'\x00\x28\x06\x40\xfe\x80\x00\x00'
#      b'\x00\x00\x00\x00'
#      b'\x02\x11\x24\xff'
#      b'\xfe\x8c\x11\xde\xfe\x80\x00\x00\x00\x00\x00\x00\x02\xb0\xd0\xff\xfe\xe1\x80\x72'
#      b'\xcd\xd3\x00\x16\xff\x50\xd7\x13\x00\x00\x00\x00\xa0\x02\xff\xff\x67\xd3'
#      b'\x00\x00\x02\x04\x05\xa0\x01\x03\x03\x00\x01\x01\x08\x0a\x7d\x18\x3a\x61'
#      b'\x00\x00\x00\x00')

#eth.dst
#eth.src
#eth.type

# s = b"\xf4\x96\x34\x7f\xa1\xa7" \
#     b"\x74\x05\xa5\x1b\x5d\xa2" \
#     b"\x08\x00" \
#     b"\x45\x74\x00(\xb3 @\x00.\x06m|" \
#     b"'\x9cB\x12" \
#     b"\xc0\xa8\x01i" \
#     b"\x01\xbb\x11\x84\xa4\xbf0\xef\xeaI*\xa1P\x11\x03\x94\x82\xa7\x00\x00'"

# b"\x00\x50\x56\xc0\x00\x01"
# \xf4\x96\x34\x7f\xa1\xa7
s = b"\x00\x50\x56\xc0\x00\x01" \
    b"\xf4\x96\x34\x7f\xa1\xa7" \
    b"\x08\x00" \
    b"\x45\x74\x00(\xb3 @\x00.\x06m|" \
    b"\xc0\xa8\xba\x01" \
    b"\xc0\xa8\x01i" \
    b"\x01\xbb\x11\x84\xa4\xbf0\xef\xeaI*\xa1P\x11\x03\x94\x82\xa7\x00\x00'"


eth = dpkt.ethernet.Ethernet(s)
packet = eth.data
print(packet)
print(eth.src)
print("the source address of the ethernet packet:",mac_addr(eth.src))
print("the destination address of the ethernet packet:", mac_addr(eth.dst))
# more types see http://www.iana.org/assignments/ethernet-numbers/ethernet-numbers.xhtml
if eth.type ==2048: # 16进制：0x0800
    print("the Ethernet type:",eth.type,'means: IPv4')
elif eth.type == 34525: #0x86DD
    print("the Ethernet type:",eth.type, 'means: IPv6')
# frame = eth.get_type(2048)
# frame.dst =  bytes.fromhex('112233')
#
# print(frame.pack_hdr())
# print(mac_addr(frame.dst))
# Send the packet (ethernet frame with an arp request) on the interface
# WinPcapUtils.send_packet("*Ethernet*", packet.decode('hex'))
WinPcapUtils.send_packet("*Ethernet*",s)