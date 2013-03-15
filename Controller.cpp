#include "Controller.h"
#include "View.h"
#include "Ship_factory.h"
#include <iostream>
#include <string>
#include <map>
using namespace std;

Controller::Controller(){
  view_ptr = new View();
  g_Model_ptr->attach(view_ptr);
  cout << "Controller constructed" << endl;
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
  map<string, void (*)()> func_map;
  
  while(true){
    cout << "\nTime " << g_Model_ptr->get_time() << ": Enter command: ";
    string command;
    cin>>command;
    
    try {
      if(command.compare("quit")==0){
	g_Model_ptr->detach(view_ptr);
	delete view_ptr;
	cout << "Done" << endl;
	return;
      }
      
      if(g_Model_ptr->is_ship_present(command)){
	string second_command;
	cin<<second_command;
	Ship* ship_ptr = g_Model_ptr->get_ship_ptr(second_command);
	if(func_map.find(command)==func_map.end()){
	  throw Error("Unrecognized command!");
	}
	func_map[second_command](ship_ptr);
	continue;
      }

      if(func_map.find(command)!=func_map.end()){
	func_map[command]();
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
  double heading;
  double speed;
  
  if(!(cin>>heading)){
    throw Error("Expected a double!");
  }
  
  if(!(cin>>speed)){
    throw Error("Expected a double!");
  }

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
  if(!(cin>>x)){
    throw Error("Expected a double!");
  }
  
  if(!(cin>>y)){
    throw Error("Expected a double!");
  }
  
  if(!(cin>>speed)){
    throw Error("Expected a double!");
  }

  if(speed<0.0){
    throw Error("Negative speed entered!");
  }
  
  ship_ptr->set_destination_position_and_speed(Position(x,y), speed);
  return;
}

void Controller::command_ship_destination(Ship* ship_ptr){
  string island_name;
  double speed;
  if(!(cin>>island_name)){
    throw Error("Unrecognized command!");
  }
  
  if(!(cin>>speed)){
    throw Error("Expected a double!");
  }
  
  Island* island_ptr = g_Model_ptr->get_island_ptr(island_name);
  
  ship_ptr->set_destination_position_and_speed(island_ptr->get_location(), speed);
  return;
}

void Ship::command_ship_load_at(Ship* ship_ptr){
  string island_name;
  if(!(cin>>island_name)){
    throw Error("Unrecognized command!");
  }
  Island* island_ptr = g_Model_ptr->get_island_ptr(island_name);
  ship_ptr->set_load_destination(island_ptr);
  return;
}


void Controller::command_ship_unload_at(Ship* ship_ptr){
  string island_name;
  if(!(cin>>island_name)){
    throw Error("Unrecognized command!");
  }
  Island* island_ptr = g_Model_ptr->get_island_ptr(island_name);
  ship_ptr->set_unload_destination(island_ptr);
  return;
}

void Controller::command_ship_dock_at(Ship* ship_ptr){
  string island_name;
  if(!(cin>>island_name)){
    throw Error("Unrecognized command!");
  }
  Island* island_ptr = g_Model_ptr->get_island_ptr(island_name);
  ship_ptr->dock(island_ptr);
  return;
}

void Controller::command_ship_attack(Ship* ship_ptr){
  string ship_name;
  if(!(cin>>ship_name)){
    throw Error("Unrecognized command!");
  }
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
  int size;
  if(!(cin>>size)){
    throw Error("Expected an integer!");
  }
  view_ptr->set_size(size);
}

void Controller::command_view_zoom(){
  double scale;
  if(!(cin>>scale)){
    throw Error("Expected a double!");
  }
  
  view_ptr->set_scale(scale);
}

void Controller::command_view_pan(){
  double x,y;
  if(!(cin>>x)){
    throw Error("Expected a double!");
  }
  
  if(!(cin>>y)){
    throw Error("Expected a double!");
  }
  
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
  string ship_name;
  double x,y;
  if(!(cin>>ship_name)){
    throw Error("Unrecognized command!");
  }
  
  if(ship_name.size()<2){
    throw Error("Name is too short!");
  }
  
  if(g_Model_ptr->is_ship_present(ship_name)){
    throw Error("Name is already in use!") 
  }
 
  if(!(cin>>x)){
    throw Error("Expected a double!");
  }
  
  if(!(cin>>y)){
    throw Error("Expected a double!");
  }
  
  Ship* ship_ptr = create_ship(ship_name, Point(x,y));
  
  g_Model_ptr->add_ship(ship_ptr);
  return;
}
