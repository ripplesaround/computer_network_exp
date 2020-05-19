import numpy as np
 

with open('C:\\Users\\Dell\\Desktop\\config.txt',"r") as f:    #设置文件对象
    data = np.loadtxt("C:\\Users\\Dell\\Desktop\\config.txt") 
    # print(data)
    print("---------------------------------")
    routerNum = data.shape[0]
    # update()


path = np.zeros((routerNum,routerNum))
datan = np.zeros((routerNum,routerNum)) # 注意不能直接datan=data！否则二者会同步变化！
# 路由路径初始化
def pathInit():
    for i in range(0,routerNum):
        for j in range(0,routerNum):
            datan[i][j] = data[i][j] #顺便ini一下datan
            if data[i][j]>0 and data[i][j]<16:
                path[i][j] = j
            else:
                path[i][j] = -1

def update():
    for i in range(0,routerNum):
        for j in range(0,routerNum):
            data[i][j] = datan[i][j]

#打印路由表函数
def printRouterTable():
    for i in range(0,routerNum):
        print("The router{}'s router table is:".format(i))
        print("---Routers Reachable:---")
        for j in range(0,routerNum):
            if i == j:
                continue
            elif data[i][j]<16 and data[i][j]>0:
                print("router{}, next router:{}, distance:{}".format(j,path[i][j],data[i][j]))
            else:
                continue
        print("---Routers Unreachable:---")
        for j in range(0,routerNum):
            if i == j:
                continue
            elif data[i][j]<16:
                continue
            else:
                print("router{}".format(j))
        print("--------------------------------------------------------")
    
# 根据邻居节点更新自身数据
def checkNeighbor(i,j):
    tag = 0
    for k in range(0,routerNum):
        if k == i or k == j or data[j][k]<0:
            continue
        # 不可用情况
        elif data[j][k] >= 16 and path[j][k]>=0:
            for t in range(0,routerNum):
                if t==i or t==j:
                    continue
                # 经该邻居发出的信号
                elif path[i][t] == j and path[j][t] == k:
                    path[i][t] = -1
                    datan[i][t] = 99
                    path[j][t] = -1
                    datan[j][t] = 99
                    print("spec")
                    tag = 1
        else:
            old_distance = data[i][k]
            new_distance = data[i][j]+data[j][k]
            if old_distance>new_distance:
                path[i][k] = path[i][j]
                datan[i][k] = new_distance
            # 还要考虑不可用情况！
                tag = 1
    if tag==1:
        return 1
    else:
        return 0

# DV算法
def DV():
    flag = 1
    count = 0
    while(flag>0):
        flag = flag -1
        for i in range(0,routerNum):
            for j in range(0,routerNum):
                if i == j:
                    continue
                elif data[i][j] <16 and data[i][j]>0:
                    flag = flag + checkNeighbor(i,j)
                else:
                    continue
        # print("--------------------------------------This is one time!")
        update()
        # print(datan)
        count = count + 1
    # 自检也算一跳
    
    return count

def main():
    pathInit()
    # print("初始路由表:")
    # printRouterTable()
    ini = DV()
    # print("从初始状态稳定到最终态用时:{},最终路由表为:".format(ini))
    # printRouterTable()
    # print("最终态：")
    # print(data)
    # print(path)
    inp = input("请输入要优化或中断的路径上的起始和结束路由器，格式:a b ")
    # print(type(inp))
    x = int(inp[0])
    y = int(inp[2])
    if x == y or x>=routerNum or x<0:
        print("请输入合适的路由器序号！范围：0-{}".format(routerNum-1))
    else:
        print("原拓部结构：{}".format(data))
        data[x][y] = 1
        goodNews = DV()
        print("好消息传播至稳定用时：{}".format(goodNews))
        print("更新的拓部结构：{}".format(data))
        data[x][y] = 99
        badNews = DV()
        print("坏消息传播至稳定用时：{}".format(badNews))
        print("更新的拓部结构：{}".format(data))


main()