#include "point.h"
#include <iostream>
Point::Point()
{   m_z = 0;
    m_r = 0;
    m_b = 0;
    m_g = 0;
}
void Point::projection(PJ *P, PJ_COORD coord_wgs84, PJ_COORD coord_lambert93)
{ 
    //Projection de wgs84 to Lambert93
    coord_wgs84 = proj_coord (m_x, m_y, 0, 0); 
    //Projection
    coord_lambert93 = proj_trans (P, PJ_FWD, coord_wgs84);
    m_x = coord_lambert93.enu.e;
    m_y = coord_lambert93.enu.n;
}

void Point::normalisation(const vector<double> size_MNT,const int picture_lenght, const int picture_head)
{
  m_x = (m_x -size_MNT[2])*(picture_lenght-1)/(size_MNT[3]- size_MNT[2]);
  m_y = (m_y -size_MNT[4])*(picture_head-1)/(size_MNT[5]-size_MNT[4]);
  if (m_z !=0)
  {
    m_z = (m_z - size_MNT[0])/(size_MNT[1]-size_MNT[0]);
  }
}

float Point::read_x() const
{
  return m_x;
}

float Point::read_y() const
{
  return m_y;
}
float Point::read_z() const
{
  return m_z;
}

int Point::set_color_Gray()
{
  return (int)(255*m_z);
}

void Point::set_color_RGB()
{
  //From https://www.particleincell.com/2014/colormap/
  if (m_z !=0)
    {
      m_z = 1 - m_z;
    }
  double a=(1-m_z)/0.25;
  int  x=(int)floor(a);
  int y = (int)floor(255*(a-x));
  switch(x)
  {
    case 0: m_r=255;m_g=y;m_b=0;break;
    case 1: m_r=255-y;m_g=255;m_b=0;break;
    case 2: m_r=0;m_g=255;m_b=y;break;
    case 3: m_r=0;m_g=255-y;m_b=255;break;
    case 4: m_r=0;m_g=0;m_b=255;break;
  }
}


int Point::read_b() const
{
  return m_b;
}

int Point::read_r() const
{
  return m_r;
}

int Point::read_g() const
{
  return m_g;
}


void Point::set_z(float value)
{
  m_z = value;
}

istream& operator>>(istream& stream, Point& pt)
{

  stream >>pt.m_y >>pt.m_x >> pt.m_z;
  return stream; 
}

Point::~Point()
{}
