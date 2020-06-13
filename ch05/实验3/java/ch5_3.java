public class ch5_3 {
    static int IPheader = 20;
    public static void fragment(int BigIPTotalLen,int ID,int MTU){
        System.out.println("---------------------------------------------");
        System.out.println("  Fragment");
        System.out.println("MTU: "+MTU);
        int MTU_ess = MTU-IPheader;
        int MF = 0;
        int DF = 0;
        int offset = 0;
        int frag_num =(int)Math.ceil((double)(BigIPTotalLen-IPheader)/MTU);
        System.out.println(frag_num);
        System.out.println("frag_num: "+frag_num);
        System.out.printf("%-20s%-20s%-20s%-10s%-10s%-10s\n" , "", "total length", "identification", "MF", "DF", "offset");
        System.out.printf("%-20s%-20s%-20s%-10s%-10s%-10s\n" , "Original IP", BigIPTotalLen, ID, MF, DF, offset);
        int totallen = BigIPTotalLen;
        if (frag_num > 1)
        {
            MF = 1;
            totallen = (MTU_ess/8)*8;
            BigIPTotalLen -=(IPheader+totallen);
        }
        for(int i=0;i<frag_num;i++){
            if (i==frag_num-1){
                MF=0;
            } 
            System.out.printf("%-20s%-20s%-20s%-10s%-10s%-10s\n" , "frag "+ String.valueOf(i+1), totallen+IPheader, ID, MF, DF, offset);
            offset += totallen / 8;
            if (BigIPTotalLen > totallen){
                totallen = (MTU_ess/8)*8;
                BigIPTotalLen -= totallen;
            }
            else{
                totallen = BigIPTotalLen;
                BigIPTotalLen -= BigIPTotalLen;
            }      
        }
        
    }
    public static void repackage(int frag_num,int[] totallen, int[] ID,int[] FragMF,int[] Fragoffset){
        System.out.println("---------------------------------------------");
        System.out.println("  package");
        System.out.println("frag_num: "+frag_num);
        System.out.printf("%-20s%-20s%-20s%-10s%-10s%-10s\n" ,"", "total length", "identification", "MF", "DF", "offset");
        int DF = 0;
        int MF = 0;
        for(int i = 0;i<frag_num;i++){
            System.out.printf("%-20s%-20s%-20s%-10s%-10s%-10s\n" ,"frag " + String.valueOf(i+1), totallen[i], ID[i], FragMF[i], DF, Fragoffset[i]);
            totallen[i] -= IPheader;
        }
        int sum = 0;
        for (int i = 0; i < totallen.length; i++) {
            sum+=totallen[i];
         } 
        int BigIPtotallen = sum + IPheader;
        int offset = 0;
        System.out.printf("%-20s%-20s%-20s%-10s%-10s%-10s" ,"Packaged IP", BigIPtotallen, ID[0], MF, DF, offset);   
    }
    public static void main(String[] args) throws Exception{
        fragment(4000,666,1500);
        repackage(3,new int[]{1500,1500,1040},new int[]{888,888,888},new int[]{1,1,0},new int[]{0,185,370});
    }
}
