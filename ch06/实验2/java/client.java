/*
 * @Author: Devil 
 * @Date: 2020-05-11 17:24:09 
 * @Last Modified by: Devil
 * @Last Modified time: 2020-06-11 17:33:10
 */
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Scanner; 
import java.net.Socket;
import java.net.UnknownHostException;

public class client {
    public static void main(String[] args) throws  IOException {
        Scanner sc = new Scanner(System.in); 
        System.out.println("IP:"); 
        String ip = sc.nextLine(); 
        System.out.println("Port:"); 
        int port = sc.nextInt(); 
        System.out.println("Content:"); 
        String Content = sc.next(); 
        Socket s=new Socket(ip,port);
        OutputStream out=s.getOutputStream();
        out.write(Content.getBytes());
        InputStream is=s.getInputStream();
        byte buf[]=new byte[1024];
        int len=is.read(buf);
        System.out.println(new String(buf,0,len));
        s.close();
    }

}