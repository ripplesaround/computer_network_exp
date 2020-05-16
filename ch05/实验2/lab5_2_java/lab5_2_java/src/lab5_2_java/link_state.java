package lab5_2_java;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.Scanner;

public class link_state {
	
	public static int n = 5;
	public static int[][] D = new int[n][n];
	public static int[][] T = new int[n][n]; 
	
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
					T[i][j] = j + 1;
				}else {
					T[i][j] = -1;
				}
			}
		}
		display();
		for(int i = 0; i < n; i++) {
			dijstra(D, i);
		}
		display();
	}
	
	public static void display() {
		System.out.println("routing table:");
		for(int i = 0; i < n; i++) {
			System.out.print(i + 1 + "'s routing table:\t[");
			for(int j = 0; j < n; j++) {
				System.out.print(D[i][j] + " |->" + T[i][j] + ", ");
			}
			System.out.println("]");
		}
	}
	
	public static void dijstra(int[][] matrix, int source) {
        int[] shortest = new int[matrix.length];
        int[] visited = new int[matrix.length];
        String[] path = new String[matrix.length];
        for (int i = 0; i < matrix.length; i++) {
            path[i] = new String("[" + (source + 1) + "]->[" + (i + 1));
        }
        shortest[source] = 0;
        visited[source] = 1;
        for (int i = 1; i < matrix.length; i++) {
            int min = 16;
            int index = -1;
            for (int j = 0; j < matrix.length; j++) {
                if (visited[j] == 0 && matrix[source][j] < min) {
                    min = matrix[source][j];
                    index = j;
                }
            }
            shortest[index] = min;
            visited[index] = 1;
            for (int m = 0; m < matrix.length; m++) {
                if (visited[m] == 0 && matrix[source][index] + matrix[index][m] < matrix[source][m]) {
                    matrix[source][m] = matrix[source][index] + matrix[index][m];
                    path[m] = path[index] + "]->[" + (m + 1);
                }
            }
        }
        for (int i = 0; i < matrix.length; i++) {
        	path[i] += "]";
            if (i != source) {
                if (shortest[i] >= 16) {
                    System.out.println((i + 1) + "is unreachable for" + (source + 1));
                    T[source][i] = -1;
                } else {
                    System.out.println("the minimum distance from [" + (source + 1) + "] to [" + (i + 1) + "] is " + shortest[i] + ", the route is " + path[i]);
                    String sub = path[i];
                    sub = sub.substring(sub.indexOf('[') + 1);
                    sub = sub.substring(sub.indexOf('[') + 1);
                    sub = sub.substring(0, sub.indexOf(']'));
                    T[source][i] = Integer.parseInt(sub);
                }
            }
        }
    }
}
