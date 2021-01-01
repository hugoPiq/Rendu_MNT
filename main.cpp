#include <cstdlib>
#include <iostream>
#include <ostream>
#include <fstream>
#include "point.h"
#include <deque>
#include <vector>
#include <cmath>
#include <list>
#include "pixel.h"

using namespace std;


void deserialize_map(const std::string& file_name, deque<Point*> &map_points, vector<double> &size_MNT)
{
    ifstream f(file_name); //ouverture fichier
    bool debut = true;
    float xmin, xmax, ymin, ymax, alt_min, alt_max;
    
    //Initialisation projection
    PJ_CONTEXT *C = PJ_DEFAULT_CTX;
    PJ *P = proj_create_crs_to_crs (C,"EPSG:4326","EPSG:2154",NULL); ;
    PJ* P_for_GIS = proj_normalize_for_visualization(C, P);
    proj_destroy(P);
    P = P_for_GIS;  
    PJ_COORD coord_wgs84, coord_lambert93;
    
    if(!f.is_open())
        cout << "Impossible d'ouvrir le fichier en lecture" << endl;
    else
    {
        while(!f.eof()) //eof end of the file
        {
            Point *pt = new Point();
            f >> *pt; //serialisation
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
    
    
    //Désalocation
    proj_destroy (P);
    proj_context_destroy (C);
    cout << "Fin projection" << endl;
}


void create_picture(int picture_lenght,const vector<double> size_MNT, deque<Point*> &map_points)
{
    int picture_head = (int)(abs(size_MNT[3] - size_MNT[2]) * picture_lenght / abs(size_MNT[5] - size_MNT[4]));
    int **picture= new int*[picture_head];
    for (int i = 0; i< picture_head; i++)
    {
        picture[i] = new int[picture_lenght];
        for (int j = 0 ; j < picture_lenght; j++) 
        {
            picture[i][j] = 0; //initialisation des pixels en noir
        }
    }
    
    int size = map_points.size();
    for (int k = 0; k< size; k++) //Pour chaque point k
    {      
        map_points[k]->normalisation(size_MNT, picture_lenght, picture_head); 
        int i = map_points[k]->read_y();
        int j = map_points[k]->read_x();
        picture[i][j] = (int)(255*map_points[k]->read_z()); //Attribution color
        //Comment supprimer les pointeurs ?
    }
    cout << "Fin attribution point" << endl;  
    
    fstream f("test_PGM.pgm"); //ouverture fichier
    if(!f.is_open())
        cout << "Impossible d'ouvrir le fichier en écriture" << endl;
    else
    {
        f << "P2" << endl; //nombre magique ASCI
        f << picture_lenght << " " << picture_head << endl; //dimension
        f << "255" << endl; //valeur blanc
        
        //Ecriture de chaque pixel:
        for (int i = picture_head-1; i>-1  ; i--)
        {//hauteur
            for (int j = 0 ; j < picture_lenght; j++)
            //longeur
            {
                f << picture[i][j] << " ";
            }
        }
        delete picture;
        f.close();
        cout << "Fin sérialisation" << endl;
    }
}        

    
     
void picture_PGM(const string file_name, int picture_length)
{
    //lecture  des points du fichier et projection en Lambert 93
    deque<Point*> map_points; //vector vide
    vector<double> size_MNT(6);
    deserialize_map(file_name,map_points,size_MNT); //insertion de tous les points du fichier dans le vector
    create_picture(picture_length,size_MNT, map_points);
}


//################################### R G B ##########################################################




// void point_to_pixel_RGB(deque<Point*> &map_points, Pixel *px, double premiere_borne_sup, double intervalle, double color_palette_coef[10][6])
// {
//     //Sans triangulation
//     //Faire correspondre une altitude d'un point à un pixel se trouvant dans la même zone
//     //idée dichotomie pour réduire temps d'exécution -> pb :pas triée par ordre croissant
//     int size = map_points.size();   
    
//     //méthode bourrin  A MODIFIER
//     //initialisation
//     int indice_pt = 0; //indice du point le plus proche du pixel
//     //Regarde quel point a la distance la plus proche du pixel
//     double dist_x = px->read_x() - map_points[indice_pt]->read_x();
//     double dist_y = px->read_y() - map_points[indice_pt]->read_y();
//     double dist = sqrt(pow(dist_x,2) + pow(dist_y, 2));
//     for (int i = 1 ; i < size; i++)
//     {
//         double dist_new_x = px->read_x() - map_points[i]->read_x();
//         double dist_new_y = px->read_y() - map_points[i]->read_y();
//         double new_dist = sqrt(pow(dist_new_x,2) + pow(dist_new_y, 2));
//         if( new_dist < dist)
//         {
//             dist = new_dist;
//             indice_pt = i;
//         }
//     }
//     //Associe le point le plus proche du pixel
//     // px->set_z(map_points[indice_pt]->read_z());
//     px->set_point(map_points[indice_pt]);
//     //Association couleurs
//     //Création de la color palette
//     px->compute_colorRGB(premiere_borne_sup, intervalle, color_palette_coef); //associe la couleur à l'altitude (z)
// }




// void create_picture_RGB(Pixel** picture[], int picture_lenght,const vector<double> size_MNT, deque<Point*> &map_points, vector<int> &size_picture, const int color_palette[11][3])
// {
//     //Pour la résolution, picture_head = nb de pixel sur la hauteur et  picture_lenght = nb pixel sur la longeur
//     //Détermine la hauteur de l'image
//     int picture_head = size_MNT[1]*picture_lenght/size_MNT[0];
//     size_picture[0] = picture_lenght;
//     size_picture[1] = picture_head;
//     //Calcul pas d'une position d'un pixel à l'autre (pas)
//     double dx = size_MNT[0]/picture_lenght;  //dimension d'un pixel
//     double dy = size_MNT[1]/picture_head; 
    
//     //Calcul des coefficients de couleurs de la palette color 
//     //Sépare la palette hoxby en 10 intervalles entre chaque couleur principale.
//     //Formation de fonctions linéaire pour les 3 couleurs primaires dans chaqu'un de ces intervalles (transition entre chaque couleurs principales)
//     double  color_palette_coef[10][6];//coef_lin rouge, coef_lin vert, coef_lin bleu et ordonnée à l'origine
//     //Séparation des valeurs comprises entre zmin et zmax en 10 parties (11 couleurs principales ) pour associer chaque interval de couleur à un interval de z
//     double diff_z = size_MNT[3] - size_MNT[2];
//     double intervalle = diff_z/10 ;
//     double premiere_borne_sup = size_MNT[2] + diff_z;
//     for (int i = 0; i<10; i ++)
//     {
//         double born_inf = size_MNT[2] + i*intervalle ;
//         double born_supp = size_MNT[2] + (i + 1)*intervalle;
//         //Stockage de ces coefficients dans color_palette_coef
//         color_palette_coef[i][0] = (color_palette[i+1][0] - color_palette[i][0])/(born_supp-born_inf); //coef_lin rouge
//         color_palette_coef[i][1] = (color_palette[i+1][1] - color_palette[i][1])/(born_supp-born_inf); //coef_lin vert
//         color_palette_coef[i][2] = (color_palette[i+1][2] - color_palette[i][2])/(born_supp-born_inf); //coef_lin bleue
//         color_palette_coef[i][3] = (born_supp*color_palette[i+1][0])/(born_supp-born_inf); //ordonnée à l'origine rouge 
//         color_palette_coef[i][4] = (born_supp*color_palette[i+1][1])/(born_supp-born_inf);//ordonnée à l'origine vert
//         color_palette_coef[i][5] = (born_supp*color_palette[i+1][2])/(born_supp-born_inf);//ordonnée à l'origine bleu
//     }
    
//     // Créations des pixels de notre image
//     for (int i = 0; i< picture_lenght; i++)
//     {//hauteur
//         picture[i] = new Pixel*[picture_head];
//         for (int j = 0 ; j < picture_head; j++)
//         //longeur
//         {
//             double pos_x = dx*i + size_MNT[4] ; //permet de placer les pixels vers la même positions que les points
//             double pos_y = dy*j + size_MNT[5];
//             Pixel *px = new Pixel(pos_x,pos_y); 
//             // //association de chaque pixel avec un point du MNT et association de couleurs
//             point_to_pixel_RGB(map_points,px, premiere_borne_sup, intervalle, color_palette_coef);
//             picture[i][j] = px; //colone/ligne attention
//             cout << i << endl;
//         }
//     }
// } 

// void picture_PPM(const string file_name, int picture_length)
// {
//     //lecture points fichier
//     deque<Point*> map_points; //vector vide
//     deserialize_map(file_name,map_points); //insertion de tous les points du fichier dans le vector
    
//     //Projecetion coordonates WGS84 to Lambert 93 et détermination taille du MNT
//     vector<double> size_MNT(6); // vector contenant la longeur, la largeur, la hauteur max, la hauteur min du MNT et la postion du points en haut à gauche
//     projection(map_points, size_MNT);
    
//     //triangularisation: à faire plus tard
//     //Passage de coordonnées aux pixels d'une image
//     //creation image contenant les pixels et association de chaque pixel à un point
//     Pixel  ***picture;
//     vector<int> size_picture(2); //dimension de l'image, largeur et longeur
//     picture = new Pixel**[picture_length]; //tableau de pointeurs de pointeurs sur des pixels
    
//     //Création de la palette color composée de 11 couleurs fixes en RGB
//     const int color_palette[11][3] = {{37,57,175},{40,127,251},{50,190,255},{106,235,255},{138,236,174},{205,255,162},{240,236,121},{255,189,87},{255,161,68},{255,186,133},{255,255,255}}; 
//     // create_picture_RGB(picture, picture_length, size_MNT, map_points, size_picture, color_palette);
//     //Création du fichier PGM
//     serialize_map_ASCI_color("test_PGM.pgm", picture, size_picture);
//     //Désalocation
//     delete picture;
// }



int main()
{
    //Nb de points : 2681710
    //varibles à mettre en paramètres:
    // const string file_name = "test.txt";
    const string file_name = "Guerledan_Feb19_50cm_wgs84.txt";
    int picture_length = 1707;
    
    //PGM 
    picture_PGM(file_name, picture_length);

    //PPM sans triangulation à tester quand plus rapide
    // picture_PPM(file_name, picture_length);
    
    


    /* TESTS:*/
}       
