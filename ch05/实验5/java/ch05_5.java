/*
 * @Author: Devil 
 * @Date: 2020-05-31 11:57:16 
 * @Last Modified by: Devil
 * @Last Modified time: 2020-05-31 13:31:12
 */
import java.io.*;
import java.util.*;
import java.math.BigInteger;

public class ch05_5 {
    public static String intToBinary32(int i, int bitNum){
        String binaryStr = Integer.toBinaryString(i);
        while(binaryStr.length() < bitNum){
            binaryStr = "0"+binaryStr;
        }
        return binaryStr;
    }
    public static String readFile(String filePath) {  
        BufferedReader br;  
        String read = "";
        String s = "";
        try {  
            File file = new File(filePath);  
            FileReader fileread = new FileReader(file);  
            br = new BufferedReader(fileread);  
            while ((read = br.readLine()) != null) {  
                s += (read+'\n');
            }   
        }catch (Exception ex) {  
            System.out.println(ex.getMessage());  
        }
        return s;
    }
    public static String str2bin(String x) {
        if(x.equals("0")){
            x = "0.0.0.0";
        }
        String[] tmp = x.split("\\.");
        String s = intToBinary32(Integer.parseInt(tmp[0]),8);
        s += intToBinary32(Integer.parseInt(tmp[1]),8);
        s += intToBinary32(Integer.parseInt(tmp[2]),8);
        s += intToBinary32(Integer.parseInt(tmp[3]),8);
        return s;
    }
    public static String int2bin(String x) {  
        char[] tmp0 = new char[Integer.parseInt(x)];
        Arrays.fill(tmp0, '1');
        char[] tmp1 = new char[32-Integer.parseInt(x)];
        Arrays.fill(tmp1, '0');
        String s = new String(tmp0) + new String(tmp1);
        return s;
    }
	public static void main(String[] args) throws IOException {
        List<router_item> router_table = new ArrayList<router_item>();
        String x = readFile("./config.txt");
        String[] tmp = x.split("\n");
        String[] routers = Arrays.copyOfRange(tmp, 1, tmp.length);
        for (String element: routers) {
            String[] router = element.split(" ");
            String next_hop = router[1];
            String[] im = router[0].split("/");
            String ip = str2bin(im[0]);
            String subnet_mask = int2bin(im[1]);
            router_table.add(new router_item(ip,subnet_mask,next_hop));
        }
        System.out.println("-------------------------------");
        System.out.println("当前路由表：");
        Iterator<router_item> it = router_table.iterator();
		while (it.hasNext()) {
            router_item router = it.next();
			System.out.printf("%-40s%-40s%-30s\n",router.ip,router.subnet_mask,router.next_hop);
		} 
        System.out.println("-------------------------------");
        System.out.print("ip: ");
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String dest_ip = str2bin(br.readLine());
        BigInteger dest = new BigInteger(dest_ip,2);
        System.out.println("-------------------------------");
        List<Integer> temp = new ArrayList<Integer>();
        for (int i = 0; i < router_table.size(); i++){
            String dst_subnet =  (dest.and(new BigInteger(router_table.get(i).subnet_mask,2))).toString(2);
            if (dst_subnet.equals(router_table.get(i).ip)){
                temp.add(i);
                System.out.println(i+1+" line match");
            }  
            else{
                System.out.println(i+1+" line not match");
            }       
        }
        System.out.println("-------------------------------");
        if(temp.size() == 0){
            System.out.println("Forward with default route");
            System.out.printf("%-10s%-30s" ,"next hop", router_table.get(router_table.size()-1).next_hop);
        }  
        else if (temp.size() == 1){
            System.out.printf("%-10s%-30s" ,"next hop", router_table.get(temp.get(0)).next_hop);
        }
        else{
            String current_max = router_table.get(temp.get(0)).subnet_mask;
            Integer current_max_index = temp.get(0);
            for(int i = 0;i < temp.size(); i++){
                if(new BigInteger(router_table.get(temp.get(i)).subnet_mask,2).compareTo(new BigInteger(current_max,2)) == 1){
                    current_max_index = temp.get(i);
                    current_max = router_table.get(temp.get(i)).subnet_mask;
                }
            }
            System.out.printf("%-10s%-30s" ,"next hop", router_table.get(current_max_index).next_hop);
        }
	}
}
class router_item {
    String ip;
    String subnet_mask;
    String next_hop;
    router_item(String ipt,String subnet_maskt,String next_hopt){
        ip = ipt;
        subnet_mask = subnet_maskt;
        next_hop = next_hopt;
    }
}