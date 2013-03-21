#include "Tanker.h"
#include "Utility.h"
#include <iostream>
using std::cout;
using std::endl;

Tanker::Tanker(const std::string& name_, Point position_)
:Ship(name_,position_,100,10,2,0),
tanker_state(NO_CARGO_DESTINATIONS),
cargo_capacity(1000.0),
cargo(0.0),
island_load(nullptr),
island_unload(nullptr)
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
  
  if(island_ptr == island_unload){
    throw Error("Load and unload cargo destinations are the same!");
  }
  
  island_load = island_ptr;
  
  cout << get_name() << " will load at " << island_load->get_name() << endl;
  
  load_unload_cycle();
}

void Tanker::set_unload_destination(Island* island_ptr){
  if(tanker_state!=NO_CARGO_DESTINATIONS){
    throw Error("Tanker has cargo destinations!");
  }
  
  if(island_ptr == island_load){
    throw Error("Load and unload cargo destinations are the same!");
  }
  
  island_unload = island_ptr;
  
  cout << get_name() << " will unload at " << island_unload->get_name() << endl;
  
  load_unload_cycle();
}

void Tanker::load_unload_cycle(){
  if(island_load!=nullptr&&island_unload!=nullptr){
    if(is_docked()){
      if(get_docked_Island()==island_load){
	tanker_state = LOADING;
	return;
      }
      if(get_docked_Island()==island_unload){
	tanker_state = UNLOADING;
	return;
      }
    }
    if(!is_moving()){
      if(cargo==0&&can_dock(island_load)){
	dock(island_load);
	tanker_state = LOADING;
	return;
      }
      if(cargo>0&&can_dock(island_unload)){
	dock(island_unload);
	tanker_state = UNLOADING;
	return;
      }
    }
    
    if(cargo==0.0){
      Ship::set_destination_position_and_speed(island_load->get_location(), get_maximum_speed());
      tanker_state = MOVING_TO_LOADING;
      return;
    }
    
    if(cargo>0.0){
      Ship::set_destination_position_and_speed(island_unload->get_location(), get_maximum_speed());
      tanker_state = MOVING_TO_UNLOADING;
      return;
    }
  }
}

void Tanker::stop(){
  Ship::stop();
  island_load = nullptr;
  island_unload = nullptr;
  tanker_state = NO_CARGO_DESTINATIONS;
  cout << get_name() << " now has no cargo destinations" << endl;
}

void Tanker::describe() const{
  cout << "\nTanker ";
  Ship::describe();
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
  cout<<endl;
}

void Tanker::update(){
  Ship::update();
  if(!can_move()){
    tanker_state = NO_CARGO_DESTINATIONS;
    island_load = nullptr;
    island_unload = nullptr;
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
	//set unload destination
	Ship::set_destination_position_and_speed(island_unload->get_location(), get_maximum_speed());
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
	//set load destination
	Ship::set_destination_position_and_speed(island_load->get_location(), get_maximum_speed());
	tanker_state = MOVING_TO_LOADING;
      }else{
	get_docked_Island()->accept_fuel(cargo);
	cargo = 0;
      }
      break;
    }
    case MOVING_TO_LOADING:{
      if(!is_moving()){
	dock(island_load);
	tanker_state = LOADING;
      }
      break;
    }
    case MOVING_TO_UNLOADING:{
      if(!is_moving()){
	dock(island_unload);
	tanker_state = UNLOADING;
      }
      break;
    }
  }
}
