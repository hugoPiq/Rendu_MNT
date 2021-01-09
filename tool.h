#ifndef __TOOL_H__
#define __TOOL_H__
#include <vector>
#include <deque>
#include "point.h"
#include <iostream>
#include <ostream>
#include <fstream>


void serialize_map_binaire(int picture_lenght, int picture_head,Point*** picture);
void create_picture(int picture_lenght,const vector<double> size_MNT, deque<Point*> &map_points);
void serialize_map_ASCII(int picture_lenght, int picture_head,Point*** picture);
void serialize_map_gray(int picture_lenght, int picture_head,Point*** picture);
void deserialize_map(const std::string& file_name, deque<Point*> &map_points, vector<double> &size_MNT);


#endif