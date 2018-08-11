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
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <dirent.h>

#include "AdjacencyMatrix.hpp"
#include "UniversalTable.hpp"



class AdjacencyMatrixUniversalTableManager {
private:
    AdjacencyMatrix adjacencyMatrix;
    UniversalTable universalTable;
    int batchSize;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::string verticesDirectory);
    bool loadEdges(std::string edgesDirectory);
    std::vector<int16_t> addVertexProperties(std::string vertexHeaderLine);
    std::vector<int16_t> addEdgeProperties(std::string edgeHeaderLine);
    
    AdjacencyMatrixUniversalTableManager(int bSize):batchSize(bSize){
        adjacencyMatrix.setBatchSize(bSize);
    };
    
};

#endif /* ADJACENCYMATRIXUNIVERSALTABLEMANAGER_HPP */

