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

//Sim_object comparator based on name
struct Sim_object_Compare{
  bool operator() (const Sim_object* o1, const Sim_object* o2){
    return o1->get_name().compare(o2->get_name())<0;
  }
};

//Sim_object comparator for name exist
struct Sim_object_Name_Compare_with_string{
  bool operator() (const Sim_object* o, const string& s){
    string name = o->get_name();
    return name[0]==s[0]&&name[1]==s[1];
  }
};

//Sim_object comparator with a name string
struct Sim_object_Compare_with_string{
  bool operator() (const Sim_object* o, const string& s){
    return o->get_name().compare(s)==0;
  }
};

//Functor for remove a sim object
struct Sim_object_Remove{
  void operator() (Sim_object* o){
    delete o;
  }
};

#endif
