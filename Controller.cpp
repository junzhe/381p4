#include "Controller.h"
#include "View.h"
#include "Ship_factory.h"
#include "Ship.h"
#include "Utility.h"
#include "Model.h"
#include <iostream>
#include <functional>
#include <string>
#include <map>
using namespace std;

Controller::Controller(){
  cout << "Controller constructed" << endl;
  view_ptr = new View();
  g_Model_ptr->attach(view_ptr);
}

Controller::~Controller(){
  cout << "Controller destructed" << endl;
}

void Controller::error_handler(const Error& error) const{
    cout << error.msg << endl;
    cin.clear();
    while ((char)cin.get() != '\n');
    return;
}

void Controller::run(){
  map<string, void (Controller::*)()> func_map;	// map of member func pointer
  map<string, void (Controller::*)(Ship*)> func_ship_map;	//map of member func pointer
  
  func_map["default"] = &Controller::command_view_default;
  func_map["size"] = &Controller::command_view_size;
  func_map["zoom"] = &Controller::command_view_zoom;
  func_map["pan"] = &Controller::command_view_pan;
  func_map["show"] = &Controller::command_view_show;
  func_map["status"] = &Controller::command_model_status;
  func_map["go"] = &Controller::command_model_go;
  func_map["create"] = &Controller::command_model_create;
  func_ship_map["course"] = &Controller::command_ship_course;
  func_ship_map["position"] = &Controller::command_ship_position;
  func_ship_map["destination"] = &Controller::command_ship_destination;
  func_ship_map["load_at"] = &Controller::command_ship_load_at;
  func_ship_map["unload_at"] = &Controller::command_ship_unload_at;
  func_ship_map["dock_at"] = &Controller::command_ship_dock_at;
  func_ship_map["attack"] = &Controller::command_ship_attack;
  func_ship_map["refuel"] = &Controller::command_ship_refuel;
  func_ship_map["stop"] = &Controller::command_ship_stop;
  func_ship_map["stop_attack"] = &Controller::command_ship_stop_attack;
  
  while(true){
    cout << "\nTime " << g_Model_ptr->get_time() << ": Enter command: ";
    string command;
    cin>>command;
    
    try {
      //Is it quit
      if(command.compare("quit")==0){
	g_Model_ptr->detach(view_ptr);
	delete view_ptr;
	cout << "Done" << endl;
	return;
      }
      
      //Is is a ship command
      if(g_Model_ptr->is_ship_present(command)){
	string second_command;
	cin>>second_command;
	Ship* ship_ptr = g_Model_ptr->get_ship_ptr(command);
	if(func_ship_map.find(second_command)==func_ship_map.end()){
	  throw Error("Unrecognized command!");
	}
	(this->*(func_ship_map[second_command]))(ship_ptr);
	continue;
      }

      //Is it a valid command
      if(func_map.find(command)!=func_map.end()){
	(this->*(func_map[command]))();
	continue;
      }else{
	throw Error("Unrecognized command!");
      }
      
    }catch(Error& error){
      error_handler(error);
    }
  }
}

void Controller::command_ship_course(Ship* ship_ptr){
  double heading, speed;
  
  heading = read_double();
  speed = read_double();

  if(heading<0.0||heading>=360.0){
    throw Error("Invalid heading entered!");
  }
  
  if(speed<0.0){
    throw Error("Negative speed entered!");
  }
  
  ship_ptr->set_course_and_speed(heading, speed);
  return;
}

void Controller::command_ship_position(Ship* ship_ptr){
  double x,y, speed;
  x = read_double();
  y = read_double();
  speed = read_double();

  if(speed<0.0){
    throw Error("Negative speed entered!");
  }
  
  ship_ptr->set_destination_position_and_speed(Point(x,y), speed);
  return;
}

void Controller::command_ship_destination(Ship* ship_ptr){
  string island_name;
  double speed;
  
  island_name = read_string();
  speed = read_double();
  
  Island* island_ptr = g_Model_ptr->get_island_ptr(island_name);
  
  ship_ptr->set_destination_position_and_speed(island_ptr->get_location(), speed);
  return;
}

void Controller::command_ship_load_at(Ship* ship_ptr){
  string island_name = read_string();
  
  Island* island_ptr = g_Model_ptr->get_island_ptr(island_name);
  ship_ptr->set_load_destination(island_ptr);
  return;
}


void Controller::command_ship_unload_at(Ship* ship_ptr){
  string island_name = read_string();
  Island* island_ptr = g_Model_ptr->get_island_ptr(island_name);
  ship_ptr->set_unload_destination(island_ptr);
  return;
}

void Controller::command_ship_dock_at(Ship* ship_ptr){
  string island_name = read_string();
  
  Island* island_ptr = g_Model_ptr->get_island_ptr(island_name);
  ship_ptr->dock(island_ptr);
  return;
}

void Controller::command_ship_attack(Ship* ship_ptr){
  string ship_name = read_string();
  
  Ship* ship_ptr_target = g_Model_ptr->get_ship_ptr(ship_name);
  ship_ptr->attack(ship_ptr_target);
  return;
}

void Controller::command_ship_refuel(Ship* ship_ptr){
  ship_ptr->refuel();
}

void Controller::command_ship_stop(Ship* ship_ptr){
  ship_ptr->stop();
}

void Controller::command_ship_stop_attack(Ship* ship_ptr){
  ship_ptr->stop_attack();
}

void Controller::command_view_default(){
  view_ptr->set_defaults();
}

void Controller::command_view_size(){
  int size = read_int();
  
  view_ptr->set_size(size);
}

void Controller::command_view_zoom(){
  double scale;
  scale = read_double();
  
  view_ptr->set_scale(scale);
}

void Controller::command_view_pan(){
  double x,y;
  x =read_double();
  y = read_double();

  view_ptr->set_origin(Point(x,y));
}

void Controller::command_view_show(){
  view_ptr->draw();
}

void Controller::command_model_status(){
  g_Model_ptr->describe();
}

void Controller::command_model_go(){
  g_Model_ptr->update();
}

void Controller::command_model_create(){
  string ship_name, type_name;
  double x,y;
  
  ship_name = read_string();
  
  if(ship_name.size()<2){
    throw Error("Name is too short!");
  }
  
  type_name = read_string();
  
  if(g_Model_ptr->is_name_in_use(ship_name)){
    throw Error("Name is already in use!");
  }
  
  x = read_double();
  y = read_double();
  
  Ship* ship_ptr = create_ship(ship_name, type_name, Point(x,y));
  
  g_Model_ptr->add_ship(ship_ptr);
  return;
}

int Controller::read_int(){
  int result;
  if(!(cin>>result)){
    throw Error("Expected an integer!");
  }

  return result;
}

double Controller::read_double(){
  double result;
  
  if(!(cin>>result)){
    throw Error("Expected a double!");
  }
  
  return result;
}

string Controller::read_string(){
  string result;
  if(!(cin>>result)){
    throw Error("Unrecognized command!");
  }
  return result;
}
