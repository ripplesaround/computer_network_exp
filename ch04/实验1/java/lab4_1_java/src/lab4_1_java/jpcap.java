package lab4_1_java;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Scanner;

import jpcap.*;
import jpcap.packet.Packet;
import jpcap.packet.ARPPacket;
import jpcap.packet.IPPacket;
import jpcap.packet.TCPPacket;
import jpcap.packet.UDPPacket;

import java.util.Calendar;
import java.util.Date;

public class jpcap {

    public static void main(String[] args) {
    	System.out.println("---网卡列表---");
        NetworkInterface[] devices = JpcapCaptor.getDeviceList();
        for(NetworkInterface n : devices)
        {
            System.out.println(n.name + " \t|\t" + n.description);
        }

        JpcapCaptor jpcap = null;
        int caplen = 1518;
        boolean promiscCheck = true;
        Scanner inputScanner = new Scanner(System.in);
        System.out.println("---请输入选择的网卡，编号从1开始---");
        int choose = inputScanner.nextInt() - 1;
        try{
            jpcap = JpcapCaptor.openDevice(devices[choose], caplen, promiscCheck, 50);
        }catch(IOException e){
            e.printStackTrace();
        }
        Packet packet  = null;
        while (packet == null || !(packet instanceof TCPPacket)) {
			packet = jpcap.getPacket();
		}
        byte b[] = packet.header;
        int i = 0;
        System.out.println("---Ethernet Packet---");
        System.out.println("destination address: "+
        Integer.toHexString(b[i++]&0xFF)+":"+
        Integer.toHexString(b[i++]&0xFF)+":"+
        Integer.toHexString(b[i++]&0xFF)+":"+
        Integer.toHexString(b[i++]&0xFF)+":"+
        Integer.toHexString(b[i++]&0xFF)+":"+
        Integer.toHexString(b[i++]&0xFF)
        		);
        System.out.println("source address: "+
                Integer.toHexString(b[i++]&0xFF)+":"+
                Integer.toHexString(b[i++]&0xFF)+":"+
                Integer.toHexString(b[i++]&0xFF)+":"+
                Integer.toHexString(b[i++]&0xFF)+":"+
                Integer.toHexString(b[i++]&0xFF)+":"+
                Integer.toHexString(b[i++]&0xFF)
                		);
        System.out.println("type:"+((b[i++]<<8)|(b[i])));
        i--;
        IPPacket ip = null;
        ARPPacket arp = null;
        switch (((b[i++]<<8)|(b[i]))) {
		case 2048:
			System.out.println("---IP Packet---");
	        ip = (IPPacket)packet;
	        packet = ip;
	        System.out.println("version:" + ip.version);
	        System.out.println("length:" + ip.length);
	        System.out.println("identity:" + ip.ident);
	        System.out.println("DF:" + ip.dont_frag);
	        System.out.println("MF:" + ip.more_frag);
	        System.out.println("priority:" + ip.priority);
	        System.out.println("offset:" + ip.offset);
	        System.out.println("hop limit:"+ ip.hop_limit);
	        Calendar calendar = Calendar.getInstance();
	        calendar.setTimeInMillis(ip.sec * 1000);
	        Date date = calendar.getTime();
	        SimpleDateFormat format = new SimpleDateFormat();
	        System.out.println("date:" + format.format(date));
	        System.out.println("source ip:" + ip.src_ip);
	        System.out.println("destination ip:" + ip.dst_ip);
	        System.out.println("protocol:" + ip.protocol);
			break;
		case 2054:
			System.out.println("---ARP Packet---");
			arp = (ARPPacket)packet;
			packet = arp;
			System.out.println("hardtype:" + arp.hardtype);
			System.out.println("hlen:" + arp.hlen);
			System.out.println("plen:" + arp.plen);
			System.out.println("operation:" + arp.operation);
			System.out.println("protocol:" + arp.prototype);
			break;
		default:
			System.out.println("unknown protocol");
			break;
		}
        
        int pro = 0;
        if(ip != null) {
        	pro = new Integer(ip.protocol);
        }else {
			pro = new Integer(arp.prototype);
		}
        UDPPacket udp = null;
        TCPPacket tcp = null;
        switch(pro) {
            case 1:
            	//protocol = "ICMP";
            	break;
            case 2:
            	//protocol = "IGMP";
            	break;
            case 6:
            	System.out.println("---TCP Packet---");
            	tcp = (TCPPacket)packet;
            	packet = tcp;
            	System.out.println("source port:" + tcp.src_port);
            	System.out.println("destination port:" + tcp.dst_port);
            	System.out.println("seq:" + tcp.sequence);
            	System.out.println("ack_num:" + tcp.ack_num);
            	System.out.println("window:" + tcp.window);
            	System.out.println("fin:" + tcp.fin);
            	System.out.println("syn:" + tcp.syn);
            	System.out.println("rst:" + tcp.rst);
            	System.out.println("psh:" + tcp.psh);
            	System.out.println("ack:" + tcp.ack);
            	System.out.println("urg:" + tcp.urg);
            	System.out.println("rsv1:" + tcp.rsv1);
            	System.out.println("rsv2:" + tcp.rsv2);
            	break;
            case 8:
            	//protocol = "EGP";
            	break;
            case 9:
            	//protocol = "IGP";
            	break;
            case 17:
            	System.out.println("---UDP Packet---");
            	udp = (UDPPacket)packet;
            	packet = udp;
            	System.out.println("source port:" +  + udp.src_port);
            	System.out.println("destination port:" + udp.dst_port);
            	System.out.println("length:" + udp.length);
            	break;
            case 41:
            	//protocol = "IPv6";
            	break;
            case 89:
            	//protocol = "OSPF";
            	break;
            default :
    			System.out.println("unknown protocol");
            	break;
        }
        System.out.println("---Application Data---");
        byte[] data = packet.data;
        for(int j = 0; j < data.length; j++) {
        	if(Integer.toHexString(data[j]&0xFF).length() == 1) {
        		System.out.print("0");
        	}
        	System.out.print(Integer.toHexString(data[j]&0xFF) + "\t");
        	if(j % 10 == 9) {
        		System.out.println();
        	}
        }
    }

}
