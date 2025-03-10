#pragma once

#include <iostream>

#include "PolygonalMesh.hpp"

using namespace std;


namespace PolygonalLibrary{

// Import the Polygonal mesh and test if the mesh is correct
// mesh: a PolygonalMesh struct
// return the result of the reading, true if is success, false otherwise
bool ImportMesh(const string &filepath, PolygonalMesh &mesh);

// Import the Cell0D properties from Cell0Ds.csv file
// mesh: a PolygonalMesh struct
// return the result of the reading, true if is success, false otherwise
bool ImportCell0Ds(const string &filename,
                     PolygonalMesh& mesh);

// Import the Cell1D properties from Cell1Ds.csv file
// mesh: a PolygonalMesh struct
// return the result of the reading, true if is success, false otherwise
bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh);

// Import the Cell2D properties from Cell2Ds.csv file
// mesh: a PolygonalMesh struct
// return the result of the reading, true if is success, false otherwise
bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh);

// Test for check if the markers have been stored correctly
bool testMarkers(const PolygonalMesh& mesh);

// Test for check if the edges of the triangles have non-zero length
bool testLength(const PolygonalMesh& mesh);

// Test for check if the area of the triangles is non-zero
bool testArea(const PolygonalMesh& mesh);

}
