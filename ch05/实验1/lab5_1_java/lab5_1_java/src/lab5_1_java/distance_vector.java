package lab5_1_java;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.Scanner;

public class distance_vector {
	
	public static int n = 5;
	public static int[][] D = new int[n][n];
	public static int[][] T = new int[n][n]; 
	public static int flag = 0;
	
	public static void update() {
		int times = 0;
		int last = times;
		while(true) {
			for(int i = 0; i < n; i++) {
				if(flag == 1 && i == 5) {
					break;
				}
				for(int j = 0; j < n; j++) {
					if((D[i][j] < 16) && (T[i][j] == j)) {
						for(int k = 0; k < n; k++) {
							if(D[i][k] < 16) {
								if(T[j][k] == i) {
									if(D[j][k] != D[j][i] + D[i][k]) {
										D[j][k] = D[j][i] + D[i][k];
										times++;
									}
								}else if (D[j][k] > D[j][i] + D[i][k]) {
									D[j][k] = D[j][i] + D[i][k];
									T[j][k] = i;
									times++;
								}
							}else {
								continue;
							}
						}
					}
				}
			}
			if(times == last) {
				break;
			}
			last = times;
		}
		System.out.println("updating " + times + " times.");
	}
	
	public static void display() {
		for(int i = 0; i < n; i++) {
			System.out.print(i + 1 + "'s routing table:\t[");
			for(int j = 0; j < n; j++) {
				System.out.print(D[i][j] + " |->" + (T[i][j] + 1) + ", ");
			}
			System.out.println("]");
		}
	}
	
	public static void main(String[] args) throws Exception {
		String path = "src/config.txt";
		String linestr;
		BufferedReader br = new BufferedReader(
				new InputStreamReader(
						new FileInputStream(path)));
		Scanner sc = new Scanner(System.in);
		for(int i = 0; i < n; i++) {
			linestr = br.readLine();
			int end = 0;
			String sub = null;
			for (int j = 0; j < n; j++) {
				end = linestr.indexOf(' ');
				if(end >= 0) {
					sub = linestr.substring(0, end);
					linestr = linestr.substring(end + 1);
					D[i][j] = Integer.parseInt(sub);
				}
			}
		}
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				if(D[i][j] < 16) {
					T[i][j] = j;
				}else {
					T[i][j] = -1;
				}
			}
		}
		String command = "";
		while(true) {
			command = sc.nextLine();
			if(command.equals("update")) {
				update();
			}else if (command.equals("display")) {
				display();
			}else if (command.equals("shutdown")) {
				D[3][4] = 16;
				flag = 1;
			}else if (command.equals("open")) {
				D[3][4] = 2;
				T[3][4] = 4;
				flag = 0;
			}else {
				break;
			}
		}
	}
}
