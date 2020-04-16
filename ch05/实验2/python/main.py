# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/4/16 18:12
desc:
'''

# 初始化
f = open('./routers.txt', 'r')
distance = f.read().split('\n')
distance = [dis for dis in distance if dis != '']
graph = []
for item in distance:
    item = item.split(" ")
    item = [int(i) for i in item]
    graph.append(item)
node_num = len(graph)

def self_table(current_node):
    dis = [99]*node_num
    table = [1]*node_num
    for i,item in enumerate(graph[current_node]):
        if item <99:
            dis[i] = item
            table[i] = [current_node, i]

    for i in range(node_num):
        if i == current_node:
            continue
        for j,item in enumerate(graph[i]):
            if item + dis[i] < dis[j]:
                dis[j] = item+dis[i]
                table[j] = table[i]
        print(f"当前路由{current_node}的最短路结果",dis)

    ans = [current_node]
    for i,item in enumerate(table):
        if item == [current_node,current_node]:
            continue
        else:
            ans.append([i,item])
    return ans

def main():
    router_table = []
    for i in range(node_num):
        print(f" 路由{i}")
        ans = self_table(i)
        print(ans)
        router_table.append(ans)
        print("-------------")

if __name__ == '__main__':
    main()


