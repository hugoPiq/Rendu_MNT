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



void serialize_map_ASCI(const std::string& file_name, Pixel*** picture,const vector<int> size_picture)
{
    ofstream f(file_name); //ouverture fichier
    if(!f.is_open())
        cout << "Impossible d'ouvrir le fichier en écriture" << endl;
    else
    {
        f << "P2" << endl; //nombre magique ASCI
        f << size_picture[0] << " " << size_picture[1] << endl; //dimension
        f << "15" << endl; //valeur blanc
        
        //Ecriture de chaque pixel:
        for (int i = 0; i< size_picture[0] ; i++)
        {//hauteur
            for (int j = 0 ; j < size_picture[1]; j++)
            //longeur
            {
                f << *picture[i][j];
            }
    }
        f.close();
    }
}

void serialize_map_ASCI_color(const std::string& file_name, Pixel*** picture,const vector<int> size_picture)
{
    ofstream f(file_name); //ouverture fichier
    if(!f.is_open())
        cout << "Impossible d'ouvrir le fichier en écriture" << endl;
    else
    {
        f << "P3" << endl; //nombre magique ASCI
        f << size_picture[0] << " " << size_picture[1] << endl; //dimension
        f << "255" << endl; //valeur blanc
        
        //Ecriture de chaque pixel:
        for (int i = 0; i< size_picture[0] ; i++)
        {//hauteur
            for (int j = 0 ; j < size_picture[1]; j++)
            //longeur
            {
                f << *picture[i][j];
                f << "   ";
            }
    }
        f.close();
    }
}


void deserialize_map(const std::string& file_name, deque<Point*> &map_points)
{
    ifstream f(file_name); //ouverture fichier
    if(!f.is_open())
        cout << "Impossible d'ouvrir le fichier en lecture" << endl;
    else
    {
        while(!f.eof()) //eof end of the file
        {
            Point *pt = new Point(0,0,0); //nouveau point accessible par un pointeur
            f >> *pt; //serialisation
            map_points.push_back(pt); //ajout du point a la derniere place du tableau
            
            if(f.eof()) // Pour supprimer la valeur en plus du au retour à la ligne avant la fin du document
            {
                map_points.pop_back(); //Supprime le dernier elt vide
                break; // ...we break the while
            }         
        }
        f.close();
    }
}

void projection(deque<Point*> &map_points, vector<float> &size_MNT)
{
    //En plus de projecter, on détermine les dimensions de la MNT
    int size = map_points.size();
    int length ;
    int head;
    
    //Initialisation projection
    PJ_CONTEXT *C = PJ_DEFAULT_CTX;
    PJ *P = proj_create_crs_to_crs (C,"EPSG:4326","EPSG:2154",NULL); ;
    PJ* P_for_GIS = proj_normalize_for_visualization(C, P);
    proj_destroy(P);
    P = P_for_GIS;  
    PJ_COORD coord_wgs84, coord_lambert93;

     //iniatilsation des premières valeurs
    map_points[0]->projection(P, coord_wgs84, coord_lambert93);
    int head_max = map_points[0]->read_y(); // y ou lat
    int head_min = map_points[0]->read_y();
    int length_min = map_points[0]->read_x(); //x ou alt
    int length_max = map_points[0]->read_x();
    float altitude_min = map_points[0]->read_z();
    float altitude_max = map_points[0]->read_z();

    //parcours de toute le deque
    for (int i = 1 ; i < size; i++)
    // for (deque<Point*>::const_iterator it = map_points.begin(); it != map_points.end(); ++it)
    // for (auto it=map_points.begin(); it!= map_points.end(); ++it)
    {
        map_points[i]->projection(P, coord_wgs84, coord_lambert93);
        // Déterminer la longeur mini
        if (map_points[i]->read_x() < length_min)
        {
            length_min = map_points[i]->read_x();
        }
        //Déterminer la longeur maxi
        if (map_points[i]->read_x() > length_max)
        {
            length_max = map_points[i]->read_x();
        }

        //Déterminer la largeur mini
        if (map_points[i]->read_y() < head_min)
        {
            head_min = map_points[i]->read_y();
        }
        //Déterminer la largeur maxi
        if (map_points[i]->read_y() > head_max)
        {
            head_max = map_points[i]->read_y();
        }
        //Détermine l'altitude la plus haute
        if (map_points[i]->read_z() > altitude_max)
        {
            altitude_max = map_points[i]->read_z();
        }
        //Détermine l'altitude la plus basse
        if (map_points[i]->read_z() < altitude_min)
        {
            altitude_min = map_points[i]->read_z();
        }
    }
    //Largeur de l'image:
    length = length_max - length_min;
    head = head_max - head_min;
    
    size_MNT[0] = length;
    size_MNT[1] = head;
    size_MNT[2] = altitude_min;
    size_MNT[3] = altitude_max;
    size_MNT[4] = length_min; //pour positionner correctement les pixels par rapport aux points
    // cout << length_min;
    size_MNT[5] = head_max; 
    //Désalocation
    proj_destroy (P);
    proj_context_destroy (C);
}   


