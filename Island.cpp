#include "Island.h"
#include <iostream>
using std::cout;
using std::endl;

Island::Island(const std::string& name_, Point position_, double fuel_, double production_rate_)
  :Sim_object(name_),
   position(position_),
   fuel(fuel_),
   production_rate(production_rate_)
{
  cout << "Island " << Sim_object::get_name() << " constructed" << endl;
}

Island::~Island(){
  cout << "Island " << Sim_object::get_name() << " destructed" << endl;
}

double Island::provide_fuel(double request){
  double supplied_amount = request>fuel?fuel:request;
  fuel-=supplied_amount;
  cout << "Island " << Sim_object::get_name() << " supplied " << supplied_amount << " tons of fuel" << endl;
  return supplied_amount;
}

void Island::accept_fuel(double amount){
  fuel+=amount;
  cout << "Island " << Sim_object::get_name() << " now has " << fuel << " tons" << endl;
  return;
}

void Island::update(){
  if(production_rate>0){
    fuel+=production_rate;
    cout << "Island " << Sim_object::get_name() << " now has " << fuel << " tons" << endl;
  }
  return;
}

void Island::describe() const{
  cout << "\nIsland " << Sim_object::get_name()<< " at position " << position << endl;
  cout << "Fuel available: " << fuel << " tons" << endl;
  return;
}

