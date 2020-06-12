/*
 * @Author: Devil 
 * @Date: 2020-05-11 17:29:10 
 * @Last Modified by: Devil
 * @Last Modified time: 2020-06-11 17:41:47
 */
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
public class server {
    public static void main(String[] args) throws Exception {
        DatagramSocket socket = new DatagramSocket(10002);
        DatagramPacket packet = new DatagramPacket(new byte[1024], 1024);
        socket.receive(packet);
        byte[] arr = packet.getData();
        int len = packet.getLength();
        String send = new String(arr,0,len).toUpperCase();
        DatagramPacket outPacket = new DatagramPacket(send.getBytes(), send.length(), packet.getAddress(), packet.getPort());
        socket.send(outPacket);
        socket.close();
    }
}