void point_to_pixel(deque<Point*> &map_points, Pixel *px, const vector<float> size_MNT)
{
    //Sans triangulation
    //Faire correspondre une altitude d'un point à un pixel se trouvant dans la même zone
    //idée dichotomie pour réduire temps d'exécution -> pb :pas triée par ordre croissant
    int size = map_points.size();   
    
    //méthode bourrin  A MODIFIER
    //initialisation
    int indice_pt = 0; //indice du point le plus proche du pixel
    //Regarde quel point a la distance la plus proche du pixel
    float dist_x = px->read_x() - map_points[indice_pt]->read_x();
    float dist_y = px->read_y() - map_points[indice_pt]->read_y();
    float dist = sqrt(pow(dist_x,2) + pow(dist_y, 2));
    for (int i = 1 ; i < size; i++)
    {
        float dist_new_x = px->read_x() - map_points[i]->read_x();
        float dist_new_y = px->read_y() - map_points[i]->read_y();
        float new_dist = sqrt(pow(dist_new_x,2) + pow(dist_new_y, 2));
        if( new_dist < dist)
        {
            dist = new_dist;
            indice_pt = i;
        }
    }
    //Associe le point le plus proche du pixel
    px->set_z(map_points[indice_pt]->read_z());
    px->set_point(map_points[indice_pt]);
    
    //Association de gris
    px->compute_color(size_MNT[2], size_MNT[3]); //associe un gris à l'altitude (z)
}

void point_to_pixel_RGB(deque<Point*> &map_points, Pixel *px, float premiere_borne_sup, float intervalle, float color_palette_coef[10][6])
{
    //Sans triangulation
    //Faire correspondre une altitude d'un point à un pixel se trouvant dans la même zone
    //idée dichotomie pour réduire temps d'exécution -> pb :pas triée par ordre croissant
    int size = map_points.size();   
    
    //méthode bourrin  A MODIFIER
    //initialisation
    int indice_pt = 0; //indice du point le plus proche du pixel
    //Regarde quel point a la distance la plus proche du pixel
    float dist_x = px->read_x() - map_points[indice_pt]->read_x();
    float dist_y = px->read_y() - map_points[indice_pt]->read_y();
    float dist = sqrt(pow(dist_x,2) + pow(dist_y, 2));
    for (int i = 1 ; i < size; i++)
    {
        float dist_new_x = px->read_x() - map_points[i]->read_x();
        float dist_new_y = px->read_y() - map_points[i]->read_y();
        float new_dist = sqrt(pow(dist_new_x,2) + pow(dist_new_y, 2));
        if( new_dist < dist)
        {
            dist = new_dist;
            indice_pt = i;
        }
    }
    //Associe le point le plus proche du pixel
    px->set_z(map_points[indice_pt]->read_z());
    px->set_point(map_points[indice_pt]);
    //Association couleurs
    //Création de la color palette
    px->compute_colorRGB(premiere_borne_sup, intervalle, color_palette_coef); //associe la couleur à l'altitude (z)
}


