#include "View.h"
#include "Geometry.h"
#include "Utility.h"
#include <sstream>
#include <cmath>
#include <map>
#include <iostream>
#include <iomanip>
using namespace std;
/* *** Use this function to calculate the subscripts for the cell. */

/* *** This code assumes the specified private member variables. */


// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool View::get_subscripts(int &ix, int &iy, Point location)
{
	// adjust with origin and scale
	Cartesian_vector subscripts = (location - origin) / scale;
	// truncate coordinates to integer after taking the floor
	// floor function will produce integer smaller than even for negative values, 
	// so - 0.05 => -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
		}
	else
		return true;
}

View::View()
:size(25),
scale(2.0),
origin(Point(-10,-10)){
  cout << "View constructed" << endl;
}

View::~View(){
  cout << "View destructed" << endl;
}

void View::update_location(const std::string& name, Point location){
  map[name] = location;
}

void View::update_remove(const std::string& name){
  map.erase(name);
}

void View::clear(){
  map.clear();
}

void View::set_size(int size_){
  if(size_>30){
    throw Error("New map size is too big!");
  }
  if(size_<=6){
    throw Error("New map size is too small!");
  }
  size = size_;
}

void View::set_scale(double scale_){
  if(scale_<=0){
    throw Error("New map scale must be positive!");
  }
  scale = scale_;
}

void View::set_origin(Point origin_){
  origin = origin_;
}

void View::set_defaults(){
  size = 25;
  scale = 2.0;
  origin = Point(-10, -10);
}

void View::draw(){
  matrix.assign(size, vector<string>(size, ". "));
  vector<pair<string, Point>> out_of_map_points;
  for(auto it = map.begin(); it!=map.end(); it++){
    int x= 0,y= 0;
    if(get_subscripts(x,y,(*it).second)){
      if(matrix[y][x][0]=='.'&&matrix[y][x][1]==' '){
	matrix[y][x][0] = (*it).first[0];
	matrix[y][x][1] = (*it).first[1];
      }else{
	matrix[y][x][0] = '*';
	matrix[y][x][1] = ' ';
      }
    }else{
      out_of_map_points.push_back((*it));
    }
  }
  int xw = 0;
  int yw = 0;
  double ox = origin.x;
  double oy = origin.y;
  double upperx = origin.x+scale*(size-1);
  double uppery = origin.y+scale*(size-1);
  stringstream sox, soy, supperx, suppery;
  sox<<ox;
  soy<<oy;
  supperx<<upperx;
  suppery<<uppery;
  
  xw = sox.str().size()>supperx.str().size()?sox.str().size():supperx.str().size();
  yw = soy.str().size()>suppery.str().size()?soy.str().size():suppery.str().size();
 
  cout<<"Display size: "<<size<<", scale: "<<scale<<", origin: ("<<ox<<", "<<oy<<")"<<endl;
  cout.precision(0);
  
  for(int i=size-1;i>=0;i--){
    cout<<" "<<setw(xw);
    if(i%3==0){
      cout<<uppery;
      uppery-=(scale*3);
    }else{
      cout<<" ";
    }
    cout<<" ";
    for(int j=0;j<size;j++){
      cout<<matrix[i][j];
    }
    cout<<endl;
  }
  cout<<setw(xw+3)<<ox;
  ox+=(scale*3);
  for(int i=0;i<(size-1)/3+(size-1)%3!=0?1:0;i++){
    cout<<setw(6)<<ox;
    ox+=(scale*3);
  }
  cout<<endl;
  cout.precision(2);
}
