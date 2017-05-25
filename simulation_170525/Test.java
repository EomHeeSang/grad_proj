package simulation;

import java.io.*;
import java.net.*;

public class Test {
   static final int PORT = 5004;
   static Socket sock_;
   static BufferedOutputStream ostream;
   static InetAddress addr_s;
   static String str;
   static DatagramSocket dsock = null;
   static DatagramPacket dpack = null;
   public static void main(String[] args){
      str ="testing.......";
      try{
         addr_s = InetAddress.getByName("192.168.0.44");
         sock_ = new Socket(addr_s, PORT);
         dsock =new DatagramSocket();
         System.out.println(addr_s);
         
      }catch(Exception e){
         
      }
      
      while(true){
         try{
            dpack =new DatagramPacket(str.getBytes(), str.getBytes().length, addr_s, PORT);
            dsock.send(dpack);
            ostream = new BufferedOutputStream(sock_.getOutputStream());
            ostream.write("testing...".getBytes());
            Thread.sleep(1000);
            System.out.println("send");
         }catch(Exception e){
            e.printStackTrace();
         }
      }
      
   }
}