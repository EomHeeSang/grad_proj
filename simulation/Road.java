package simulation;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JFrame;
import javax.swing.JLabel;

public class Road extends JFrame {
	private static final long serialVersionUID = 1L;
	
	private Car []cars = new Car[300];
	private Signal []signals = new Signal[9];
	private int numbers_car;
	double random;
	
	private Image offScreenImage;
	private Graphics offScreen;

	public Road() {
		setSize(1000, 1000);
		setTitle("An Empty Frame");
		getContentPane().setBackground(Color.DARK_GRAY);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setResizable(false);
		setLayout(null);
		
		numbers_car = 0;
	
		makeSignal();
		
		this.addKeyListener(new KeyListener() {
			public void keyPressed(KeyEvent e) {
				//space bar = 32
				if(e.getKeyCode() == 32) {
					random = Math.random();
					
					// 테스팅용 고정 출발지/목적지 생성 (0,250) -> (1000, 250)
					 makeCar(12, 4);
					 makeCar(9, 1);
					 makeCar(11, 1);
					
					// 차 출발지, 목적지 포인트 랜덤 생성 
					// makeCar((int)(random*12)+1, (int)(random*12)+1);
					getContentPane().setBackground(Color.BLUE);
				}
			}

			@Override
			public void keyTyped(KeyEvent keyevent) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void keyReleased(KeyEvent keyevent) {
				// TODO Auto-generated method stub
				
			}
		});
		
		setVisible(true);
		
		createBufferStrategy(2);
	}

	
	private void makeCar(int start, int finish) {
		
		cars[numbers_car] = new Car(start, finish, signals);
		cars[numbers_car].start();
		numbers_car++;
	}
	
	
	private void makeSignal(){
		for(int i=0;i<9;i++){
			signals[i] = new Signal(i);
			signals[i].start();
		}
	}
	private void paintingjob(Graphics g){
		int blockw, blockh;
		blockw = this.getWidth()/6;
		blockh = this.getHeight()/6;
		
		g.clearRect(0, 0, 1000, 1000);
		/*
		g.setColor(Color.GREEN); // 잔디
		g.fillRect(0, 35, blockw, blockh); // 좌상
		g.fillRect(0, blockh*2 + 35, blockw, blockh); // 좌하
		//g.fillRect(0, blockh*4 + 35, blockw, blockh); // 좌하
		g.fillRect(blockw*2, 35, blockw, blockh); // 우상
		g.fillRect(blockw*2, blockh*2 + 35, blockw, blockw); // 우하
		//g.fillRect(blockw*2, blockh*4 + 35, blockw, blockw); // 우하
*/

		g.setColor(Color.YELLOW); // 중앙선
		g.fillRect(250, 0, 10, 1000); // 상
		g.fillRect(500, 0, 10, 1000); // 하
		g.fillRect(750, 0, 10, 1000); // 좌
		g.fillRect(0, 250 , 1000 , 10); // 우
		g.fillRect(0, 500 , 1000 , 10); // 우
		g.fillRect(0, 750 , 1000 , 10); // 우
		
		
	      for(int i=0;i<9;i++){
	    	  g.setColor(Color.BLUE);
	         switch(signals[i].getSigInfo()){
	         case 0:
	            g.fillRect(200, 200, 100, 10);
	            g.fillRect(200, 300, 100, 10);
	            g.setColor(Color.RED);
	            g.fillRect(200, 200, 10, 100);
	            g.fillRect(300, 200, 10, 100);
	            break;
	         case 1:
	            g.fillRect(200, 200, 10, 100);
	            g.fillRect(300, 200, 10, 100);
	            g.setColor(Color.RED);
	            g.fillRect(200, 200, 100, 10);
	            g.fillRect(200, 300, 100, 10);
	            break;
	         case 2:
	            break;
	         default:
	            break;
	         }
	      }

		g.setColor(Color.RED);
		for(int i=0;i<numbers_car;i++){
			Point p;
			p = cars[i].getPosition();
			g.fillOval(p.x, p.y, 30, 30);
		}
	}
	@Override public void paint(Graphics g) {
		//super.paint(g);

		Graphics2D g2 = (Graphics2D)getBufferStrategy().getDrawGraphics();
		
		paintingjob(g2);
		
		this.getBufferStrategy().show();
	}
	
	
	public static void main(String[] args) {
		Road m = new Road();
		
		while (true){
			m.repaint();
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}


