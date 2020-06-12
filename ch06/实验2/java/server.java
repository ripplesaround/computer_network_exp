/*
 * @Author: Devil 
 * @Date: 2020-05-11 17:23:56 
 * @Last Modified by: Devil
 * @Last Modified time: 2020-06-11 17:24:22
 */

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class server {
    public static void main(String[] args) throws IOException {

        ServerSocket ss = new ServerSocket(10002);
        Socket s = ss.accept();
        String ip = s.getInetAddress().getHostAddress();
        System.out.println(ip + "....connected....");
        InputStream in = s.getInputStream();
        int len = 0;
        byte[] buf = new byte[1024];
        len = in.read(buf);
        String send = new String(buf, 0, len).toUpperCase();
        OutputStream os=s.getOutputStream();
        os.write(send.getBytes());
        os.close();
        s.close();
        ss.close();
    }

}