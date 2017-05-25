package simulation;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Random;

public class ReceiveSocket extends Thread {
	private ServerSocket sock_s;
	private Socket sock_;
	private DataInputStream dis;
	private Road r;

	public ReceiveSocket(Road _r) {
		r = _r;
		/*
		try {
			sock_s = new ServerSocket(5000);
			System.out.println("서버 소켓 생성");
		} catch (IOException e) {
			e.printStackTrace();
		}
		try {
			InetAddress local = InetAddress.getLocalHost();
			System.out.println("local IP: " + local.getHostAddress());

			sock_ = sock_s.accept();
			// System.out.println(sock_.getInetAddress() + "와 연결됨");

			//InputStream in = sock_.getInputStream();
			//dis = new DataInputStream(in);
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}*/
	}
	// ServerSocket sock_s = null;

	public void run() {

		while (true) {
			// try {
			// BufferedReader br = new BufferedReader(new
			// InputStreamReader(dis));

			// char[] ctmp = new char[6];

			// if(br != null)
			// br.read(ctmp);
			// System.out.println("수신" + ctmp[0] + ctmp[1] + ctmp[2]);
			// System.out.println(" end");
			Random random = new Random();
			char[] direction = { 'e', 'w', 's', 'n' };
			char[] route = new char[6];
			char tmpRoute;

			int start = 0;
			int finish = 0;
			
			start = random.nextInt(12) + 1;
			
			while (start == finish)
				finish = random.nextInt(12) + 1;

			switch (start) {
			case 1:
			case 2:
			case 3:
				route[0] = 's';
				break;
			case 4:
			case 5:
			case 6:
				route[0] = 'w';
				break;
			case 7:
			case 8:
			case 9:
				route[0] = 'n';
				break;
			case 10:
			case 11:
			case 12:
				route[0] = 'e';
				break;
			}

			// 두번째 방향부터 랜덤
			for (int i = 1; i < 6; i++) {
				tmpRoute = direction[random.nextInt(4)];
				
				switch(tmpRoute) {
				case 'e':
					if(route[i-1] == 'w') {
						i--;
						continue;
					}
					route[i] = tmpRoute;
					break;
				case 'w':
					if(route[i-1] == 'e') {
						i--;
						continue;
					}
					route[i] = tmpRoute;
					break;
				case 's':
					if(route[i-1] == 'n') {
						i--;
						continue;
					}
					route[i] = tmpRoute;
					break;
				case 'n':
					if(route[i-1] == 's') {
						i--;
						continue;
					}
					route[i] = tmpRoute;
					break;
				}
			}

			//char []route = {'e', 's', 'e', 's', 'e', 's'};
			
			try {
				r.getCarInfo(start, finish, route);
				Thread.sleep(4000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}

			// } catch (IOException ie) {
			// ie.printStackTrace();
			// }
		}

	}
}