void create_picture1(Pixel** picture[], int picture_lenght,const vector<float> size_MNT, deque<Point*> &map_points, vector<int> &size_picture)
{
    //Pour la résolution, picture_head = nb de pixel sur la hauteur et  picture_lenght = nb pixel sur la longeur
    //Détermine la hauteur de l'image
    int picture_head = size_MNT[1]*picture_lenght/size_MNT[0];
    size_picture[0] = picture_lenght;
    size_picture[1] = picture_head;
    //Calcul pas d'une position d'un pixel à l'autre
    float dx = size_MNT[0]/picture_lenght; 
    float dy = size_MNT[1]/picture_head; 
    // Créations des pixels de notre image
    for (int i = 0; i< picture_lenght; i++)
    {//hauteur
        picture[i] = new Pixel*[picture_head];
        for (int j = 0 ; j < picture_head; j++)
        //longeur
        {
            float pos_x = dx*i + size_MNT[4] ; //permet de placer les pixels vers la même positions que les points
            float pos_y = dy*j + size_MNT[5];
            Pixel *px = new Pixel(pos_x,pos_y); 
            // //association de chaque pixel avec un point du MNT avec couleurs
            point_to_pixel(map_points,px, size_MNT);
            picture[i][j] = px; //colone/ligne attention
            cout << i << endl;
        }
    }
}   

void create_picture_RGB(Pixel** picture[], int picture_lenght,const vector<float> size_MNT, deque<Point*> &map_points, vector<int> &size_picture, const int color_palette[11][3])
{
    //Pour la résolution, picture_head = nb de pixel sur la hauteur et  picture_lenght = nb pixel sur la longeur
    //Détermine la hauteur de l'image
    int picture_head = size_MNT[1]*picture_lenght/size_MNT[0];
    size_picture[0] = picture_lenght;
    size_picture[1] = picture_head;
    //Calcul pas d'une position d'un pixel à l'autre (pas)
    float dx = size_MNT[0]/picture_lenght; 
    float dy = size_MNT[1]/picture_head; 
    
    //Calcul des coefficients de couleurs de la palette color 
    //Sépare la palette hoxby en 10 intervalles entre chaque couleur principale.
    //Formation de fonctions linéaire pour les 3 couleurs primaires dans chaqu'un de ces intervalles (transition entre chaque couleurs principales)
    float  color_palette_coef[10][6];//coef_lin rouge, coef_lin vert, coef_lin bleu et ordonnée à l'origine
    //Séparation des valeurs comprises entre zmin et zmax en 10 parties (11 couleurs principales ) pour associer chaque interval de couleur à un interval de z
    float diff_z = size_MNT[3] - size_MNT[2];
    float intervalle = diff_z/10 ;
    float premiere_borne_sup = size_MNT[2] + diff_z;
    for (int i = 0; i<10; i ++)
    {
        float born_inf = size_MNT[2] + i*intervalle ;
        float born_supp = size_MNT[2] + (i + 1)*intervalle;
        //Stockage de ces coefficients dans color_palette_coef
        color_palette_coef[i][0] = (color_palette[i+1][0] - color_palette[i][0])/(born_supp-born_inf); //coef_lin rouge
        color_palette_coef[i][1] = (color_palette[i+1][1] - color_palette[i][1])/(born_supp-born_inf); //coef_lin vert
        color_palette_coef[i][2] = (color_palette[i+1][2] - color_palette[i][2])/(born_supp-born_inf); //coef_lin bleue
        color_palette_coef[i][3] = (born_supp*color_palette[i+1][0])/(born_supp-born_inf); //ordonnée à l'origine rouge 
        color_palette_coef[i][4] = (born_supp*color_palette[i+1][1])/(born_supp-born_inf);//ordonnée à l'origine vert
        color_palette_coef[i][5] = (born_supp*color_palette[i+1][2])/(born_supp-born_inf);//ordonnée à l'origine bleu
    }
    
    // Créations des pixels de notre image
    for (int i = 0; i< picture_lenght; i++)
    {//hauteur
        picture[i] = new Pixel*[picture_head];
        for (int j = 0 ; j < picture_head; j++)
        //longeur
        {
            float pos_x = dx*i + size_MNT[4] ; //permet de placer les pixels vers la même positions que les points
            float pos_y = dy*j + size_MNT[5];
            Pixel *px = new Pixel(pos_x,pos_y); 
            // //association de chaque pixel avec un point du MNT et association de couleurs
            point_to_pixel_RGB(map_points,px, premiere_borne_sup, intervalle, color_palette_coef);
            picture[i][j] = px; //colone/ligne attention
            cout << i << endl;
        }
    }
} 


