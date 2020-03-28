package lab3_3_java;

import java.math.BigInteger;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class Client {

	public static int FilterError = 10;
	public static int FilterLost = 10;
	public static int serverUDPPort = 7777;
	public static int clientUDPPort = 8888;
	public static void main(String[] args) throws Exception{
		InetAddress ipAddress = InetAddress.getLocalHost();
		byte[] buffer = new byte[1024];
		BigInteger genx = new BigInteger("10001000000100001");
		boolean test;
		String infoString;
		int seq = 0;
		DatagramSocket client = new DatagramSocket(clientUDPPort);
		DatagramPacket packet_recv = new DatagramPacket(buffer, 1024);
		DatagramPacket packet_send;
		while(true) {//轮询接收包
			client.receive(packet_recv);
			infoString = new String(packet_recv.getData(), 0, packet_recv.getLength());
			if(infoString.equals("end")) {
				System.out.println("trasfer end!");
				break;
			}
			if(infoString.equals("start")) {
				System.out.println("trasfer start!");
				continue;
			}
			System.out.print("expecting packet " + Integer.toString(seq) + " ...\t");
			System.out.print("receving packet " + infoString.substring(0, 1) + " !\t");
			test = CRC.CRCver(CRC.getbi2(infoString.substring(1)), genx);
			if(Integer.toString(seq).equals(infoString.substring(0, 1)) && test) {
				System.out.print("which contains:" + infoString.substring(1, 32) + "\t");
				infoString = Integer.toString(seq);
				seq = (seq + 1) % 2;
			}else {
				System.out.print("an error occurs... requesting for resending packet " + Integer.toString(seq) + "\t");
				infoString = "err";
			}
			packet_send = new DatagramPacket
					(infoString.getBytes(), infoString.length(), ipAddress, serverUDPPort);
			client.send(packet_send);
			System.out.println("sending ack as " + infoString);
			Thread.sleep(1000);
		}
		client.close();
	}
	
}
