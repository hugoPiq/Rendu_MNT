#include "tool.h"

void deserialize_map(const std::string& file_name, deque<Point*> &map_points, vector<double> &size_MNT)
{
    ifstream f(file_name);
    bool debut = true;
    float xmin, xmax, ymin, ymax, alt_min, alt_max;
    
    //Projection initialization
    PJ_CONTEXT *C = PJ_DEFAULT_CTX;
    PJ *P = proj_create_crs_to_crs (C,"EPSG:4326","EPSG:2154",NULL); ;
    PJ* P_for_GIS = proj_normalize_for_visualization(C, P);
    proj_destroy(P);
    P = P_for_GIS;  
    PJ_COORD coord_wgs84, coord_lambert93;
    
    //Open file
    if(!f.is_open())
        cout << "Impossible d'ouvrir le fichier en lecture" << endl;
    else
    {
        while(!f.eof()) 
        {
            Point *pt = new Point();
            
            //Serialisation
            f >> *pt; 
            //Projection
            pt->projection(P, coord_wgs84, coord_lambert93);
            if (debut)
            {
                xmin = pt->read_x();
                xmax = pt->read_x();
                ymin = pt->read_y();
                ymax = pt->read_y();
                alt_min = pt->read_z();
                alt_max = pt->read_z();
                debut = false;
            }
            else
            {
                //Find extremums
                if (pt->read_z() < alt_min)
                {   
                    alt_min = pt->read_z();
                }
                if (pt->read_z()> alt_max)
                {   
                    alt_max = pt->read_z();
                }
                if (pt->read_x() < xmin)
                {   
                    xmin = pt->read_x();
                }
                if (pt->read_x() > xmax)
                {   
                    xmax = pt->read_x();
                }                    
                if (pt->read_y() < ymin)
                {   
                    ymin = pt->read_y();
                }
                if (pt->read_y() > ymax)
                {   
                    ymax = pt->read_y();
                }
            }
            map_points.push_back(pt); 
        }
        f.close();
        cout << "Fin désérialisation" << endl;
    }

    size_MNT[0] = alt_min;
    size_MNT[1] = alt_max;
    size_MNT[2] = xmin; 
    size_MNT[3] = xmax; 
    size_MNT[4] = ymin; 
    size_MNT[5] = ymax; 
    //Dealocation
    proj_destroy (P);
    proj_context_destroy (C);
    cout << "End projection" << endl;
}


void serialize_map_gray(int picture_lenght, int picture_head,Point*** picture)
{
    //Open file
    ofstream f("Projection_MNT_gray_ASCII.pgm"); 
    if(!f.is_open())
        cout << "Impossible d'ouvrir le fichier en écriture" << endl;
    else
    {
        f << "P2" << endl; //Magic number
        f << picture_lenght << " " << picture_head << endl; //dimension
        f << "255" << endl; //White value
        
        //Writting of each point
        for (int i = picture_head-1; i>-1  ; i--)
        {
            for (int j = 0 ; j < picture_lenght; j++)
            {
                f << picture[i][j]->set_color_Gray() << " ";
            }
        }
        f.close();
    }
}


void serialize_map_binaire(int picture_lenght, int picture_head,Point*** picture)
{
    ofstream f("Projection_MNT_color_bin.ppm");
    if(!f.is_open())
        cout << "Impossible d'ouvrir le fichier en écriture" << endl;
    else
    {
        f << "P6" << endl; //Magic number
        f << picture_lenght << " " << picture_head << endl; //dimension
        f << "255" << endl; //White value
        
        //Writting of each point
        for (int i = picture_head-1; i>-1  ; i--)
        {
            for (int j = 0 ; j < picture_lenght; j++)
            {
                int r = picture[i][j]->read_r();
                int g = picture[i][j]->read_g();
                int b = picture[i][j]->read_b();
                f << (uint8_t)r<< (uint8_t)g << (uint8_t)b;
            }
        }
        f.close();
    }
}


void serialize_map_ASCII(int picture_lenght, int picture_head,Point*** picture)
{
    ofstream f("Projection_MNT_color_ASCII.ppm"); 
    if(!f.is_open())
        cout << "Impossible d'ouvrir le fichier en écriture" << endl;
    else
    {
        f << "P3" << endl; 
        f << picture_lenght << " " << picture_head << endl;
        f << "255" << endl;
        for (int i = picture_head-1; i>-1  ; i--)
        {
            for (int j = 0 ; j < picture_lenght; j++)
            {
                int r = picture[i][j]->read_r();
                int g = picture[i][j]->read_g();
                int b = picture[i][j]->read_b();
                f <<" "<< r<< " "<< g <<" " << b << " ";
            }
        }
        
        f.close();
    }
}

void create_picture(int picture_lenght,const vector<double> size_MNT, deque<Point*> &map_points)
{
    //Compute head of the picture
    int picture_head = (int)(abs(size_MNT[3] - size_MNT[2]) * picture_lenght / abs(size_MNT[5] - size_MNT[4]));
    //Tab containing each point (pixel) of the picture
    Point ***picture= new Point**[picture_head];
    for (int i = 0; i< picture_head; i++)
    {
        picture[i] = new Point*[picture_lenght];
        for (int j = 0 ; j < picture_lenght; j++) 
        {
            //Each box contains at the beginning a pixel of zero altitude
            Point *pt = new Point();
            picture[i][j] = pt;
        }
    }
    //Association of each point of the MNT to the boxes of the table
    int size = map_points.size();
    for (int k = 0; k< size; k++)
    {      
        map_points[k]->normalisation(size_MNT, picture_lenght, picture_head); 
        int i = map_points[k]->read_y();
        int j = map_points[k]->read_x();
        picture[i][j] = map_points[k];
        // picture[i][j]->set_color_Gray();
        picture[i][j]->set_color_RGB();
    }
    map_points.clear();
    cout << "End of point allocation" << endl;
    // serialize_map_gray(picture_lenght,picture_head, picture);
    serialize_map_binaire(picture_lenght,picture_head, picture);
    // serialize_map_ASCII(picture_lenght,picture_head, picture);
    cout << "End of serialization" << endl;
    cout << "File created: Projection_MNT_color_bin.ppm" << endl;
    delete[] picture;
}        

int use(int argc)
{
    int a = 0;
    a = argc - argc;
    return a;
}