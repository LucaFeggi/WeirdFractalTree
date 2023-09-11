#include "Branch.hpp"

#include <cmath>

Branch::Branch(int StartX, int StartY, float Length, float Angle){
	this->x = StartX + Length*sin(Angle);
	this->y = StartY - Length*cos(Angle);
	this->x = (this->x - StartX)*cos(Angle) - (this->y - StartY)*sin(Angle) + StartX;
	this->y = (this->x - StartX)*sin(Angle) + (this->y - StartY)*cos(Angle) + StartY;
}
