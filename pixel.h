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
    int read_x();
    int read_y();
    double read_z();
    int read_color();
    void set_colorRGB(int r, int g, int b);
    //mutateur
    void set_x(int x);
    void set_y(int y);
    void set_z(double z);
    void compute_color(double min, double max);
    void compute_colorRGB(double premiere_born_supp, double intervalle, double color_palette[10][6]);
    void set_point(Point *pt);

    ~Pixel();
    friend ostream& operator<<(ostream& stream, const Pixel& px); //serialiser
    // private:
    int m_x;
    int m_y;
    double m_z; 
    int m_color;
    int m_colorRGB[3] = {0,0,0};
    Point *m_point = NULL;
};
#endif