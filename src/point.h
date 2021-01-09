#ifndef __POINT_H__
#define __POINT_H__

#include <cstdlib>
#include <fstream>
#include <proj.h>
#include <vector>
#include "math.h"
using namespace std;

/**
 * \class Point
 * \brief A point of the MNT
 * \author Hugo Piquard
 * \date 2020
 */



class Point
{
    
    public:
        /**
        * \brief Create a point 
        */
        Point(); 
        ~Point();
        
        /**
        * \brief Read the x value of the Point
        */
        float read_x() const;
        
        /**
        * \brief Read the x value of the Point
        */
        float read_y() const;
        
        /**
        * \brief Read the x value of the Point
        */
        float read_z() const;
        
        /**
        * \brief Compute the gray intensity of the point
        */
        int set_color_Gray();

        /**
        * \brief Compute the RGB color of the point
        */
        void set_color_RGB();
        
        /**
        * \brief Read the red value of the point
        */
        int read_r() const;

        /**
        * \brief Read the blue value of the point
        */
        int read_b() const;

        /**
        * \brief Read the green value of the point
        */
        int read_g() const;

        /**
        * \brief Project WGS84 coordonate's point to lambert 93
        * \param P PJ object
        * \param coord_wgs84 Coordonate to project, PJ_COORD object
        * \param  coord_lambert93 Coordonate projected, PJ_COORD object
        */
        void projection(PJ *P, PJ_COORD coord_wgs84, PJ_COORD coord_lambert93);
        
        /**
        * \brief Normalise the x, y, z values in function of max and min values
        * \param size_MNT Vector with x size, y size and z size, const vector<double> 
        * \param picture_lenght Lenght of the picture, const int
        * \param picture_head  Head of the picture const int
        */
        void normalisation(const vector<double> size_MNT, const int picture_lenght, const int picture_head);
        
        
        /**
        * \brief Overloading of >>
        */
        friend istream& operator>>(istream& stream, Point& pt); 
    private:
        float m_x;
        float m_y;
        float m_z;
        int m_r, m_b, m_g;

};
#endif