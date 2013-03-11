#include "Geometry.h"

/*
Define the destructor function even if it was declared as a pure virtual function.
This seems odd, because pure virtual functions are usually not defined in the class
that declares them. But this is often done as a way to make a class abstract, 
if there is no other virtual function that makes sense to mark as pure. Here we
are defining it just to get the destructor message output.
*/

Ship::~Ship()
{
	cout << "Ship "  << get_name() << " destructed" << endl;
}

/* Private Function Definitions */

/*
Calculate the new position of a ship based on how it is moving, its speed, and
fuel state. This function should be called only if the state is 
MOVING_TO_POSITION or MOVING_ON_COURSE.

Track_base has an update_position(double time) function that computes the new position
of an object after the specified time has elapsed. If the Ship is going to move
for a full time unit (one hour), then it will get go the "full step" distance,
so update_position would be called with time = 1.0. If we can move less than that,
e.g. due to not enough fuel, update position  will be called with the corresponding
time less than 1.0.

For clarity in specifying the computation, this code assumes the specified private variable names, 
but you may change the variable names or state names if you wish (e.g. movement_state).
*/
void Ship:: calculate_movement()
{
	// Compute values for how much we need to move, and how much we can, and how long we can,
	// given the fuel state, then decide what to do.
	double time = 1.0;	// "full step" time
	// get the distance to destination
	double destination_distance = cartesian_distance(get_location(), destination);
	// get full step distance we can move on this time step
	double full_distance = get_speed() * time;
	// get fuel required for full step distance
	double full_fuel_required = full_distance * fuel_consumption;	// tons = nm * tons/nm
	// how far and how long can we sail in this time period based on the fuel state?
	double distance_possible, time_possible;
	if(full_fuel_required <= fuel) {
		distance_possible = full_distance;
		time_possible = time;
		}
	else {
		distance_possible = fuel / fuel_consumption;	// nm = tons / tons/nm
		time_possible = (distance_possible / full_distance) * time;
		}
	
	// are we are moving to a destination, and is the destination within the distance possible?
	if(ship_state == MOVING_TO_POSITION && destination_distance <= distance_possible) {
		// yes, make our new position the destination
		set_position(destination);
		// we travel the destination distance, using that much fuel
		double fuel_required = destination_distance * fuel_consumption;
		fuel -= fuel_required;
		set_speed(0.);
		ship_state = STOPPED;
		}
	else {
		// go as far as we can, stay in the same movement state
		// simply move for the amount of time possible
		update_position(time_possible);
		// have we used up our fuel?
		if(full_fuel_required >= fuel) {
			fuel = 0.0;
			ship_state = DEAD_IN_THE_WATER;
			}
		else {
			fuel -= full_fuel_required;
			}
		}
}

Ship::Ship(const std::string& name_, Point position_, double fuel_capacity_, 
		double maximum_speed_, double fuel_consumption_, int resistance_)
  :Sim_object(name),
   Track_base(position_),
   fuel_capacity(fuel_capacity_),
   maximum_speed(maximum_speed_),
   fuel_consumption(fuel_consumption_),
   resistance(resistance_)
{
  cout << "Ship " << Sim_object::get_name()<< " constructed" << endl;
}

bool Ship::can_move() const{
  return ship_state!=DEAD_IN_THE_WATER||(ship_state!=SINKING&&ship_state!=SUNK&&ship_state!=ON_THE_BOTTOM);
}

bool Ship::is_moving() const{
  return ship_state==MOVING_TO_POSITION||ship_state==MOVING_ON_COURSE;
}

bool Ship::is_docked() const{
  return ship_state==DOCKED;
}

bool Ship::is_afloated() const{
  return ship_state==DOCKED||ship_state==STOPPED||ship_state==MOVING_TO_POSITION||ship_state==MOVING_ON_COURSE;
}

bool Ship::is_on_the_bottom() const{
  return ship_state==ON_THE_BOTTOM;
}

bool Ship::can_dock(Island* island_ptr) const{
  Point cur_point = get_position();
  Point island_point = island_ptr->get_location();
  
  double dist = cartesian_distance(cur_point, island_point);
  return dist<0.1;
}

