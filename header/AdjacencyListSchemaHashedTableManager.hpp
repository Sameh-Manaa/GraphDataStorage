/* 
 * File:   AdjacencyListSchemaHashedTableManager.hpp
 * Author: same7
 *
 * Created on May 23, 2018, 01:01 AM
 */

#ifndef ADJACENCYLISTSCHEMAHASHEDTABLEMANAGER_HPP
#define ADJACENCYLISTSCHEMAHASHEDTABLEMANAGER_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <dirent.h>
#include "UtilityFunctions.hpp"

#include "AdjacencyList.hpp"
#include "SchemaHashedTable.hpp"

class AdjacencyListSchemaHashedTableManager {
private:
    AdjacencyList adjacencyList;
    SchemaHashedTable schemaHashedTable;
    int batchSize;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory, uint8_t filesToLoad = 0);
    bool loadVertices(std::string verticesDirectory, uint8_t filesToLoad);
    bool loadEdges(std::string edgesDirectory, uint8_t filesToLoad);
    std::map<int, std::string> getVertexProperties(std::string vertexHeaderLine);
    std::map<int, std::string> getEdgeProperties(std::string edgeHeaderLine);
    
    void executeQueryBI1(tm messageCreationDate, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    
    uint64_t getAdjacencyListSizeInBytes(){return this->adjacencyList.getAdjacencyListSizeInBytes();};
    uint64_t getSchemaHashedTableSizeInBytes(){return this->schemaHashedTable.getSchemaHashedTableSizeInBytes();};
    
    AdjacencyListSchemaHashedTableManager(int bSize):batchSize(bSize){};
};

#endif /* ADJACENCYLISTSCHEMAHASHEDTABLEMANAGER_HPP */

