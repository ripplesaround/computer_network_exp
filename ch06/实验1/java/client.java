/*
 * @Author: Devil 
 * @Date: 2020-05-11 17:31:26 
 * @Last Modified by: Devil
 * @Last Modified time: 2020-06-11 17:42:35
 */

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.Scanner;
import java.net.UnknownHostException;

public class client {
    public static void main(String[] args) throws Exception {
        Scanner sc = new Scanner(System.in); 
        System.out.println("IP:"); 
        String ip = sc.nextLine(); 
        System.out.println("Port:"); 
        int port = sc.nextInt(); 
        System.out.println("Content:"); 
        String Content = sc.next(); 
        DatagramSocket socket = new DatagramSocket();
        DatagramPacket packet = new DatagramPacket(Content.getBytes(), Content.getBytes().length, InetAddress.getByName(ip), port);
        socket.send(packet);
        socket.receive(packet);
        byte[] arr = packet.getData();
        int len = packet.getLength();
        System.out.println("receive: " + new String(arr,0,len));
        socket.close();
    }
}