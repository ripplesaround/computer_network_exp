from winpcapy import WinPcapUtils
from winpcapy import WinPcapDevices

print("---------------------------------")
print("当前的网络配置器列表")
list_device = WinPcapDevices.list_devices()
with WinPcapDevices() as devices:
    for i,device in enumerate(devices):
        print(i,device.name, device.description)


print("---------------------------------")
print("选择捕获适配器的编号")
flag = int(input(">>>"))
print(f"正在捕获{flag}号网络适配器")
print("---------------------------------")

# Example Callback function to parse IP packets
def packet_callback(win_pcap, param, header, pkt_data):
    # Assuming IP (for real parsing use modules like dpkt)
    ip_frame = pkt_data[14:]
    # Parse ips
    src_ip = ".".join([str(ord(b)) for b in ip_frame[0xc:0x10]])
    dst_ip = ".".join([str(ord(b)) for b in ip_frame[0x10:0x14]])
    print("%s -> %s" % (src_ip, dst_ip))

for i,key in enumerate(list_device.keys()):
    if i== flag:
        WinPcapUtils.capture_on_device_name(key, callback=packet_callback)
        break