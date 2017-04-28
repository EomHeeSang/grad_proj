package simulation;

import java.awt.*;

public class Car extends Thread {
	private Point p;
	private Point r;
	//SharedArea sh;

	// 자동차 출발지 _p , 목적지 _r
	public Car(int _p, int _r) {

			switch (_p) {
			case 1:
				p = new Point(250, 0);
				break;
			case 2:
				p = new Point(500, 0);
				break;
			case 3:
				p = new Point(750, 0);
				break;
			case 4:
				p = new Point(1000, 250);
				break;
			case 5:
				p = new Point(1000, 500);
				break;
			case 6:
				p = new Point(1000, 750);
				break;
			case 7:
				p = new Point(750, 1000);
				break;
			case 8:
				p = new Point(500, 1000);
				break;
			case 9:
				p = new Point(250, 1000);
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
				r = new Point(250, 0);
				break;
			case 2:
				r = new Point(500, 0);
				break;
			case 3:
				r = new Point(750, 0);
				break;
			case 4:
				r = new Point(1000, 250);
				break;
			case 5:
				r = new Point(1000, 500);
				break;
			case 6:
				r = new Point(1000, 750);
				break;
			case 7:
				r = new Point(750, 1000);
				break;
			case 8:
				r = new Point(500, 1000);
				break;
			case 9:
				r = new Point(750, 1000);
				break;
			case 10:
				r = new Point(0, 750);
				break;
			case 11:
				r = new Point(0, 500);
				break;
			case 12:
				r = new Point(0, 250);
				break;
			}
	}

	// 자동차의 위치를 이동
	private void move(int _x, int _y) {
		p.x = _x;
		p.y = _y;
	}

	public Point getPosition() {
		return p;
	}

	public int getArea() {
		// if(p.x )
		return 0;
	}

	// 스레드 실행
	public void run() {
		// 테스팅용 고정 y값
		if (p.y == 250) {
			while (true) {
				p.x += 5;
				p.y += 0;
				
				move(p.x, p.y);

				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}

		// 테스티용 고정 x값
		else if (p.x == 250) {
			while (p.x > 0) {
				p.x -= 0;
				p.y -= 5;
				move(p.x, p.y);

				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}

	}
}