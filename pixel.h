#ifndef __PIXEL_H__
#define __PIXEL_H__

#include <cstdlib>
#include <fstream>
#include "point.h"
#include <vector>
#include <deque>
using namespace std;

class Pixel
{
    public:
    Pixel(int x, int y); //constructeur
    Pixel();
    //accesseur
    float read_x();
    float read_y();
    float read_z();
    int read_color();
    // void set_color(int r, int g, int b);
    //mutateur
    void set_x(float x);
    void set_y(float y);
    void set_z(float z);
    void set_color(float min, float max);
    void set_point(Point *pt);

    ~Pixel();
    friend ostream& operator<<(ostream& stream, const Pixel& px); //serialiser
    // private:
    float m_x;
    float m_y;
    float m_z; 
    int m_color;
    // vector<int> m_colorRGB();
    Point *m_point = NULL;
};
#endif