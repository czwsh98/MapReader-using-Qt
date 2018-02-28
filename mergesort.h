/*
* mergesort.h
* Contains the header file for merge sort
* First sort in ascending order of rotation speed
* Then in the descending order of the mass flow rate
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#ifndef MERGESORT_H
#define MERGESORT_H

#include <QVector>
#include "info_point.h"

bool compare(info_point a, info_point b);
void merge(QVector<info_point> left, QVector<info_point> right,
                    QVector<info_point>& target);
void merge_sort(QVector<info_point> &intersect);


#endif // MERGESORT_H
