#include "Tanker.h"
#include "Geometry.h"
#include "Utility.h"
#include <iostream>
#include <string>
using namespace std;

int main(){
  try{
  Tanker s("test", Point(0,0));
  Point p = s.get_location();
  if(p.x!=0||p.y!=0){
    cout<< "Error"<<endl;
    exit(0);
  }
  s.set_destination_position_and_speed(Point(1, 1), 1);
  
  if(s.can_move()){
    cout<< "can move"<<endl;
  }else{
    cout<< "can't move"<<endl;
  }
  
  if(s.is_moving()){
    cout<<"is moving"<<endl;
  }else{
    cout<<"not moving"<<endl;
  }
  if(s.is_afloat()){
    cout<<"is floating"<<endl;
  }else{
    cout<<"not floating"<<endl;
  }
  
  Island island("tt", Point(3,4));
  s.set_load_destination(&island);
  
  s.describe();
  
  s.update();
  
  s.describe();
  
  
  }catch(Error & e){
    cout<< e.msg<<endl;
  }
}
