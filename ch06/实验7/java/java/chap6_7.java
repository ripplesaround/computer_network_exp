/*
 * @Author: Devil 
 * @Date: 2020-05-11 23:03:40 
 * @Last Modified by: Devil
 * @Last Modified time: 2020-06-11 23:30:07
 */

public class chap6_7 {
    static Double[] RTT={1.5,2.5};
    static Double Alpha=0.125;
    static Double Beita=0.25;
    public static void main(String[] args){
        Double RTTs = RTT[0];
        Double RTTd = RTT[0]/2;
        Double RTO = RTTs + 4 * RTTd;
        System.out.println(RTO);
        for(int i = 1; i < RTT.length; i++){
            RTTs = (1 - Alpha) * RTTs + Alpha * RTT[i];
            RTTd = (1 - Beita) * RTTd + Beita * Math.abs(RTT[i] - RTTs);
            RTO = RTTs + 4 * RTTd;
            System.out.println(RTO);
        }
    }
}