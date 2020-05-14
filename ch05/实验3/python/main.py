# coding=utf-8

'''
Author: ripples
Email: ripplesaround@sina.com

date: 2020/5/14 20:15
desc:
'''
IPheader = 20
def fragment(BigIPTotalLen = 4000,ID = 666,MTU = 1500):
    print("---------------------------------------------")
    print("  Fragment")
    print("MTU: ",MTU)
    MTU_ess = MTU-IPheader

    MF = 0
    DF = 0
    offset = 0
    frag_num =int( (BigIPTotalLen-IPheader)/MTU)
    if frag_num*MTU<BigIPTotalLen:
        frag_num+=1
    print("frag_num: ",frag_num)
    print("%-20s%-20s%-20s%-10s%-10s%-10s" % ("", "total length", "identification", "MF", "DF", "offset"))
    print("%-20s%-20s%-20s%-10s%-10s%-10s" % ("Original IP", BigIPTotalLen, ID, MF, DF, offset))
    totallen = BigIPTotalLen
    if frag_num>1:
        MF = 1
        totallen = (MTU_ess//8)*8
        BigIPTotalLen -=(IPheader+totallen)


    for i in range(frag_num):
        if i==frag_num-1:
            MF=0
        print("%-20s%-20s%-20s%-10s%-10s%-10s" % ("frag "+ str(i+1), totallen+IPheader, ID, MF, DF, offset))
        offset += int(totallen / 8)
        if BigIPTotalLen > totallen:
            totallen = (MTU_ess//8)*8
            BigIPTotalLen -= totallen
        else:
            totallen = BigIPTotalLen
            BigIPTotalLen -= BigIPTotalLen



def package(frag_num = 3,totallen = [1500,1500,1040],ID = [888,888,888],FragMF = [1,1,0],Fragoffset = [0,185,370]):
    print("---------------------------------------------")
    print("  package")
    print("frag_num: ", frag_num)
    print("%-20s%-20s%-20s%-10s%-10s%-10s" % ("", "total length", "identification", "MF", "DF", "offset"))
    DF = 0
    MF = 0
    for i in range(frag_num):
        print("%-20s%-20s%-20s%-10s%-10s%-10s" % ("frag " + str(i + 1), totallen[i], ID[i], FragMF[i], DF, Fragoffset[i]))
        totallen[i] -= IPheader
    BigIPtotallen = sum(totallen) + IPheader
    offset = 0
    print("%-20s%-20s%-20s%-10s%-10s%-10s" % ("Packaged IP", BigIPtotallen, ID[0], MF, DF, offset))

if __name__ == '__main__':
    fragment()
    package()