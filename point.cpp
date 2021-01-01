#include "point.h"
#include <iostream>
Point::Point(double lat, double longi, double alti): m_lat(lat),m_longi(longi), m_alti(alti)
{   
    m_x = 0;
    m_y = 0;
    m_z = 0.;
}
void Point::projection(PJ *P, PJ_COORD coord_wgs84, PJ_COORD coord_lambert93)
{ 
    //Projection de wgs84 to Lambert93
    coord_wgs84 = proj_coord (m_longi, m_lat, 0, 0); 
    //Projection
    coord_lambert93 = proj_trans (P, PJ_FWD, coord_wgs84);
    m_x = coord_lambert93.xyz.x;
    m_y = coord_lambert93.xyz.y;
    // m_x = coord_lambert93.enu.e;
    // m_y = coord_lambert93.enu.n;
    m_z = m_alti;
}

void Point::normalisation(const vector<double> size_MNT, vector<int> &size_picture)
{
  // m_x = (m_x -size_MNT[4])*size_picture[0]/size_MNT[0];
  // m_y = (m_y -size_MNT[5])*size_picture[1]/size_MNT[1];
  m_x = (m_x -size_MNT[4]); 
  m_y = (m_y -size_MNT[5]);
}

int Point::read_x()
{
  return m_x;
}

int Point::read_y()
{
  return m_y;
}
double Point::read_z()
{
  return m_z;
}
istream& operator>>(istream& stream, Point& pt)
{

  stream >>pt.m_lat; // recopiage du double
  stream >>pt.m_longi;
  stream >>pt.m_alti;

  return stream; 
}

Point::~Point()
{}
