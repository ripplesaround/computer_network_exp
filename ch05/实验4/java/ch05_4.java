/*
 * @Author: Devil 
 * @Date: 2020-05-30 22:20:07 
 * @Last Modified by: Devil
 * @Last Modified time: 2020-05-31 00:41:36
 */
public class ch05_4 {
    public static String intToBinary(int i, int bitNum){
        String binaryStr = Integer.toBinaryString(i);
        while(binaryStr.length() < bitNum){
            binaryStr = "0"+binaryStr;
        }
        return binaryStr;
    }
    public static void checksums(String header)
    {    	
        System.out.println("Version: "+Integer.parseInt(header.substring(0,1),16));
        System.out.println("Header Length: "+Integer.parseInt(header.substring(1,2),16));
        System.out.println("Type of Service: "+Integer.parseInt(header.substring(2,4),16));
        System.out.println("Total Length: "+Integer.parseInt(header.substring(4,8),16));
        System.out.println("Flag: "+Integer.parseInt(header.substring(8,12),16));
        String binaryStr = intToBinary(Integer.parseInt(header.substring(12,16), 16),16);
        System.out.println("DF: "+binaryStr.charAt(1));
        System.out.println("MF: "+binaryStr.charAt(2));
        System.out.println("Fragment Offset: "+Integer.parseInt(binaryStr.substring(3,16),2));
        System.out.println("TTL: "+Integer.parseInt(header.substring(16,18),16));
        System.out.println("Protocol: "+Integer.parseInt(header.substring(18,20),16));
        System.out.println("Header Checksum: "+Integer.parseInt(header.substring(20,24),16));
        System.out.printf("Source Addresses: %d.%d.%d.%d\n",Integer.parseInt(header.substring(24,26),16),Integer.parseInt(header.substring(26,28),16),Integer.parseInt(header.substring(28,30),16),Integer.parseInt(header.substring(30,32),16));
        System.out.printf("Destination Addresses: %d.%d.%d.%d\n",Integer.parseInt(header.substring(32,34),16),Integer.parseInt(header.substring(34,36),16),Integer.parseInt(header.substring(36,38),16),Integer.parseInt(header.substring(38,40),16));
        Integer total = 0;
        for(int i = 0; i < 10; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                total = total + (int)Math.pow(16,3-j)*Integer.parseInt(header.substring(4*i+j,4*i+j+1),16);
            }
        }
        Integer x = (int)Math.pow(16,4);
        Integer left = total - (total % x);
        Integer right = total % x;
        left = (int)left/x;
        Integer checksum = right +left;
        System.out.println(Integer.toBinaryString(checksum)+"b");
        checksum += (int)Math.pow(2,16);
        System.out.println(Integer.toBinaryString(checksum)+"b");
    }
	public static void main(String[] args) {
		checksums("4500003CB53040008006E251C0A80168D83AC8EE");
	}

}
