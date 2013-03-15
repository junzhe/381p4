#ifndef UTILITIES_H
#define UTILITIES_H
#include <string>
using std::string;

struct Error {
	Error(const char * in_msg) : msg(in_msg) {}
	const char * msg;
};

/* add any of your own declarations here */

class Sim_object_Compare{
  bool operator() (const Sim_object* o1, const Sim_object* o2){
    return o1->get_name().compare(o2->get_name())<0;
  }
};

class Sim_object_Compare_with_string{
  bool operator() (const Sim_object* o, const string& s){
    return o->get_name().compare(s)==0;
  }
};

class Sim_object_Remove{
  void operator() (Sim_object* o){
    delete o;
  }
};

#endif
