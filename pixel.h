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
    Pixel(); //constructeur
    //accesseur
    double read_z();
    int read_color();
    int read_nb_point();
    void set_colorRGB(int r, int g, int b);
    //mutateur
    void set_z(double value);
    void compute_color(double min, double max);
    void compute_colorRGB(double premiere_born_supp, double intervalle, double color_palette[10][6]);
    void set_point(Point *pt);

    ~Pixel();
    friend ostream& operator<<(ostream& stream, const Pixel& px); //serialiser
    // private:
    int m_nb_point;
    double m_z; 
    int m_color;
    int m_colorRGB[3] = {0,0,0};
    Point **m_point = NULL;
};
#endif