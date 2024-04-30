#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

namespace PolygonalLibrary {

bool ImportMesh(const string& filepath,
                PolygonalMesh &mesh)
{

    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
            {
                cout << "\t" << id;
            }

            cout << endl;
        }
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
            {
                cout << "\t" << id;
            }

            cout << endl;
        }
    }

    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        // Test:
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c];
            unsigned int ne = edges.size(); // the c-edges are many as the dimension of the vector

            for(unsigned int e = 0; e < ne; e++) // check all the edges of the c-component
            {
                const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
                const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
                if(findOrigin == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if(findEnd == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

            }
        }
    }
    return true;
}


bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }

    file.close();

    listLines.pop_front(); //il primo elemento della lista di stringhe è la stringa d'intestazione, la rimuovo

    mesh.NumberCell0D = listLines.size(); //ogni riga rappresenta una cella

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >>  id >> marker >> coord(0) >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }
    file.close();
    return true;
}


bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }

    file.close();

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >>  id >> marker >> vertices(0) >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }
    file.close();
    return true;
}


bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int numvertices;
        unsigned int numedges;

        converter >>  id >> marker >> numvertices;
        vector<unsigned int> vertices(numvertices) ;

        for(unsigned int i = 0; i < numvertices; i++)
            converter >> vertices[i];

        converter >> numedges;
        vector<unsigned int> edges(numedges);

        for(unsigned int i = 0; i < numedges; i++)
            converter >> edges[i];

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);
    }
    file.close();
    return true;
}

bool testMarkers(const PolygonalMesh& mesh)
{
    if(!mesh.Cell0DMarkers.empty()) //check if the the vector with the markers isn't empty
        return true;
    if(!mesh.Cell1DMarkers.empty())
      return true;
    if(!mesh.Cell2DMarkers.empty())
      return true;
    return true;
}

bool testLength(const PolygonalMesh& mesh)
{
    for (const auto& edges : mesh.Cell2DEdges)
    {
        for (unsigned int edge : edges)
        {
            unsigned int origin = mesh.Cell1DVertices[edge][0]; // Get edge vertices
            unsigned int end = mesh.Cell1DVertices[edge][1];


            double length = (mesh.Cell0DCoordinates[end] - mesh.Cell0DCoordinates[origin]).norm(); // Calculate edge length with matrix operator

            if(length > 0)
                continue;
            else
                break;

        }
    }
    return true;
}


bool testArea(const PolygonalMesh& mesh)
{
    for (const auto& vertices : mesh.Cell2DVertices)
    {
        if (vertices.size() != 3) // Skip polygons that are not triangles
            continue;

        // Get triangle vertices
        Vector2d v0 = mesh.Cell0DCoordinates[vertices[0]];
        Vector2d v1 = mesh.Cell0DCoordinates[vertices[1]];
        Vector2d v2 = mesh.Cell0DCoordinates[vertices[2]];


        Vector2d side1 = v1 - v0;
        Vector2d side2 = v2 - v0;
        double area = 0.5 * std::abs(side1[0] * side2[1] - side1[1] * side2[0]); //Area with matrix operation

        if(area > 0)
            continue;
    }
    return true;
}

}

