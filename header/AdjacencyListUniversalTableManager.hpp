/* 
 * File:   AdjacencyListUniversalTableManager.hpp
 * Author: same7
 *
 * Created on May 23, 2018, 01:01 AM
 */

#ifndef ADJACENCYLISTUNIVERSALTABLEMANAGER_HPP
#define ADJACENCYLISTUNIVERSALTABLEMANAGER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <experimental/filesystem>

#include "AdjacencyList.hpp"
#include "UniversalTable.hpp"



class AdjacencyListUniversalTableManager {
private:
    AdjacencyList* adjacencyList = new AdjacencyList();
    UniversalTable* universalTable = new UniversalTable();
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::string verticesDirectory);
    bool loadEdges(std::string edgesDirectory);
    std::vector<int64_t> addVertexProperties(std::string vertexHeaderLine);
    std::vector<int64_t> addEdgeProperties(std::string edgeHeaderLine);
    
};

#endif /* ADJACENCYLISTUNIVERSALTABLEMANAGER_HPP */

