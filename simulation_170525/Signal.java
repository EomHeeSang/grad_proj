package simulation;

import java.awt.Point;
import java.io.IOException;
import java.net.*;

public class Signal extends Thread {
   // traffic signal physical information
   private Point location;

   // signal logical information
   private SignalView[] sigViews;
   private int currentSignal;
   private int nextSigTime;

   //var network
   static final int REQPORT =5005;
   InetAddress addr_server;
   String str_send;
   String str_recv =null;
   byte[] dataNextSignal =new byte[1024];
   DatagramSocket sock_udp =null;
   DatagramPacket packet_send_udp =null;
   DatagramPacket packet_recv_udp =null;
   
   //var tmp
   static int tmpInt =0;
   public Signal(Point where) {
      //network setting
      try {
         addr_server =InetAddress.getByName("218.101.227.36");
         sock_udp =new DatagramSocket();
      } catch (UnknownHostException e) {
         e.printStackTrace();
      } catch (SocketException e) {
         e.printStackTrace();
      }
      
      location = new Point(where.x, where.y);

      // signal information
      sigViews = new SignalView[4];
      for (int i = 0; i < 4; i++) {
         sigViews[i] = new SignalView();
      }
      //tmp
      sigViews[tmpInt%4].setSigStat('g');
      currentSignal =tmpInt++;
      nextSigTime = 5; // [s]
   }
   
   //run()
   public void run() {
//      int i=0;
      while (true) {
//         requestNextSignal();
//         DataReceiver dataRecver =new DataReceiver();
//         dataRecver.start();
         // changeSigInfo();
//         switch(i){
//         case 0:
//            sigViews[0].sigStat ='g';
//            i=1;
//            break;
//         case 1:
//            sigViews[0].sigStat ='r';
//            i=0;
//            break;
//         }
         
         //static signal change
         sigViews[currentSignal%4].setSigStat('r');
         currentSignal++;
         sigViews[currentSignal%4].setSigStat('g');
         try {
            Thread.sleep(3000);
         } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
         }
      }
   }
   // interface to simulation
   public Point getLocation() {
      return location;
   }
   // interface to car
   char getSigStat(char which) {
      switch(which){
      case 'n':
         return sigViews[0].sigStat;
      case 'e':
         return sigViews[1].sigStat;
      case 's':
         return sigViews[2].sigStat;
      case 'w':
         return sigViews[3].sigStat;
      }
      return 'f';
   }
   //interface to server
   void requestNextSignal(){
      str_send ="!" +currentSignal;
      packet_send_udp =new DatagramPacket(str_send.getBytes(), str_send.getBytes().length, addr_server, REQPORT);
      try {
         sock_udp.send(packet_send_udp);
      } catch (IOException e) {
         // TODO Auto-generated catch block
         e.printStackTrace();
      }
      packet_recv_udp =new DatagramPacket(dataNextSignal, dataNextSignal.length);
      try {
         System.out.println("recv start");
         sock_udp.receive(packet_recv_udp);
         System.out.println("recv");
      } catch (IOException e) {
         // TODO Auto-generated catch block
         e.printStackTrace();
      }
      str_recv =new String(packet_recv_udp.getData());
      
      System.out.println("From server:" + str_recv);
   }
   //iner class
   //socket class
   public class DataReceiver extends Thread {
      public void run(){
         
      }
   }
   //traffic signal class
   public class SignalView {
      char sigStat; // oooo: 직 좌 노 빨: 'g' 'l' 'y' 'r'

      public SignalView() {
         sigStat = 'r';
      };
      //interface to CrossRoad
      public void setSigStat(char stat) {
         sigStat = stat;
      }
   };
}