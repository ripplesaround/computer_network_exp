package lab3_1_java;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;

public class main {
	public static void main(String[] args) throws Exception{
		int times = 5;
		String path = "src/config.txt";
		String linestr;
		long info1;
		long genx;
		long info2;
		long crc;
		BufferedReader br = new BufferedReader(
				new InputStreamReader(
						new FileInputStream(path)));
		for(int i = 0; i < times; i++) {
			System.out.println("message " + (i+1) + ":");
			linestr = br.readLine();
			info1 = CRC.getbi(linestr);
			System.out.println("InfoString1:\t\t" + Long.toBinaryString(info1));
			linestr = br.readLine();
			genx = CRC.getbi(linestr);
			System.out.println("GenXString:\t\t" + Long.toBinaryString(genx));
			crc = CRC.CRCcal(info1, genx);
			System.out.println("CRC String:\t\t" + Long.toBinaryString(crc));
			info1 = CRC.Sendcal(info1, genx);
			System.out.println("Sending String:\t\t" + Long.toBinaryString(info1));
			linestr = br.readLine();
			info2 = CRC.getbi(linestr);
			System.out.println("InfoString2:\t\t" + Long.toBinaryString(info2));
			info2 = (info2 << (CRC.longlen(genx, 2) - 1)) + crc;
			System.out.println("Receving String:\t" + Long.toBinaryString(info2));
			System.out.println("CRC Verification:\t" + CRC.CRCver(info2, genx));
			linestr = br.readLine();
		}
	}
}
