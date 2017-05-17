package simulation;

import java.awt.*;

public class Car extends Thread {
	private int start;		// 차의 출발지
	private int finish;		// 차의 목적지
	private Point p;		// 현재 차의 위치를 저장
	Signal[] sig;
	char[] route;
	boolean[] sigState;

	// 자동차 출발지 _p , 목적지 _r
	public Car(int _p, int _r, char[] _route, Signal[] _sig) {

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
		
		start = _p;
		finish = _r;
		
		route = _route;
		sig = _sig;
	}

	// 자동차의 위치를 이동
	private void move(int _x, int _y) {
		p.x = _x;
		p.y = _y;
	}

	// 경로에 따라 사거리에서 이동해주는 함수
	private void changeDirection(char[] route) {
		
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

		while (true) {

			sigState = sig[0].getSigState();
			
			while(p.x <= 1000 || p.y <= 1000) {
				for(int i = 0; i < 6; i++) {
					if(route[i] == 'e') {
						for(int j = 0; j < 50; j++) {
							
							if (sigState[0] == true || sigState[2] == true) {
								if (p.x >= 180 && p.x <= 200) {
									move(p.x, p.y);
									j--;
								}
								
								else {
									p.x += 5;
									p.y += 0;
									move(p.x, p.y);
								}
									
							}
							
							else {
								p.x += 5;
								p.y += 0;
								move(p.x, p.y);
							}
							
							try {
								Thread.sleep(100);
							} catch (InterruptedException e) {
								e.printStackTrace();
							}
						}
					}
					else if (route[i] == 'w') {
						for(int j = 0; j < 50; j++) {
							p.x -= 5;
							p.y -= 0;
							move(p.x, p.y);
							
							try {
								Thread.sleep(100);
							} catch (InterruptedException e) {
								e.printStackTrace();
							}
						}
					}
					
					else if (route[i] == 's') {
						for(int j = 0; j < 50; j++) {
							p.x += 0;
							p.y += 5;
							move(p.x, p.y);
							
							try {
								Thread.sleep(100);
							} catch (InterruptedException e) {
								e.printStackTrace();
							}
						}
					}
					
					else if (route[i] == 'n') {
						for(int j = 0; j < 50; j++) {
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
					
					else
						move(p.x, p.y);
					
				}
			}
			
			/*
			// 12번 구역 생성
			if (p.y == 250) {
				// 도로 영역 벗어날때까지
				while (p.x < 1000 || p.y < 1000) {
					// 정지신호시 멈춤
					if (sigState[0] == true || sigState[2] == true) {
						if (p.x >= 180 && p.x <= 200) {
							move(p.x, p.y);
						}

						else {
							p.x += 5;
							p.y += 0;
							move(p.x, p.y);
						}
					}

					// 초록불일시
					else {
						p.x += 5;
						p.y += 0;
						move(p.x, p.y);
					}

					try {
						Thread.sleep(100);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					//
				}
			}

			// 테스팅용 고정 x값
			if (p.x == 250) {
				while (p.x > 0) {

					if (sigState[1] == true || sigState[3] == true ) {
						if (p.y >= 300 && p.y <= 320) {
							move(p.x, p.y);	
						}

						else {
							p.x -= 0;
							p.y -= 5;
							move(p.x, p.y);
						}
					}

					else {
						p.x -= 0;
						p.y -= 5;
						move(p.x, p.y);
					}

					try {
						Thread.sleep(100);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
			
			else if (p.y == 500) {
				while (p.x <= 250) {
					
						p.x += 5;
						p.y += 0;
						move(p.x, p.y);
						
						if(p.x >= 240 && p.x <=250) {
							p.x = 250;
							p.y = 499;
							move(p.x, p.y);
							break;
						}
				
					try {
						Thread.sleep(100);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			*/}
		}
	}
