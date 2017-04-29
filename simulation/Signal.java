package simulation;

import java.awt.*;
import java.io.*;
import java.util.*;

public class Signal extends Thread {
	// traffic signal information
	private Point location;

	// signal information
	private char sigInfo;// drive 정의 문서 참조
	private boolean sigOn[];
	private int nextSigTime;

	// road information
	private Realroad road_data;

	public Signal(int where) {
		switch (where) {
		case 0:
			location = new Point(400, 400);
			break;
		default:
			break;
		}

		// signal information

		sigInfo = 0;
		sigOn = new boolean[4];
		sigOn[0] = true;
		for (int i = 1; i < 4; i++) {
			sigOn[i] = false;
		}
		nextSigTime = 5; // [s]

		road_data = new Realroad();
	}

	// traffic signal method
	public Point getLocation() {
		return location;
	}

	// signal information method
	public int getSigInfo() {
		return sigInfo;
	}

	private void changeSigInfo() {
		int currentSig = 0;
		for (int i = 0; i < 4; i++) {
			if (sigOn[i] == true) {
				currentSig = i;
				break;
			}
		}

		sigOn[currentSig] = false;
		
		if(currentSig == 3)
			sigOn[0] = true;
		
		else 
			sigOn[currentSig+1]=true;
		
		sigInfo++;
		sigInfo %= 2;
	}
	
	
	// 차량 신호 수신 테스팅용 sigOn[0]
	public boolean[] getSigState() {
		return sigOn;
	}

	// thread
	public void run() {
		while (true) {
			changeSigInfo();
			try {
				Thread.sleep(nextSigTime * 1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

	}

	// server 기능
	public void commuCar(int where) {
		// road_data.addCar(where);
	}

	// this road data class
	public class Realroad {
		// private int n_north, n_south, n_west, n_east;
		private Queue<Car>[] waitCars;

		public Realroad() {
			// n_north = n_south = n_west = n_east = 0;
			waitCars = new Queue[4];
			for (int i = 0; i < 4; i++) {
				waitCars[i] = new LinkedList<Car>();
			}
		}

		public void addCar(Car c) {
			// switch(where){
			// case 0:
			// n_north++;
			// break;
			// case 3:
			// n_east++;
			// break;
			// case 6:
			// n_south++;
			// break;
			// case 9:
			// n_west++;
			// break;
			// default:
			// break;
			// }

		}

		public int getNextSigTime(int where) {
			switch (where) {
			case 0:
			default:
				return 0;
			}
		}

		public int getNextSigInfo(int sigInfo) {
			return 0;
		}
	}
}