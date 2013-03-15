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
  s.set_destination_position_and_speed(Point(10, 10), 1);
  }catch(Error & e){
    cout<< e.msg<<endl;
  }
}
