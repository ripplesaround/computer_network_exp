package lab3_1_java;

import java.math.BigInteger;

public class CRC {

	public static BigInteger getbi(String linestr) {//将二进制字符串转为biginteger
		String bs = linestr.substring(linestr.indexOf("=") + 1);
		BigInteger ib = new BigInteger(bs, 2);
		return ib;
	}
	
	public static BigInteger CRCcal(BigInteger info1, BigInteger genx) {//返回余数
		info1 = info1.shiftLeft(BigIntegerlen(genx, new BigInteger("2")) - 1);
		BigInteger rem = mod2div_rem(info1, genx);
		return rem;
	}
	
	public static BigInteger Sendcal(BigInteger info1, BigInteger genx) {//返回发送帧
		info1 = info1.shiftLeft(BigIntegerlen(genx, new BigInteger("2")) - 1);
		BigInteger rem = mod2div_rem(info1, genx);
		return info1.add(rem);
	}
	
	public static boolean CRCver(BigInteger rece, BigInteger genx) {//检验余数
		return (mod2div_rem(rece, genx).compareTo(new BigInteger("0")) == 0);
	}
	
	public static BigInteger mod2div_rem(BigInteger a, BigInteger b) {//模2除法生成余数
		int lena = BigIntegerlen(a, new BigInteger("2"));
		int lenb = BigIntegerlen(b, new BigInteger("2"));
		BigInteger d = new BigInteger("0");
		BigInteger e = new BigInteger("0");
		for(int i = lena; i > 0; i--) {
			BigInteger c = getIndex(a, i); 
			d = d.shiftLeft(1).add(c);
			e = e.shiftLeft(1);
			if(BigIntegerlen(d, new BigInteger("2")) == lenb) {
				e = e.add(new BigInteger("1"));
				d = d.xor(b);
			}
		}
		return d;
	}
	
	public static int BigIntegerlen(BigInteger num,BigInteger radix) {//获取位数
		BigInteger c = new BigInteger("1");
		for(int i = 1; true; i++) {
			c = c.multiply(radix);
			if(c.compareTo(num) == 1) {
				return i;
			}
		}
	}
	
	private static BigInteger getIndex(BigInteger a,int index) {
		return a.shiftRight(index-1).and(new BigInteger("1"));
	}
	
}