void Ship::update(){
  if(is_afloated()){
    if(resistance>=0){
      switch(ship_state){
	case MOVING_ON_COURSE:
	case MOVING_TO_POSITION:
	  caculate_movement();
	  cout << get_name()<< " now at " << get_position()<< endl;
	  //TODO Model::notify_location();
	  break;
	case STOPPED:
	  cout << get_name()<< " stopped at " << get_position()<< endl;
	  break;
	case DOCKED:
	  cout << get_name() << " docked at " << get_location() << endl;
	  break;
	case DEAD_IN_THE_WATER:
	  cout << get_name() << " dead in the water at " << get_position()<< endl;
	  break;
      }
      return;
    }else{
      ship_state = SINKING;
      cout <<get_name()<< " sinking" << endl;
      return;
    }
  }else{
      switch(ship_state){
	case SINKING:
	  ship_state = SUNK;
	  cout<<get_name()<<"sunk"<<endl;
	  //TODO Model::notify_gone();
	  break;
	case SUNK:
	  ship_state = ON_THE_BOTTOM;
	  cout<<get_name()<<"on the bottom"<<endl;
	  break;
	case ON_THE_BOTTOM:
	  cout<<get_name()<<"on the bottom"<<endl;
	  break;
      }
      return;
  }
}

void Ship::describe() const{
  cout << get_name() <<cout << << " hit with " << << ", resistance now " << << endl; " at " << get_position();
  if(is_afloated()){
    cout << ", fuel: " << fuel << " tons" << ", resistance: " << resistance << endl;
    switch(ship_state){
      case MOVING_TO_POSITION:
	cout << "Moving to " << destination << " on " << get_course() << endl;
	break;
      case MOVING_ON_COURSE:
	cout << "Moving on " << get_course() << endl;
	break;
      case STOPPED:
	cout << "Stopped" << endl;
	break;
      case DOCKED:
	cout << "Docked at " << get_position()<< endl;
	break;
      case DEAD_IN_THE_WATER:
	cout << "Dead in the water" << endl;
	break;
    }
  }else{
    switch(ship_state){
      case SINKING:
	cout << " sinking" << endl;
	break;
      case SUNK:
	cout << " sunk" << endl;
	break;
      case ON_THE_BOTTOM:
	cout << " on the bottom" << endl;
	break;
    }
  }
  return;
}


void Ship::broadcast_current_state(){
  //TODO Model::notify_location();
}

void Ship::set_destination_position_and_speed(Point destination_position, double speed){
  is(!can_move()){
    throw Error("Ship cannot move!");
  }
  
  if(speed>maximum_speed){
    throw Error("Ship cannot go that fast!");
  }
  
  destination = destination_position;
  Point cur = get_position();
  Compass_vector cv(cur, destination);
  
  set_speed(speed);
  set_course(cv.direction);
  ship_state = MOVING_TO_POSITION;
  cout << get_name() << " will sail on " << get_course() << " to " << destination<< endl;
  return;
}

void Ship::set_course_and_speed(double course, double speed){
  is(!can_move()){
    throw Error("Ship cannot move!");
  }
  
  if(speed>maximum_speed){
    throw Error("Ship cannot go that fast!");
  }
  
  set_course(course);
  set_speed(speed);
  
  ship_state = MOVING_ON_COURSE;
  cout << get_name() << " will sail on " << get_course() << endl;
  
  return;
}

void Ship::stop(){
  if(!can_move()){
    throw Error("Ship cannot move!");
  }
  
  set_speed(0);
  cout << get_name() << " stopping at " << get_position()<< endl;
  ship_state = STOPPED;
  return;
}

void Ship::dock(Island * island_ptr){
  if(ship_state!=STOPPED){
    throw Error("Can't dock!");
  }
  if(!can_dock(island_ptr){
    throw rror("Can't dock!");
  }
  
  ship_state = DOCKED;
  set_position(island_ptr->get_location());
  island_docked = island_ptr;
  cout<<get_name()<< " docked at " << get_position()<< endl;
  return;
}

void Ship::refuel(){
  if(!is_docked()){
    throw Error("Must be docked!");
  }
  double required_fuel = fuel_capacity - fuel;
  if(required_fuel<0.005){
    fuel = fule_capacity;
    return;
  }else{
    double fuel_get = island_docked->provide_fuel(required_fuel);
    fuel+=fule_get;
    cout << get_name() << " now has " << fuel << " tons of fuel" << endl;
  }
  return;
}

void Ship::set_load_destination(Island *){
  throw Error("Cannot load at a destination!");
}

void Ship::set_unload_destination(Island *){
  throw Error("Cannot unload at a destination!");
}

void Ship::attack(Ship * in_target_ptr){
  throw Error("Cannot attack!");
}

void Ship::stop_attack(){
  throw Error("Cannot attack!");
}
  
void Ship::receive_hit(int hit_force, Ship* attacker_ptr){
  resistance-=hit_force;
  cout << get_name() << " hit with " << hit_force << ", resistance now " << resistance<< endl;
  return;
}


