package simulation;

import java.util.*;

public class CrossRoadData {
   private RoadData[] crossRoadID =new RoadData[9];
   
   public CrossRoadData(){
      for(int i=0; i<9; i++){
         crossRoadID[i] =new RoadData();
      }
   }
   public void acceptCar(Car cID, int startingP){
      switch(startingP){
      case 1:
         crossRoadID[0].roadID[0].add(cID);
      case 2:
         crossRoadID[1].roadID[0].add(cID);
      case 3:
         crossRoadID[2].roadID[0].add(cID);
      case 4:
         crossRoadID[2].roadID[2].add(cID);
      case 5:
         crossRoadID[5].roadID[2].add(cID);
      case 6:
         crossRoadID[8].roadID[2].add(cID);
      case 7:
         crossRoadID[8].roadID[4].add(cID);
      case 8:
         crossRoadID[7].roadID[4].add(cID);
      case 9:
         crossRoadID[6].roadID[4].add(cID);
      case 10:
         crossRoadID[6].roadID[6].add(cID);
      case 11:
         crossRoadID[3].roadID[6].add(cID);
      case 12:
         crossRoadID[0].roadID[6].add(cID);
      }
   }
   //id로 해당 교차로, 해당 도로를 찾아내고
   public void changeRoad(Car cID, char nextRoute){
      int crossRID =0;
      int rID =0;
      
      for(int i=0; i<9; i++){
         rID =crossRoadID[i].isThereCar(cID);
         if(rID != -1){
            crossRID =i;
            break;
         }
      }
      
      System.out.println("crossID" +crossRID +"roadID" +rID);
   }

   //진행 방향으로 다음 교차로, 다음 도로를 찾아낸다.
   public int whereCrossRoad(int rID){
      
      return 0;
   }
   public int whereRoad(){
      return 0;
   }
   
   public class RoadData{
      private Queue<Car>[] roadID =new LinkedList[9];
      
      public RoadData(){
         for(int i=0; i<9; i++){
            roadID[i] =new LinkedList<Car>();
         }
      }
      
      public int isThereCar(Car cID){
         for(int i=0; i<9; i++){
            if(roadID[i].contains(cID))
               return i;
         }
         return -1;
      }
   
   }
}