package simulation;

import java.awt.Color;
import java.awt.Point;

public class Car extends Thread {
	private int carId;
	private Point start; // 차의 출발지
	private Point finish; // 차의 목적지
	private Point p; // 현재 차의 위치를 저장
	private boolean isStopped; // 차량이 정지했는지
	private boolean isChecked; // 서버로 전송했는지
	private boolean isFinished; // 차량이 목적지에 도착했는가
	private String carInfo; // 서버로 보낼 차량의 정보
	Signal[] sig;
	char[] route;
	int sigState;

	// 자동차 출발지 _p , 목적지 _r
	public Car(int id, int _p, int _r, char[] _route, Signal[] _sig) {

		switch (_p) {
		case 1:
			p = new Point(500, 0);
			break;
		case 2:
			p = new Point(1000, 0);
			break;
		case 3:
			p = new Point(1500, 0);
			break;
		case 4:
			p = new Point(2000, 250);
			break;
		case 5:
			p = new Point(2000, 500);
			break;
		case 6:
			p = new Point(2000, 750);
			break;
		case 7:
			p = new Point(750, 2000);
			break;
		case 8:
			p = new Point(500, 2000);
			break;
		case 9:
			p = new Point(250, 2000);
			break;
		case 10:
			p = new Point(0, 750);
			break;
		case 11:
			p = new Point(0, 500);
			break;
		case 12:
			p = new Point(0, 250);
			break;
		}

		switch (_r) {
		case 1:
			finish = new Point(500, 0);
			break;
		case 2:
			finish = new Point(1000, 0);
			break;
		case 3:
			finish = new Point(1500, 0);
			break;
		case 4:
			finish = new Point(2000, 250);
			break;
		case 5:
			finish = new Point(2000, 500);
			break;
		case 6:
			finish = new Point(2000, 750);
			break;
		case 7:
			finish = new Point(750, 2000);
			break;
		case 8:
			finish = new Point(500, 2000);
			break;
		case 9:
			finish = new Point(250, 2000);
			break;
		case 10:
			finish = new Point(0, 750);
			break;
		case 11:
			finish = new Point(0, 500);
			break;
		case 12:
			finish = new Point(0, 250);
			break;
		}

		isFinished = false;
		carId = id;
		start = p;
		route = _route;
		sig = _sig;
	}

	// 자동차의 위치를 이동
	private void move(int _x, int _y) {
		p.setLocation(_x, _y);
	}

	public Point getPosition() {
		return p;
	}
	
	public String getCarStat(char route) {
		carInfo = carId + "/" + p.getLocation().x + "/" + p.getLocation().y + "/" + route + "/";

		System.out.println(carInfo);

		return carInfo;
	}
	
	public int getCarId() {
		return carId;
	}
	
	public boolean isFinished() {
		return isFinished;
	}

	// 차량의 신호 요구... cRoute = 현재 차량 이동방향, nRoute = 사거리 통과 후 차량 이동방향
	public void requestSig(Signal[] sig, char cRoute, char nRoute, Point p) {

			for (int i = 0; i < 9; i++) {
				if (Math.abs(sig[i].getLocation().x - p.x) <= 70 && Math.abs(sig[i].getLocation().y - p.y) <= 70) {
					
					if(cRoute == 'e' && nRoute == 's') {
						isStopped = false;
						break;
					}
					
					if(cRoute == 'w' && nRoute == 'n') {
						isStopped = false;
						break;
					}
					
					if(cRoute == 's' && nRoute == 'w') {
						isStopped = false;
						break;
					}
					
					if(cRoute == 'n' && nRoute == 'e') {
						isStopped = false;
						break;
					}
					
					
					if(Math.abs(sig[i].getLocation().x - p.x) < 50 && Math.abs(sig[i].getLocation().y - p.y) < 50)
					{
						isStopped = false;
						break;
					}
					
					if(cRoute == 's') {
						if(p.y - sig[i].getLocation().y > 0) {
							isStopped = false;
							break;
						}
					}
					
					else if(cRoute == 'n') {
						if(p.y - sig[i].getLocation().y < 0) {
							isStopped = false;
							break;
						}
					}
					
					else if(cRoute == 'w') {
						if(p.x - sig[i].getLocation().x < 0) {
							isStopped = false;
							break;
						}
					}
					
					else if(cRoute == 'e') {
						if(p.x - sig[i].getLocation().x > 0) {
							isStopped = false;
							break;
						}
					}
					
					if (sig[i].getSigStat(nRoute) == 'r')
						isStopped = true;
					else if (sig[i].getSigStat(nRoute) == 'g')
						isStopped = false;
					
					break;
				}
			}
	}

	// 스레드 실행
	public void run() {
		// 테스팅용 고정 y값

		while (true) {
			for (int i = 0; i < 6; i++) {
				isChecked = false;
				isStopped = false;

				if (route[i] == 'e') {
					for (int j = 0; j < 100; j++) {

						if (i < 5)
							requestSig(sig, route[i], route[i + 1], p);

						if (isStopped == true) {
							//getCarStat(route[i + 1]);
							j--;
							//isChecked = true;
						}

						else {
							p.x += 5;
						}

						move(p.x, p.y);

						try {
							Thread.sleep(100);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}

				else if (route[i] == 'w') {
					for (int j = 0; j < 100; j++) {
						if (i < 5)
							requestSig(sig, route[i], route[i + 1], p);

						if (isStopped == true) {
							//getCarStat(route[i + 1]);
							j--;
							//isChecked = true;
						}

						else {
							p.x -= 5;
						}

						move(p.x, p.y);

						try {
							Thread.sleep(100);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}

				else if (route[i] == 's') {
					for (int j = 0; j < 50; j++) {

						if (i < 5)
							requestSig(sig, route[i], route[i + 1], p);

						if (isStopped == true) {
							//getCarStat(route[i + 1]);
							j--;
							//isChecked = true;
						}

						else {
							p.y += 5;
						}

						move(p.x, p.y);

						try {
							Thread.sleep(100);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}

				else if (route[i] == 'n') {
					for (int j = 0; j < 50; j++) {

						if (i < 5)
							requestSig(sig, route[i], route[i + 1], p);

						if (isStopped == true) {
							//getCarStat(route[i + 1]);
							j--;
							//isChecked = true;
						}

						else {
							p.y -= 5;
						}

						move(p.x, p.y);

						try {
							Thread.sleep(100);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}
				
				if (p.x == 0 || p.y == 0 || p.x == 2000 || p.y == 1000) {
					isFinished = true;
				}
			} 
		}
	}
}
