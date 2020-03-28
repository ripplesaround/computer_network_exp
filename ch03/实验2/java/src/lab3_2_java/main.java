package lab3_2_java;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;

public class main {
	public static String bitflagString = "01111110";
	public static String byteflagString = "7E";
	public static String byteescapeString = "7D";
	public static void main(String[] args) throws Exception{
		String infoString;
		String send;
		String recv;
		String path = "src/config.txt";
		BufferedReader br = new BufferedReader(
				new InputStreamReader(
						new FileInputStream(path)));
		for(int i = 0; i < 3; i++) {
			infoString = br.readLine();
			send = Bit_padding.padding(infoString, bitflagString);
			recv = Bit_padding.parsing(send, bitflagString);
			System.out.println("bitString:\t\t" + infoString + "\n" + 
					"flagString:\t" + bitflagString + "\n" + 
					"sendString:\t" + send + "\n" + 
					"recvString:\t" + recv + "\n");
		}
		br.readLine();
		for(int i = 0; i < 3; i++) {
			infoString = br.readLine();
			send = Byte_padding.padding(infoString, byteflagString, byteescapeString);
			recv = Byte_padding.parsing(send, byteflagString, byteescapeString);
			System.out.println("bitString:\t\t" + infoString + "\n" + 
					"flagString:\t" + bitflagString + "\n" + 
					"sendString:\t" + send + "\n" + 
					"recvString:\t" + recv + "\n");
		}
	}
}
