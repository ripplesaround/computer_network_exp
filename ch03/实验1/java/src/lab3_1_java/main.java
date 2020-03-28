package lab3_1_java;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.math.BigInteger;

public class main {
	public static void main(String[] args) throws Exception{
		int times = 5;
		String path = "src/config.txt";
		String linestr;
		BigInteger info1;
		BigInteger genx;
		BigInteger info2;
		BigInteger crc;
		BufferedReader br = new BufferedReader(
				new InputStreamReader(
						new FileInputStream(path)));
		for(int i = 0; i < times; i++) {
			System.out.println("message " + (i+1) + ":");
			linestr = br.readLine();
			info1 = CRC.getbi(linestr);
			System.out.println("InfoString1:\t\t" + info1.toString(2));
			linestr = br.readLine();
			genx = CRC.getbi(linestr);
			System.out.println("GenXString:\t\t" + genx.toString(2));
			crc = CRC.CRCcal(info1, genx);
			System.out.println("CRC String:\t\t" + crc.toString(2));
			info1 = CRC.Sendcal(info1, genx);
			System.out.println("Sending String:\t\t" + info1.toString(2));
			linestr = br.readLine();
			info2 = CRC.getbi(linestr);
			System.out.println("InfoString2:\t\t" + info2.toString(2));
			info2 = (info2.shiftLeft(CRC.BigIntegerlen(genx, new BigInteger("2")) - 1)).add(crc);
			System.out.println("Receving String:\t" +info2.toString(2));
			System.out.println("CRC Verification:\t" + CRC.CRCver(info2, genx));
			linestr = br.readLine();
		}
	}
}
