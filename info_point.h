/*
* info_point.h
* Contains the header file for a customized class that stores intersection information
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#ifndef INFO_POINT_H
#define INFO_POINT_H


class info_point
{
public:
    info_point();
    info_point(int rot, double eff, double x, double y);
    void set_rotation(int rot);
    void set_efficiency(double eff);
    void set_pos_y(double y);
    void set_pos_x(double x);
    int return_rot();
    double return_eff();
    double return_pos_x();
    double return_pos_y();

private:
    int rotation;
    double efficiency;
    double pos_x;
    double pos_y;
};

#endif // INFO_POINT_H
