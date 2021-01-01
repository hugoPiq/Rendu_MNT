#include "point.h"
#include <iostream>
Point::Point()
{   m_z = 0;
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
  m_z = (m_z - size_MNT[0])/(size_MNT[1]-size_MNT[0]);
}

float Point::read_x()
{
  return m_x;
}

float Point::read_y()
{
  return m_y;
}
float Point::read_z()
{
  return m_z;
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
