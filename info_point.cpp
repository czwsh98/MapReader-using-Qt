/*
* info_point.cpp
* Contains the implementation for a customized class that stores intersection information
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#include "info_point.h"

info_point::info_point(){
    //Default constructor is empty
}

info_point::info_point(int rot, double eff, double x, double y){
    set_rotation(rot);
    set_efficiency(eff);
    set_pos_x(x);
    set_pos_y(y);
}

void info_point::set_rotation(int rot){
    rotation = rot;
}

void info_point::set_efficiency(double eff){
    efficiency = eff;
}

void info_point::set_pos_x(double x){
    pos_x = x;
}

void info_point::set_pos_y(double y){
    pos_y = y;
}

int info_point::return_rot(){
    return rotation;
}

double info_point::return_eff(){
    return efficiency;
}

double info_point::return_pos_x(){
    return pos_x;
}

double info_point::return_pos_y(){
    return pos_y;
}
