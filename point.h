#ifndef __POINT_H__
#define __POINT_H__

#include <cstdlib>
#include <fstream>
#include <proj.h>
#include <vector>
#include "math.h"
using namespace std;

class Point
{
    public:
        Point(); 
        ~Point();
        float read_x();
        float read_y();
        float read_z();
        int set_color_Gray();
        void set_color_RGB();
        int read_r();
        int read_b();
        int read_g();
        void set_z(float value);
        void projection(PJ *P, PJ_COORD coord_wgs84, PJ_COORD coord_lambert93);
        void normalisation(const vector<double> size_MNT, const int picture_lenght, const int picture_head);
        friend istream& operator>>(istream& stream, Point& pt); 
    // private:
    float m_x;
    float m_y;
    float m_z;
    int m_r, m_b, m_g;

};
#endif