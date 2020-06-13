import java.util.Random;
import java.util.Scanner;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
public class chap_Verification {
	public static String stringToMD5(String plainText) {
        byte[] secretBytes = null;
        try {
            secretBytes = MessageDigest.getInstance("MD5").digest(
                    plainText.getBytes());
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException("error");
        }
        String md5code = new BigInteger(1, secretBytes).toString(16);
        for (int i = 0; i < 32 - md5code.length(); i++) {
            md5code = "0" + md5code;
        }
        return md5code;
    }
    public static void main(String[] args){
    	Scanner input = new Scanner(System.in);
        Random r = new Random();
        int rand = r.nextInt(1000000)+1000000;
        System.out.println(rand);
        System.out.println("Enter your password:"); 
        String str = input.next();
        System.out.println("Password: "+str); 
        System.out.println("Random: "+rand); 
        str=rand+str;
        String md5code = stringToMD5(str);
        System.out.println("MD5: "+md5code);
    }

}