#ifndef __POINT_H__
#define __POINT_H__

#include <cstdlib>
#include <fstream>
#include <proj.h>
using namespace std;

class Point
{
    public:
        Point(float lat, float longi, float alti); //constructeur
        ~Point();
        float read_x();
        float read_y();
        float read_z();
        void projection(PJ *P, PJ_COORD coord_wgs84, PJ_COORD coord_lambert93);
        // friend ostream& operator<<(ostream& stream, const Point& r);//utile ?
        friend istream& operator>>(istream& stream, Point& pt); 
    // private:
    float m_lat;
    float m_longi;
    float m_alti;
    float m_x;
    float m_y;
    float m_z;

};
#endif