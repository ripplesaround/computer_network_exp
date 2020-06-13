/*
 * @Author: Devil 
 * @Date: 2020-06-12 00:11:38 
 * @Last Modified by: Devil
 * @Last Modified time: 2020-06-12 00:43:50
 */

public class chap6_6 {
    static Integer MSS=1024;
    static Integer Threshold=32768;
    static Integer TriACKRound=16;
    static Integer TimeoutRound=22;
    static Integer EndRound=26;
    public static void main(String[] args){
        Integer CongWin = 0;
        System.out.println("MSS: "+ MSS + "     Init_Threshold: " + Threshold);
        for(int i = 1; i <= EndRound; i++){
            if(i == TriACKRound){
                Threshold = (int)(CongWin / 2);
                CongWin = Threshold;
            }else if(i == TimeoutRound){
                Threshold = (int)(CongWin / 2);
                CongWin = MSS;
            }else{
                if(CongWin >= Threshold){
                    CongWin += (int)(MSS * MSS / CongWin);
                }else{
                    CongWin += MSS;
                }
            }
            System.out.printf("round: %2d     Congwin: %5d\n", i, CongWin);
        }
    }
}