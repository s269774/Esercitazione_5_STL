#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include "map"
#include "Utils.hpp"
#include "PolygonalMesh.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{    
    PolygonalMesh mesh;

    string filepath = "PolygonalMesh";
    if(!ImportMesh(filepath,
                    mesh))
    {
        return 1;
    }

    testMarkers(mesh);
    testLength(mesh);
    testArea(mesh);

    cout << "All tests passed successfully" << endl;

    return 0;
}
