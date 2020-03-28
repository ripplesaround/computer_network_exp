package lab3_3_java;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.InterruptedIOException;
import java.math.BigInteger;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Random;

public class Server {

	public static int FilterError = 10;
	public static int FilterLost = 10;
	public static int ErrorStart;
	public static int LostStart;
	public static int serverUDPPort = 7777;
	public static int clientUDPPort = 8888;
	public static int TIMEOUT = 3000;
	
	public static void main(String[] args) throws Exception {
		InetAddress ipAddress = InetAddress.getLocalHost();
		boolean ready = true;
		boolean resend = false;
		String path = "src/config.txt";
		String infoString;
		String startString = "start";
		String endString = "end";
		BufferedReader br = new BufferedReader(
				new InputStreamReader(
						new FileInputStream(path)));
		byte[] buffer = new byte[1024];
		BigInteger genx = new BigInteger("10001000000100001");
		DatagramSocket server = new DatagramSocket(serverUDPPort);
		DatagramPacket packet_recv = new DatagramPacket(buffer, 1024);
		DatagramPacket packet_send;
		Random r = new Random();
		ErrorStart = r.nextInt(10);
		LostStart = r.nextInt(10);
		int seq = 0;
		int times = 0;
		server.setSoTimeout(TIMEOUT);
		packet_send = new DatagramPacket
				(startString.getBytes(), startString.length(), ipAddress, clientUDPPort);
		System.out.println("transfer start!");
		server.send(packet_send);//发送开始包
		
		while(true) {//主要轮询
			
			if(resend) {
				server.send(packet_send);
				System.out.print("resending packet " + Integer.toString(seq) + " ...\t");
				resend = false;
				ready = false;
			}
			
			if(ready) {//收到ack，可以发送下一个包
				
				infoString = br.readLine();
				if(infoString == null) {
					break;
				}
				infoString = CRC.Sendcal(CRC.getbi2(infoString), genx).toString(2);
				infoString = Integer.toString(seq) + infoString;
				packet_send = new DatagramPacket
						(infoString.getBytes(), infoString.length(), ipAddress, clientUDPPort);
				System.out.print(Integer.toString(times) + ":packet " + Integer.toString(seq) + 
						" is sent...\t");
				if(times % 10 == LostStart) {
					System.out.println("oops, packet " + Integer.toString(seq) + 
							" is lost...");
				}
				else if(times % 10 == ErrorStart){
					System.out.println("oops, packet " + Integer.toString(seq) + 
							" has something wrong during the transfer...");
					String errinfoString = infoString.substring(0, 25) + infoString.substring(26);
					DatagramPacket errpacket_send = new DatagramPacket
							(errinfoString.getBytes(), errinfoString.length(), ipAddress, clientUDPPort);
					server.send(errpacket_send);
				}else {
					server.send(packet_send);
				}
				ready = false;
				times++;
				Thread.sleep(1000);
			}
			
			
			try {
				server.receive(packet_recv);
				infoString = new String(packet_recv.getData(), 0, packet_recv.getLength());
				System.out.print("receving ack as:" + infoString + "\t");
				if(infoString.equals("err") || !infoString.substring(0, 1).equals(Integer.toString(seq))) {
					System.out.println("error occurs...");
					resend = true;
				}else if(infoString.substring(0, 1).equals(Integer.toString(seq))) {
					ready = true;
					System.out.print("packet " + Integer.toString(seq) + 
							" is recieved successfully!\t");
					seq = (seq + 1) % 2;
					System.out.println("preparing packet "+ Integer.toString(seq) + " ...");
				}
			} catch (InterruptedIOException e) {//超时重发
				System.out.println("Time out...");
				resend = true;
			}
			
			
			
		}
		packet_send = new DatagramPacket
				(endString.getBytes(), endString.length(), ipAddress, clientUDPPort);
		server.send(packet_send);
		System.out.println("transfer end!");
		server.close();
	}
}
