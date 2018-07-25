/* 
 * File:   AdjacencyMatrixEmergingSchemaManager.hpp
 * Author: same7
 *
 * Created on Jul 23, 2018, 03:24 PM
 */

#ifndef ADJACENCYMATRIXEMERGINGSCHEMAMANAGER_HPP
#define ADJACENCYMATRIXEMERGINGSCHEMAMANAGER_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <dirent.h>

#include "AdjacencyMatrix.hpp"
#include "EmergingSchema.hpp"



class AdjacencyMatrixEmergingSchemaManager {
private:
    AdjacencyMatrix adjacencyMatrix;
    EmergingSchema emergingSchema;
    int batchSize;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::string verticesDirectory);
    bool loadEdges(std::string edgesDirectory);
    std::vector<int64_t> addVertexProperties(std::string vertexHeaderLine);
    std::vector<int64_t> addEdgeProperties(std::string edgeHeaderLine);
    
    AdjacencyMatrixEmergingSchemaManager(int bSize):batchSize(bSize){};
    
};

#endif /* ADJACENCYMATRIXEMERGINGSCHEMAMANAGER_HPP */
