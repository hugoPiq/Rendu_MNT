#include <cstdlib>
#include "tool.h"
#include "point.h"
#include <deque>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    //deque containing each point of the MNT
    deque<Point*> map_points;
    //dimension: x size, y size, z size,  of the MNT
    vector<double> size_MNT(6);
    deserialize_map(argv[1],map_points,size_MNT); 
    create_picture(atoi(argv[2]),size_MNT, map_points);
}       
