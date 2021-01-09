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
        float read_x() const;
        float read_y() const;
        float read_z() const;
        int set_color_Gray();
        void set_color_RGB();
        int read_r() const;
        int read_b() const;
        int read_g() const;
        void set_z(float value);
        void projection(PJ *P, PJ_COORD coord_wgs84, PJ_COORD coord_lambert93);
        void normalisation(const vector<double> size_MNT, const int picture_lenght, const int picture_head);
        friend istream& operator>>(istream& stream, Point& pt); 
    private:
        float m_x;
        float m_y;
        float m_z;
        int m_r, m_b, m_g;

};
#endif