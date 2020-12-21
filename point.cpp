#include "point.h"
#include <iostream>
Point::Point(float lat, float longi, float alti): m_lat(lat),m_longi(longi), m_alti(alti)
{   
    m_x = 0.;
    m_y = 0.;
    m_z = 0.;
}
void Point::projection()
{ 
    //Projection de wgs84 to Lambert93
    //Initialisation
    PJ_CONTEXT *C = PJ_DEFAULT_CTX;
    PJ *P = proj_create_crs_to_crs (C,"EPSG:4326","EPSG:2154",NULL); ;
    PJ* P_for_GIS = proj_normalize_for_visualization(C, P);
    proj_destroy(P);
    P = P_for_GIS;  
    PJ_COORD coord_wgs84, coord_lambert93;
    coord_wgs84 = proj_coord (m_longi, m_lat, 0, 0); 
    //Projection
    coord_lambert93 = proj_trans (P, PJ_FWD, coord_wgs84);
    // cout << coord_lambert93.xyz.x;
    m_x = coord_lambert93.xyz.x;
    m_y = coord_lambert93.xyz.y;
    m_z = m_alti;
    proj_destroy (P);
    proj_context_destroy (C);

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
istream& operator>>(istream& stream, Point& pt)
{

  stream >>pt.m_lat; // recopiage du float
  stream >>pt.m_longi;
  stream >>pt.m_alti;

  return stream; 
}

Point::~Point()
{}
