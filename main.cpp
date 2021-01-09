#include <cstdlib>
#include "tool.h"
#include "point.h"
#include <deque>
#include <vector>

using namespace std;
int main(int argc, char *argv[])
{
    deque<Point*> map_points; //vector vide
    vector<double> size_MNT(6);
    deserialize_map(argv[1],map_points,size_MNT); //insertion de tous les points du fichier dans le vector
    create_picture(atoi(argv[2]),size_MNT, map_points);
}       
