#include <iostream>
#include <string>
#include "Island.h"
#include "Geometry.h"
using namespace std;

int main(){
  Point p;
  Island is("test", p, 1002.232, -0.0000000001);
  is.provide_fuel(1002.23);
  is.accept_fuel(1000.00);
  is.update();
  is.describe();
}
