#include "pixel.h"
#include <iostream>
Pixel::Pixel(int x, int y): m_x(x),m_y(y)
{   
}

Pixel::Pixel()
{
}

void Pixel::compute_color(float min, float max)
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

void Pixel::set_colorRGB(int r, int g, int b)
{
    if (r>255 || r< 0)
    {
        cout << "Mauvaise valeur de rouge" << endl;
    }   
    else
    {
        m_colorRGB[0] = r;
    }
    if (g>255 || g< 0)
    {
        cout << "Mauvaise valeur de vert" << endl;
    }   
    else
    {
        m_colorRGB[1] = g;
    }
    if (b>255 || b< 0)
    {
        cout << "Mauvaise valeur de bleu" << endl;
    }   
    else
    {
        m_colorRGB[2] = b;
    }
}

void Pixel::compute_colorRGB(float premiere_born_supp, float intervalle, float color_palette_coef[10][4])
{
    //Dans quel intervalle est le z du pixel:
    float born_sup = premiere_born_supp;
    int indice_intervalle = 0;
    for (int i = 1; i<11; i ++)
    {
        if (m_z >born_sup) 
        {
            // Passage à m'intervalle supérieur
            born_sup += intervalle;
            indice_intervalle ++;
        }
    }
    //Selon l'intervalle, 3 fonctions linéaires associées au rouge, vert et bleu
    for (int i = 0; i <3; i++)
    {
        m_colorRGB[i] = abs(color_palette_coef[indice_intervalle][i]*m_z - color_palette_coef[indice_intervalle][3]) ; //ax + b
    }   
    set_colorRGB(m_colorRGB[0],m_colorRGB[1],m_colorRGB[2]);
}

Pixel::~Pixel()
{}
