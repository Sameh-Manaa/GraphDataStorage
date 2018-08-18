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
#include "UtilityFunctions.hpp"

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
    
    void executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void executeQueryDC(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    
    uint64_t getAdjacencyMatrixSizeInBytes(){return this->adjacencyMatrix.getAdjacencyMatrixSizeInBytes();};
    uint64_t getUniversalTableSizeInBytes(){return this->universalTable.getUniversalTableSizeInBytes();};
    
    AdjacencyMatrixUniversalTableManager(int bSize):batchSize(bSize){
        adjacencyMatrix.setBatchSize(bSize);
    };
    
};

#endif /* ADJACENCYMATRIXUNIVERSALTABLEMANAGER_HPP */

