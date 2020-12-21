#include "pixel.h"
#include <iostream>
Pixel::Pixel(int x, int y): m_x(x),m_y(y)
{   
}

Pixel::Pixel()
{
}

void Pixel::set_color(float min, float max)
{
    int value_white = 15; //valeur arbitraire de blanc 
    int value_black = 0;
    float coef_lin = (value_white - value_black)/(max-min); //calcul du coef directeur de la fonction linéaire reliant la couleur à la profondeur
    m_color = abs(m_z*coef_lin - (max*value_white)/(max-min)); //entier
    
}
ostream& operator<<(ostream& stream, const Pixel& px)
{
  stream << px.m_color << " ";
  return stream;
}

float Pixel::read_x()
{
    return m_x;
}


float Pixel::read_y()
{
    return m_y;
}


float Pixel::read_z()
{
    return m_z;
}


int Pixel::read_color()
{
    return m_color;
}

void Pixel::set_x(float x)
{
    m_x = x;
}

void Pixel::set_y(float y)
{
    m_y = y;
}

void Pixel::set_z(float z)
{
    m_z = z;
}

void Pixel::set_point(Point *pt)
{
    m_point = pt;
}

Pixel::~Pixel()
{}
