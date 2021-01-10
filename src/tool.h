#ifndef __TOOL_H__
#define __TOOL_H__
#include <vector>
#include <deque>
#include "point.h"
#include <iostream>
#include <ostream>
#include <fstream>

/**
* \brief Saving each pixel of Picture in a ppm file.
* \param picture_lenght, int
* \param picture_head, int
* \param picture, Point ***
*/
void serialize_map_binaire(int picture_lenght, int picture_head,Point*** picture);

/**
* \brief Create a tab Picture containing each pixel of the MNT and serialize it.
* \param picture_lenght lenght of the picture, int
* \param size_MNT dimension: x size, y size, z size, vector<double>
* \param map_points deque containing each point of the MNT, deque<Point*>
*/
void create_picture(int picture_lenght,const vector<double> size_MNT, deque<Point*> &map_points);

/**
* \brief Saving each pixel of Picture in a ppm file in ASCII.
* \param picture_lenght, int
* \param picture_head, int
* \param picture, Point ***
*/
void serialize_map_ASCII(int picture_lenght, int picture_head,Point*** picture);

/**
* \brief Saving each pixel of Picture in a pgm file in gray and ASCII.
* \param picture_lenght, int
* \param picture_head, int
* \param picture, Point ***
*/
void serialize_map_gray(int picture_lenght, int picture_head,Point*** picture);

/**
* \brief Saving data from MNT file into a deque.
* \param file_name name of the MNT file, const std::string&
* \param map_points deque containing each point of the MNT, deque<Point*> &map_points
* \param size_MNT dimension: x size, y size, z size of the MNT, vector<double>
*/
void deserialize_map(const std::string& file_name, deque<Point*> &map_points, vector<double> &size_MNT);

int use(int argc);
#endif
