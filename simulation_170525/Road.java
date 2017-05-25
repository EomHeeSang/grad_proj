package simulation;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.util.ArrayList;
import java.util.Iterator;

import javax.swing.JFrame;

public class Road extends JFrame {
	private static final long serialVersionUID = 1L;

	private ArrayList <Car> cars = new ArrayList <Car>();
	private Car c;
	//private Car []car = new Car[300];
	private Signal []signals = new Signal[9];
	private int numbers_car;
	double random;
	private Point point;
	private Image offScreenImage;
	private Graphics offScreen;

	public Road() {
		
		setSize(2000, 1000);
		setTitle("An Empty Frame");
		getContentPane().setBackground(Color.DARK_GRAY);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setResizable(false);
		setLayout(null);
		
		numbers_car = 0;
	
		makeSignal();
	
		setVisible(true);
		
		createBufferStrategy(2);
	}

	void getCarInfo(int start, int finish, char[] route) {
		// 소켓 통신으로 수신한 차량 생성 데이터정보를 바탕으로 차량 생성 
		makeCar(start, finish, route);
	}
	
	private void makeCar(int start, int finish, char[] route) {
		c = new Car(numbers_car, start, finish, route, signals);
		cars.add(c);
		c.start();
		numbers_car++;
	}
	/*
	private void removeCar(Car[] cars) {
		for(int i=0; i<cars.length; i++) {
			if (cars[i].getPosition() == cars[i].isFinished())
				cars[i] = null;
		}
	}*/
	
	private void makeSignal(){
		for(int i=0;i<9;i++){
			if(i<3){
				signals[i] = new Signal(new Point (500*(i%3+1), 250*1));
			}else if(i>5){
				signals[i] = new Signal(new Point (500*(i%3+1), 250*3));
			}else{
				signals[i] = new Signal(new Point (500*(i%3+1), 250*2));
			}
			signals[i].start();
		}
	}
	private void paintingjob(Graphics g){
		int blockw, blockh;
		blockw = this.getWidth()/6;
		blockh = this.getHeight()/6;
		
		g.clearRect(0, 0, 2000, 1000);

		g.setColor(Color.YELLOW); // 중앙선
		
		// 좌 수직
		g.fillRect(500, 0, 10, 200); 
		g.fillRect(500, 300, 10, 150);
		g.fillRect(500, 550, 10, 150);
		g.fillRect(500, 800, 10, 200);
		
		// 중 수직
		g.fillRect(1000, 0, 10, 200);
		g.fillRect(1000, 300, 10, 150);
		g.fillRect(1000, 550, 10, 150);
		g.fillRect(1000, 800, 10, 200);
		
		// 우 수직
		g.fillRect(1500, 0, 10, 200);
		g.fillRect(1500, 300, 10, 150);
		g.fillRect(1500, 550, 10, 150);
		g.fillRect(1500, 800, 10, 200);
		
		// 상 수평
		g.fillRect(0, 250, 450, 10);
		g.fillRect(550, 250, 400, 10);
		g.fillRect(1050, 250, 400, 10);
		g.fillRect(1550, 250, 450, 10);
		
		// 중 수평
		g.fillRect(0, 500, 450, 10);
		g.fillRect(550, 500, 400, 10);
		g.fillRect(1050, 500, 400, 10);
		g.fillRect(1550, 500, 450, 10);
		
		// 하 수평
		g.fillRect(0, 750, 450, 10);
		g.fillRect(550, 750, 400, 10);
		g.fillRect(1050, 750, 400, 10);
		g.fillRect(1550, 750, 450, 10);
		
		//traffic signal drawing
	    for(int i=0;i<9;i++){
	    	g.setColor(Color.BLUE);
	    	switch(signals[i].getSigStat('n')){
	    	case 'g':
	    		g.setColor(Color.GREEN);
	    		g.fillRect(signals[i].getLocation().x-50, signals[i].getLocation().y-50, 100, 10);
	            break;
            case 'l':
	         case 'r':
	        	 g.setColor(Color.RED);
	        	 g.fillRect(signals[i].getLocation().x-50, signals[i].getLocation().y-50, 100, 10);
	        	break;
        	default:
        		break;
	    	}
	    	switch(signals[i].getSigStat('e')){
	    	case 'g':
	    		g.setColor(Color.GREEN);
	    		g.fillRect(signals[i].getLocation().x+50, signals[i].getLocation().y-50, 10, 100);
	            break;
            case 'l':
	         case 'r':
	        	 g.setColor(Color.RED);
	        	 g.fillRect(signals[i].getLocation().x+50, signals[i].getLocation().y-50, 10, 100);
	        	break;
        	default:
        		break;
	    	}
	    	switch(signals[i].getSigStat('s')){
	    	case 'g':
	    		g.setColor(Color.GREEN);
	    		g.fillRect(signals[i].getLocation().x-50, signals[i].getLocation().y+50, 100, 10);
	            break;
            case 'l':
	         case 'r':
	        	 g.setColor(Color.RED);
	        	 g.fillRect(signals[i].getLocation().x-50, signals[i].getLocation().y+50, 100, 10);
	        	break;
        	default:
        		break;
	    	}
	    	switch(signals[i].getSigStat('w')){
	    	case 'g':
	    		g.setColor(Color.GREEN);
	    		g.fillRect(signals[i].getLocation().x-50, signals[i].getLocation().y-50, 10, 100);
	            break;
            case 'l':
	        case 'r':
	        	 g.setColor(Color.RED);
	        	 g.fillRect(signals[i].getLocation().x-50, signals[i].getLocation().y-50, 10, 100);
	        	break;
        	default:
        		break;
	    	}
	    }

		g.setColor(Color.RED);
		
		for (int i = 0; i < cars.size(); i++) {
			if(cars.get(i).isFinished() == true)
				cars.remove(i);
		}
		
		int total = cars.size();
		
		for (int j = 0; j < total; j++) {
			Point p;
			p = cars.get(j).getPosition();
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
		ReceiveSocket rs = new ReceiveSocket(m);
		
		rs.start();
	
		while (true){
			m.repaint();
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}


