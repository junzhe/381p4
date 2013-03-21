#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Ship.h"
#include "View.h"
#include "Utility.h"
#include <string>
/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/
class Controller {
public:	
	// output constructor message
	Controller();
	// output destructor message
	~Controller();

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();
private:
  View* view_ptr;		// the view object
  
  //command of ship course
  void command_ship_course(Ship* ship_ptr);
  //command of ship positioin
  void command_ship_position(Ship* ship_ptr);
  //command of ship destination
  void command_ship_destination(Ship* ship_ptr);
  //command of ship load
  void command_ship_load_at(Ship* ship_ptr);
  //command of ship unload
  void command_ship_unload_at(Ship* ship_ptr);
  //command of ship dock
  void command_ship_dock_at(Ship* ship_ptr);
  //command of ship attack
  void command_ship_attack(Ship* ship_ptr);
  //command of ship refuel
  void command_ship_refuel(Ship* ship_ptr);
  //command of ship stop
  void command_ship_stop(Ship* ship_ptr);
  //command of ship stop attack
  void command_ship_stop_attack(Ship* ship_ptr);
  //command of default view
  void command_view_default();
  //command of view size
  void command_view_size();
  //command of view zoom
  void command_view_zoom();
  //command of view pan
  void command_view_pan();
  //command of view show
  void command_view_show();
  //command of model status
  void command_model_status();
  //command of model go
  void command_model_go();
  //command of model create
  void command_model_create();
  
  int read_int();
  double read_double();
  std::string read_string();
  
  //Handler error
  void error_handler(const Error& error) const;
};

#endif
