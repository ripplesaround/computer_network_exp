# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/4/22 19:13
desc:
'''
def str2bit(ip_add):
    """
    将点分十进制字符串转换为二进制字符串
    :param ip_add: 点分十进制ip地址或点分十进制的子网掩码
    :return: 二进制字符串
    """
    if ip_add == '0':
        ip_add = '0.0.0.0'
    ip_add = ip_add.split('.')
    ans = ''
    for i in range(len(ip_add)):
        temp = bin(int(ip_add[i]))[2:]
        while len(temp)<8:
            temp = '0' + temp
        ans += temp
    return ans

def int2bit(ip_add):
    """
    按要求转换成二进制
    :param ip_add: /num的那个整数
    :return: 二进制的字符串
    """
    ip_add = int(ip_add)
    ans = ''
    for i in range(ip_add):
        ans += '1'
    for i in range(32-ip_add):
        ans += '0'
    return ans

class table_item:
    def __init__(self):
        self.next_hop = None
        self.ip = None
        self.subnet_mask = None

# 初始化
f = open('./config.txt', 'r')
distance = f.read().split('\n')
distance = [dis for dis in distance if dis != '']
table_len = len(distance)-1
distance = distance[1:]
router_table = []
for i in range(table_len):
    router_table.append(table_item())
# router_table = [table_item(),table_item(),table_item(),table_item(),table_item(),table_item()]
for i,item in enumerate(distance):
    item = item.split(' ')
    router_table[i].next_hop = item[1]
    item = item[0].split('/')
    router_table[i].ip = str2bit(item[0]).encode('utf-8')
    router_table[i].subnet_mask = int2bit(item[1]).encode('utf-8')

print("-------------------------------")
print("当前路由表：")
for i in range(table_len):
    print("%-40s%-40s%-30s"%(router_table[i].ip,router_table[i].subnet_mask,router_table[i].next_hop))
print("-------------------------------")

dst_ip = input("请输入目的IP地址>>>")
print("-------------------------------")
temp = bin(int(router_table[0].ip,2) & int(router_table[0].subnet_mask,2))

temp = []

for i in range(table_len):
    dst_subnet =  bin(int(str2bit(dst_ip),2) & int(router_table[i].subnet_mask,2))[2:].encode('utf-8')
    if dst_subnet == router_table[i].ip:
        # print("%-10s%-30s"%("next hop",router_table[i].next_hop))
        temp.append(i)
        print(f"第{i}行匹配")
    else:
        print(f"第{i}行不匹配")
print("-------------------------------")
if len(temp) == 0:
    print("用默认路由进行转发")
    print("%-10s%-30s" % ("next hop", router_table[table_len-1].next_hop))
elif len(temp) == 1:
    print("%-10s%-30s" % ("next hop", router_table[temp[0]].next_hop))
else:
    current_max = router_table[temp[0]].subnet_mask
    current_max_index = temp[0]
    for item in temp:
        if int(router_table[item].subnet_mask)>int(current_max):
            current_max_index = item
            current_max = router_table[item].subnet_mask
    print("%-10s%-30s" % ("next hop", router_table[current_max_index].next_hop))