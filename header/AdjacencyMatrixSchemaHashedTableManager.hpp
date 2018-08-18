/* 
 * File:   AdjacencyListSchemaHashedTableManager.hpp
 * Author: same7
 *
 * Created on May 23, 2018, 01:01 AM
 */

#ifndef ADJACENCYMATRIXSCHEMAHASHEDTABLEMANAGER_HPP
#define ADJACENCYMATRIXSCHEMAHASHEDTABLEMANAGER_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <dirent.h>

#include "AdjacencyMatrix.hpp"
#include "SchemaHashedTable.hpp"

class AdjacencyMatrixSchemaHashedTableManager {
private:
    AdjacencyMatrix adjacencyMatrix;
    SchemaHashedTable schemaHashedTable;
    int batchSize;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::string verticesDirectory);
    bool loadEdges(std::string edgesDirectory);
    std::map<int, std::string> getVertexProperties(std::string vertexHeaderLine);
    std::map<int, std::string> getEdgeProperties(std::string edgeHeaderLine);
    
    uint64_t getAdjacencyMatrixSizeInBytes(){return this->adjacencyMatrix.getAdjacencyMatrixSizeInBytes();};
    uint64_t getSchemaHashedTableSizeInBytes(){return this->schemaHashedTable.getSchemaHashedTableSizeInBytes();};
    
    AdjacencyMatrixSchemaHashedTableManager(int bSize):batchSize(bSize){
        adjacencyMatrix.setBatchSize(bSize);
    };
};

#endif /* ADJACENCYMATRIXSCHEMAHASHEDTABLEMANAGER_HPP */

