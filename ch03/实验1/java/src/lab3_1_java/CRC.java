package lab3_1_java;

public class CRC {

	public static long getbi(String linestr) {//将二进制字符串转为long
		String bs = linestr.substring(linestr.indexOf("=") + 1);
		long ib = sbtoi(bs);
		return ib;
	}
	
	public static long sbtoi(String bs) {
		long total = 0;
		char[] ba = bs.toCharArray();
		for(int i = 0; i < ba.length; i++) {
			total = total + ba[i] - 48;
			if(i != ba.length - 1) {
				total = total << 1;
			}
		}
		return total;
	}
	
	public static long CRCcal(long info1, long genx) {//返回余数
		info1 = info1 << longlen(genx, 2) - 1;
		long rem = mod2div_rem(info1, genx);
		return rem;
	}
	
	public static long Sendcal(long info1, long genx) {//返回发送帧
		info1 = info1 << longlen(genx, 2) - 1;
		long rem = mod2div_rem(info1, genx);
		return info1 + rem;
	}
	
	public static boolean CRCver(long recv, long genx) {//CRC校验
		return (mod2div_rem(recv, genx) == 0);
	}
	
	public static long mod2div_rem(long a, long b) {//模2除法生成余数
		int lena = longlen(a,2);
		int lenb = longlen(b,2);
		long d = 0;
		long e = 0;
		for(int i = lena; i > 0; i--) {
			long c = getIndex(a, i); 
			d = (d << 1) + c;
			e = e << 1;
			if(longlen(d, 2) == lenb) {
				e = e + 1;
				d = d ^ b;
			}
		}
		return d;
	}
	
	public static int longlen(long num,int radix) {//获取位数
		for(int i = 0; i < 64; i++) {
			long c = (long) Math.pow(radix, i);
			if(num % c == num) {
				return i;
			}
		}
		return 0;
	}
	
	private static long getIndex(long a,int index) {
		return a >> (index - 1) & 1;
	}
	
}
