package lab3_2_java;

public class Bit_padding {

	public static String padding(String send, String flag) {
		StringBuilder sendb = new StringBuilder(send);
		for(int i = 0; i <= sendb.length() - 5 ; i++) {
			if(sendb.subSequence(i, i + 5).equals("11111")) {
				sendb.insert(i + 5, "0");
			}
		}
		sendb.insert(0, flag);
		sendb.append(flag);
		return sendb.toString();
	}
	
	public static String parsing(String recv, String flag) {
		StringBuilder recvb = new StringBuilder(recv);
		recvb.replace(0, 8, "");
		recvb.replace(recvb.length() - 8, recvb.length(), "");
		for(int i = recvb.length(); i >= 6 ; i--) {
			if(recvb.subSequence(i - 6, i).equals("111110")) {
				recvb.replace(i - 1, i, "");
			}
		}
		return recvb.toString();
	}
}
