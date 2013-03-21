#include "Model.h"
#include "Utility.h"
#include "Ship_factory.h"
#include <iostream>
#include <algorithm>
#include <functional>
using std::cout;
using std::endl;
using namespace std;
using namespace std::placeholders;

Model* g_Model_ptr = nullptr;

Model::Model(){
  Island* island_ptr;
  island_ptr = new Island("Exxon", Point(10, 10), 1000, 200);
  sim_object_set.insert(island_ptr);
  island_set.insert(island_ptr);
  island_ptr = new Island("Shell", Point(0, 30), 1000, 200);
  sim_object_set.insert(island_ptr);
  island_set.insert(island_ptr);
  island_ptr=new Island("Bermuda", Point(20, 20));
  sim_object_set.insert(island_ptr);
  island_set.insert(island_ptr);
  Ship* ship_ptr;
  ship_ptr = create_ship("Ajax", "Cruiser", Point (15, 15));
  sim_object_set.insert(ship_ptr);
  ship_set.insert(ship_ptr);
  ship_ptr = create_ship("Xerxes", "Cruiser", Point (25, 25));
  sim_object_set.insert(ship_ptr);
  ship_set.insert(ship_ptr);
  ship_ptr = create_ship("Valdez", "Tanker", Point (30, 30));
  sim_object_set.insert(ship_ptr);
  ship_set.insert(ship_ptr);
  cout << "Model constructed" << endl;
}


Model::~Model(){
  for_each(sim_object_set.begin(), sim_object_set.end(), [](Sim_object* o){delete o;});
  cout << "Model destructed" << endl;
}

bool Model::is_name_in_use(const std::string& name) const{
  return find_if(sim_object_set.begin(), sim_object_set.end(), bind(Sim_object_Name_Compare_with_string(),_1, name))!=sim_object_set.end();
}


bool Model::is_island_present(const std::string& name) const{
  return find_if(island_set.begin(), island_set.end(), bind(Sim_object_Compare_with_string(),_1, name))!= island_set.end();
}

void Model::add_island(Island* island_ptr){
  sim_object_set.insert(island_ptr);
  island_set.insert(island_ptr);
  island_ptr->broadcast_current_state();
}

Island* Model::get_island_ptr(const std::string& name) const{
  auto it = find_if(island_set.begin(), island_set.end(),bind(Sim_object_Compare_with_string(),_1, name));
  if(it==island_set.end()){
    throw Error("Island not found!");
  }
  
  return *it;
}

bool Model::is_ship_present(const std::string& name) const{
  return find_if(ship_set.begin(), ship_set.end(), bind(Sim_object_Compare_with_string(),_1, name))!= ship_set.end();
}

void Model::add_ship(Ship* ship_ptr){
  sim_object_set.insert(ship_ptr);
  ship_set.insert(ship_ptr);
  ship_ptr->broadcast_current_state();
}

Ship* Model::get_ship_ptr(const std::string& name) const{
  auto it = find_if(ship_set.begin(), ship_set.end(),bind(Sim_object_Compare_with_string(),_1, name));
  if(it==ship_set.end()){
    throw Error("Ship not found!");
  }
  
  return *it;
}

void Model::describe() const{
  for_each(sim_object_set.begin(), sim_object_set.end(), [](Sim_object* object){ object->describe();});
}

void Model::update(){
  time++;
  for_each(sim_object_set.begin(), sim_object_set.end(), [](Sim_object* object){ object->update();});
  
  vector<Ship*> ship_remove;
  for_each(ship_set.begin(), ship_set.end(), [&ship_remove](Ship* ship){if(ship->is_on_the_bottom()){ship_remove.push_back(ship);}});
  
  for(Ship* ship:ship_remove){
    notify_gone(ship->get_name());
    sim_object_set.erase(ship);
    ship_set.erase(ship);
    delete ship;
  }
}

void Model::attach(View* view_ptr){
  view_set.insert(view_ptr);
  for_each(sim_object_set.begin(), sim_object_set.end(), [](Sim_object* object){ object->broadcast_current_state();});
}

void Model::detach(View* view_ptr){
  view_set.erase(view_ptr);
}

void Model::notify_location(const std::string& name, Point location){
    for_each(view_set.begin(), view_set.end(), [&name,&location](View* view){ view->update_location(name, location);});
}

void Model::notify_gone(const std::string& name){
    for_each(view_set.begin(), view_set.end(), [&name](View* view){ view->update_remove(name);});
}


