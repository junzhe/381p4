#include "Ship_factory.h"
#include <string>
#include "Cruiser.h"
#include "Ship.h"
#include "Tanker.h"
#include "Warship.h"
#include "Utility.h"
using std::string;

Ship * create_ship(const std::string& name, const std::string& type, Point initial_position){
  if(type.compare("Cruiser")==0){
    return new Cruiser(name, initial_position);
  }
  
  if(type.compare("Tanker")==0){
    return new Tanker(name, initial_position);
  }
  
  throw Error("Trying to create ship of unknown type!");
}
  
  