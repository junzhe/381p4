#ifndef CONTROLLER_H
#define CONTROLLER_H
/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

/* 
*** This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
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
  View* view_ptr;
  void command_ship_course(Ship* ship_ptr);
  void command_ship_position(Ship* ship_ptr);
  void command_ship_destination(Ship* ship_ptr);
  void command_ship_load_at(Ship* ship_ptr);
  void command_ship_unload_at(Ship* ship_ptr);
  void command_ship_dock_at(Ship* ship_ptr);
  void command_ship_attack(Ship* ship_ptr);
  void command_ship_refuel(Ship* ship_ptr);
  void command_ship_stop(Ship* ship_ptr);
  void command_ship_stop_attack(Ship* ship_ptr);
  void command_view_default();
  void command_view_size();
  void command_view_zoom();
  void command_view_pan();
  void command_view_show();
  void command_model_status();
  void command_model_go();
  void command_model_create();
  void error_handler(const Error& error) const;
};

#endif
