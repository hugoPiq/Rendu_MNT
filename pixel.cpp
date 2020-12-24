#include "pixel.h"
#include <iostream>
Pixel::Pixel(int x, int y): m_x(x),m_y(y)
{   
    m_nb_point = 0;
}

Pixel::Pixel()
{
}

void Pixel::compute_color(double min, double max)
{
    if (m_nb_point == 0)
    {
        m_color = 0;
    }
    else
    {
        int value_white = 15; //valeur arbitraire de blanc 
        int value_black = 0;
        double coef_lin = (value_white - value_black)/(max-min); //calcul du coef directeur de la fonction linéaire reliant la couleur à la profondeur
        m_color = abs(m_z*coef_lin - (max*value_white)/(max-min)); //entier
    }
}
ostream& operator<<(ostream& stream, const Pixel& px)
{
  //ATTENTION: A CHANGER SI PGM OU PPM
  //PGM
//   cout << "Méthode PGM" << endl;
  stream << px.m_color << " ";
  //PPM
//   cout << "Méthode PPM" << endl;
//   stream << px.m_colorRGB[0] << " ";
//   stream << px.m_colorRGB[1] << " ";
//   stream << px.m_colorRGB[2] << " ";


  return stream;
}

int Pixel::read_x()
{
    return m_x;
}


int Pixel::read_y()
{
    return m_y;
}


double Pixel::read_z()
{
    return m_z;
}


int Pixel::read_color()
{
    return m_color;
}

void Pixel::set_x(int x)
{
    m_x = x;
}

void Pixel::set_y(int y)
{
    m_y = y;
}

int Pixel::read_nb_point()
{
    return m_nb_point;
}

void Pixel::set_z()
{
    // Attribution du z:
    // Si un ou plusieurs points: moyenne des altitudes de ces points  
    double moyenne = 0.; 
    for ( int i=0 ; i < m_nb_point; i++)
    {
        moyenne += m_point[i]->read_z();
    }
    moyenne = moyenne/m_nb_point;
    m_z = moyenne;
}

void Pixel::set_point(Point *pt)
{
    //Ajout du point à la liste de point du pixel
    m_nb_point ++;
    Point** inter = new Point*[m_nb_point];
    for ( int i=0 ; i < m_nb_point-1; i++)
    {
        inter[i] = m_point[i];
    }
    inter[m_nb_point-1] = pt;
    delete[] m_point;
    m_point = inter;
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

void Pixel::compute_colorRGB(double premiere_born_supp, double intervalle, double color_palette_coef[10][6])
{
    //Dans quel intervalle est le z du pixel:
    double born_sup = premiere_born_supp;
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
        m_colorRGB[i] = abs(color_palette_coef[indice_intervalle][i]*m_z - color_palette_coef[indice_intervalle][i+3]) ; //ax + b
    }   
    set_colorRGB(m_colorRGB[0],m_colorRGB[1],m_colorRGB[2]);
}

Pixel::~Pixel()
{}
