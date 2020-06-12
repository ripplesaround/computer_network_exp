public class TCPchecknum {
    static String tcpsegment = "ff7c2712a0627b9b2d3d9b00501827f9a38f0000777364";
    static String sourceip = "127.0.0.1";
    static String destip = "127.0.0.1";
    public static String intToBinary(int i, int bitNum){
        String binaryStr = Integer.toBinaryString(i);
        while(binaryStr.length() < bitNum){
            binaryStr = "0"+binaryStr;
        }
        return binaryStr;
    }
    public static String parseByte2HexStr(byte buf[]) {
        StringBuffer sb = new StringBuffer();
        for (int i = 0; i < buf.length; i++) {
            String hex = Integer.toHexString(buf[i] & 0xFF);
            if (hex.length() == 1) {
                hex = '0' + hex;
            }
            sb.append(hex.toUpperCase());
        }
        return sb.toString();
    }
    public static String makeChecksum(String data) {
        if (data == null || data.equals("")) {
         return "";
        }
        int total = 0;
        int len = data.length();
        int num = 0;
        while (num < len) {
            String s = data.substring(num, num + 4);
            total += Integer.parseInt(s, 16);
            num = num + 4;
        }
        String hex = Integer.toHexString(total);
        len = hex.length();
        if(len < 4) {
            hex = Integer.toHexString(65535 - Integer.parseInt(hex, 16));
            hex = "0" + hex;
        }
        if(len > 4){
            //System.out.println(Integer.parseInt(hex.substring(0,len-4), 16));
            //System.out.println(Integer.parseInt(hex.substring(len-4,len), 16));
            hex = Integer.toHexString(65535 - Integer.parseInt(hex.substring(0,len-4), 16) - Integer.parseInt(hex.substring(len-4,len), 16));
        }
        return hex;
    }
    public static void main(String[] args){
        String pseudo_head = "";
        String[] tmp = sourceip.split("\\.");
        String x = intToBinary(Integer.parseInt(tmp[0]),8)+intToBinary(Integer.parseInt(tmp[1]),8)+intToBinary(Integer.parseInt(tmp[2]),8)+intToBinary(Integer.parseInt(tmp[3]),8);
        System.out.printf("Source Addresses: %s\n",x);
        pseudo_head += (String.format("%02X",Integer.parseInt(tmp[0]))+String.format("%02X",Integer.parseInt(tmp[1]))+String.format("%02X",Integer.parseInt(tmp[2]))+String.format("%02X",Integer.parseInt(tmp[3])));
        tmp = destip.split("\\.");
        x = intToBinary(Integer.parseInt(tmp[0]),8)+intToBinary(Integer.parseInt(tmp[1]),8)+intToBinary(Integer.parseInt(tmp[2]),8)+intToBinary(Integer.parseInt(tmp[3]),8);
        System.out.printf("Destination Addresses: %s\n",x);
        pseudo_head += (String.format("%02X",Integer.parseInt(tmp[0]))+String.format("%02X",Integer.parseInt(tmp[1]))+String.format("%02X",Integer.parseInt(tmp[2]))+String.format("%02X",Integer.parseInt(tmp[3])));
        tmp = destip.split("\\.");
        System.out.println("Retain: 00000000");
        pseudo_head += "00";
        System.out.println("Transport layer protocol: " + intToBinary(6,8));
        pseudo_head += String.format("%02X",6);
        System.out.println("TCP Length: " + intToBinary(tcpsegment.length()/2,16));
        pseudo_head += String.format("%04X",tcpsegment.length()/2);
        tcpsegment =  tcpsegment.substring(0,32)+"0000"+tcpsegment.substring(36,tcpsegment.length());
        String checkstr = (pseudo_head + tcpsegment).toUpperCase();
        if((checkstr.length()/2)%2 == 1){
            checkstr += "00";
        }
        System.out.println(makeChecksum(checkstr));
    }
}