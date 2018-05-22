/* 
 * File:   AdjacencyMatrixUniversalTableManager.hpp
 * Author: same7
 *
 * Created on May 21, 2018, 12:37 PM
 */

#ifndef ADJACENCYMATRIXUNIVERSALTABLEMANAGER_HPP
#define ADJACENCYMATRIXUNIVERSALTABLEMANAGER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <experimental/filesystem>

#include "AdjacencyMatrix.hpp"
#include "UniversalTable.hpp"



class AdjacencyMatrixUniversalTableManager {
private:
    AdjacencyMatrix* adjacencyMatrix = new AdjacencyMatrix();
    UniversalTable* universalTable = new UniversalTable();
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::string verticesDirectory);
    bool loadEdges(std::string edgesDirectory);
    std::vector<int64_t> addVertexProperties(std::string vertexHeaderLine);
    std::vector<int64_t> addEdgeProperties(std::string edgeHeaderLine);
    
};

#endif /* ADJACENCYMATRIXUNIVERSALTABLEMANAGER_HPP */

