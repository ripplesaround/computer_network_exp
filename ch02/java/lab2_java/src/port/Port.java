package port;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.Scanner;
import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;

public class Port {

    public static void main(String[] args) throws Exception {
    	SerialPort c1 = openport("COM1", 9600);
    	SerialPort c2 = openport("COM2", 9600);
    	System.out.print("please enter datasend from COM1:");
    	Scanner sc = new Scanner(System.in);
    	String sdata = sc.nextLine();
    	byte[] datasend = sdata.getBytes();
    	write(c1, datasend);
    	byte[] dataread = read(c2);
    	System.out.println("dataread from COM2 is:"+new String(dataread));
    	closeport(c1);
    	closeport(c2);
    }	
	
    public static SerialPort openport(String portname, int baudrate) throws Exception {
    	CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portname);
        CommPort commPort = portIdentifier.open(portname, 2000);
        SerialPort serialPort = (SerialPort) commPort;
        serialPort.setSerialPortParams(baudrate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
        System.out.println("portname:"+portname+"\nbaudrate:"+baudrate+"\ndatabits:"+SerialPort.DATABITS_8+"\nparity:"+SerialPort.PARITY_NONE+"\nstopbits:"+SerialPort.STOPBITS_1);
        return serialPort;
	}
    
    public static void closeport(SerialPort serialport) {
    	if(serialport != null) {
    		serialport.close();
    		serialport = null;
    	}
    	return;
	}
    
    public static void write(SerialPort serialport, byte[] data) throws Exception {
		OutputStream out = null;
		out = serialport.getOutputStream();
		out.write(data);
		out.flush();
		return;
	}
    
    public static byte[] read(SerialPort serialport) throws Exception {
		InputStream in = null;
		byte[] bytes = null;
		in = serialport.getInputStream();
        int bufflenth = in.available();
        while (bufflenth != 0) {                             
            bytes = new byte[bufflenth];
            in.read(bytes);
            bufflenth = in.available();
        }
        if (in != null) {
            in.close();
            in = null;
        }
        return bytes;
	}
    
}