void picture_PGM_v1(const string file_name, int picture_length)
{
    //lecture points fichier
    deque<Point*> map_points; //vector vide
    deserialize_map(file_name,map_points); //insertion de tous les points du fichier dans le vector
    
    //Projecetion coordonates WGS84 to Lambert 93 et détermination taille du MNT
    vector<float> size_MNT(6); // vector contenant la longeur, la largeur, la hauteur max, la hauteur min du MNT et la postion du points en haut à gauche
    projection(map_points, size_MNT);
    
    //triangularisation: à faire plus tard
    //Passage de coordonnées aux pixels d'une image
    //creation image contenant les pixels et association de chaque pixel à un point
    Pixel  ***picture;
    vector<int> size_picture(2); //dimension de l'image, largeur et longeur
    picture = new Pixel**[picture_length]; //tableau de pointeurs de pointeurs sur des pixels
    create_picture1(picture, picture_length, size_MNT, map_points, size_picture);
    //Création du fichier PGM
    serialize_map_ASCI("test_PGM.pgm", picture, size_picture);
    //Désalocation
    delete picture;
}


void picture_PPM(const string file_name, int picture_length)
{
    //lecture points fichier
    deque<Point*> map_points; //vector vide
    deserialize_map(file_name,map_points); //insertion de tous les points du fichier dans le vector
    
    //Projecetion coordonates WGS84 to Lambert 93 et détermination taille du MNT
    vector<float> size_MNT(6); // vector contenant la longeur, la largeur, la hauteur max, la hauteur min du MNT et la postion du points en haut à gauche
    projection(map_points, size_MNT);
    
    //triangularisation: à faire plus tard
    //Passage de coordonnées aux pixels d'une image
    //creation image contenant les pixels et association de chaque pixel à un point
    Pixel  ***picture;
    vector<int> size_picture(2); //dimension de l'image, largeur et longeur
    picture = new Pixel**[picture_length]; //tableau de pointeurs de pointeurs sur des pixels
    
    //Création de la palette color composée de 11 couleurs fixes en RGB
    const int color_palette[11][3] = {{37,57,175},{40,127,251},{50,190,255},{106,235,255},{138,236,174},{205,255,162},{240,236,121},{255,189,87},{255,161,68},{255,186,133},{255,255,255}}; 
    create_picture_RGB(picture, picture_length, size_MNT, map_points, size_picture, color_palette);
    //Création du fichier PGM
    serialize_map_ASCI_color("test_PGM.pgm", picture, size_picture);
    //Désalocation
    delete picture;
}



int main()
{
    //varibles à mettre en paramètres:
    // const string file_name = "test.txt";
    const string file_name = "Guerledan_Feb19_50cm_wgs84.txt";
    int picture_length = 800;
    
    //PGM sans triangulation methode bourrin très long ....
    // picture_PGM_v1(file_name, picture_length);

    //PPM sans triangulation à tester quand plus rapide
    // picture_PPM(file_name, picture_length);
    
    


    /* TESTS:*/
}       
