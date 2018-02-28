/*
* mergesort.cpp
* Contains implementation for merge sort
* First sort in ascending order of rotation speed
* Then in the descending order of the mass flow rate
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#include "mergesort.h"

//Merge sort to put items in ascending order
void merge_sort(QVector<info_point> &intersect){
    if (intersect.size() < 2)
        return;
    int index = intersect.size() / 2;
    QVector<info_point> left;
    QVector<info_point> right;
    for (int i = 0; i < index; i ++)
        left.append(intersect[i]);
    for (int i = index; i < intersect.size(); i ++ )
        right.append(intersect[i]);
    merge_sort(left);
    merge_sort(right);
    merge(left, right, intersect);
}

//Merge left and right subarrays
void merge(QVector<info_point> left, QVector<info_point> right,
                    QVector<info_point>& target){
    int i = 0, j = 0;
    QVector<info_point> source;
    while ((i < left.size()) && (j < right.size())){
        //Arrange in order of rotation speed
        if (compare(left[i],right[j])){
            source.push_back(left[i]);
            i ++;
        }
        else{
            source.push_back(right[j]);
            j++;
        }
    }
    //If left sub array still have elements, directly put them to the back
    while (i < left.size()){
        source.push_back(left[i]);
        i ++;
    }
    //OR If the right sub array have elements
    while (j < right.size()){
        source.push_back(right[j]);
        j ++;
    }
    //Update the merged vector
    target = source;
}

//If the rotation speed is the same, put items in the desending order of the flow rate
bool compare(info_point a, info_point b){
    if (a.return_rot() == b.return_rot()){
        if (a.return_pos_x() >= b.return_pos_x())
            return true;
        else
            return false;
    }
    else if (a.return_rot() < b.return_rot())
        return true;
    else
        return false;
}
