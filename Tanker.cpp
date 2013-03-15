#include "Tanker.h"
#include "Utility.h"
#include <iostream>
using std::cout;
using std::endl;

Tanker::Tanker(const std::string& name_, Point position_)
:Ship(name_,position_,100,10,2,0),
tanker_state(NO_CARGO_DESTINATIONS)
{
  cout << "Tanker " << get_name() << " constructed" << endl;
}

Tanker::~Tanker(){
  cout << "Tanker " << get_name() << " destructed" << endl;
}
  
  
void Tanker::set_destination_position_and_speed(Point destination, double speed){
  if(tanker_state!=NO_CARGO_DESTINATIONS){
    throw Error("Tanker has cargo destinations!");
  }
  
  Ship::set_destination_position_and_speed(destination, speed);
}

void Tanker::set_course_and_speed(double course, double speed){
  if(tanker_state!=NO_CARGO_DESTINATIONS){
    throw Error("Tanker has cargo destinations!");
  }
  
  Ship::set_course_and_speed(course, speed);
}

void Tanker::set_load_destination(Island* island_ptr){
  if(tanker_state!=NO_CARGO_DESTINATIONS){
    throw Error("Tanker has cargo destinations!");
  }
  
  island = island_ptr;
  
  if(island != get_docked_Island()){
    throw Error("Load and unload cargo destinations are the same!");
  }
  
  cout << get_name() << " will load at " << island->get_name() << endl;

  switch(get_ship_state()){
    case DOCKED:
      tanker_state = LOADING;
      break;
    case STOPPED:
      if(cargo==0){
	set_ship_state(DOCKED);
	tanker_state = LOADING;
      }
      break;
    default:
      if(cargo==0){
	Ship::set_destination_position_and_speed(island->get_location(), get_maximum_speed());
	tanker_state = MOVING_TO_LOADING;
      }
      break;
  }
}

void Tanker::set_unload_destination(Island* island_ptr){
  if(tanker_state!=NO_CARGO_DESTINATIONS){
    throw Error("Tanker has cargo destinations!");
  }
  
  island = island_ptr;
  
  if(island != get_docked_Island()){
    throw Error("Load and unload cargo destinations are the same!");
  }
  
  cout << get_name() << " will unload at " << island->get_name() << endl;

  switch(get_ship_state()){
    case DOCKED:
      tanker_state = UNLOADING;
      break;
    case STOPPED:
      if(cargo>0){
	set_ship_state(DOCKED);
	tanker_state = UNLOADING;
      }
      break;
    default:
      if(cargo>0){
	Ship::set_destination_position_and_speed(island->get_location(), get_maximum_speed());
	tanker_state = MOVING_TO_UNLOADING;
      }
      break;
  }
}

void Tanker::stop(){
  Ship::stop();
  island = nullptr;
  tanker_state = NO_CARGO_DESTINATIONS;
  cout << get_name() << " now has no cargo destinations" << endl;
}

void Tanker::describe() const{
  Ship::describe();
  cout << "\nTanker ";
  cout << "Cargo: " << cargo << " tons";
  
  switch(tanker_state){
    case NO_CARGO_DESTINATIONS:
      cout << ", no cargo destinations";
      break;
    case LOADING:
      cout << ", loading";
      break;
    case UNLOADING:
      cout << ", unloading";
      break;
    case MOVING_TO_LOADING:
      cout << ", moving to loading destination";
      break;
    case MOVING_TO_UNLOADING:
      cout << ", moving to unloading destination";
      break;
  }
}

void Tanker::update(){
  Ship::update();
  if(!can_move()){
    tanker_state = NO_CARGO_DESTINATIONS;
    island = nullptr;
    cout << get_name() << " now has no cargo destinations" << endl;
  }
  switch(tanker_state){
    case NO_CARGO_DESTINATIONS:
      break;
    case LOADING:{
      Ship::refuel();
      double required_cargo = cargo_capacity-cargo;
      if(required_cargo<0.005){
	cargo = cargo_capacity;
	set_destination_position_and_speed(island->get_location(), get_maximum_speed());
	tanker_state = MOVING_TO_UNLOADING;
      }else{
	double cargo_get = get_docked_Island()->provide_fuel(required_cargo);
	cargo+=cargo_get;
	cout <<  get_name() << " now has " << cargo << " of cargo" << endl;
      }
      break;
    }
    case UNLOADING:{
      if(cargo==0.0){
	Ship::set_destination_position_and_speed(island->get_location(), get_maximum_speed());
	tanker_state = MOVING_TO_LOADING;
      }else{
	get_docked_Island()->accept_fuel(cargo);
	cargo = 0;
      }
      break;
    }
    case MOVING_TO_LOADING:{
      if(!is_moving()){
	set_ship_state(DOCKED);
	tanker_state = LOADING;
      }
      break;
    }
    case MOVING_TO_UNLOADING:{
      if(!is_moving()){
	set_ship_state(DOCKED);
	tanker_state = UNLOADING;
      }
      break;
    }
  }
}
