#ifndef __POINT_H__
#define __POINT_H__

#include <cstdlib>
#include <fstream>
#include <proj.h>
using namespace std;

class Point
{
    public:
        Point(double lat, double longi, double alti); //constructeur
        ~Point();
        int read_x();
        int read_y();
        double read_z();
        void projection(PJ *P, PJ_COORD coord_wgs84, PJ_COORD coord_lambert93);
        // friend ostream& operator<<(ostream& stream, const Point& r);//utile ?
        friend istream& operator>>(istream& stream, Point& pt); 
    // private:
    double m_lat;
    double m_longi;
    double m_alti;
    int m_x;
    int m_y;
    double m_z;

};
#endif