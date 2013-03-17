#ifndef UTILITIES_H
#define UTILITIES_H
#include <string>
#include "Sim_object.h"
#include "Ship.h"
#include "Warship.h"
#include "Cruiser.h"
#include "Island.h"
using std::string;

struct Error {
	Error(const char * in_msg) : msg(in_msg) {}
	const char * msg;
};

/* add any of your own declarations here */

struct Sim_object_Compare{
  bool operator() (const Sim_object* o1, const Sim_object* o2){
    return o1->get_name().compare(o2->get_name())<0;
  }
};

struct Sim_object_Compare_with_string{
  bool operator() (const Sim_object* o, const string& s){
    return o->get_name().compare(s)==0;
  }
};

struct Sim_object_Remove{
  void operator() (Sim_object* o){
    delete o;
  }
};

#endif
