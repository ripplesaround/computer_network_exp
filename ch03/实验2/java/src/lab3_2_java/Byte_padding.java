package lab3_2_java;

public class Byte_padding {
	
	public static String padding(String send, String flag, String escape) {
		StringBuilder sendb = new StringBuilder(send);
		for(int i = 0; i <= sendb.length() - 2 ; i++) {
			if(sendb.subSequence(i, i + 2).equals(escape) || 
					sendb.subSequence(i, i + 2).equals(flag)) {
				sendb.insert(i, escape);
				i += 2;
			}
		}
		sendb.insert(0, flag);
		sendb.append(flag);
		return sendb.toString();
	}
	
	public static String parsing(String recv, String flag, String escape) {
		StringBuilder recvb = new StringBuilder(recv);
		recvb.replace(0, 2, "");
		recvb.replace(recvb.length() - 2, recvb.length(), "");
		for(int i = 0; i <= recvb.length() - 2; i++) {
			if(recvb.subSequence(i, i + 2).equals(escape)) {
				recvb.replace(i, i + 2, "");
				i++;
			}
		}
		return recvb.toString();
	}
}
