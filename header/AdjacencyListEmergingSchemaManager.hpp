/* 
 * File:   AdjacencyListEmergingSchemaManager.hpp
 * Author: same7
 *
 * Created on Jul 18, 2018, 11:32 AM
 */

#ifndef ADJACENCYLISTEMERGINGSCHEMAMANAGER_HPP
#define ADJACENCYLISTEMERGINGSCHEMAMANAGER_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <dirent.h>
#include "UtilityFunctions.hpp"

#include "AdjacencyList.hpp"
#include "EmergingSchema.hpp"
#include "UniversalTable.hpp"



class AdjacencyListEmergingSchemaManager {
private:
    AdjacencyList adjacencyList;
    UniversalTable universalTable;
    EmergingSchema emergingSchema;
    int batchSize;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory, uint8_t filesToLoad = 0);
    bool loadVertices(std::string verticesDirectory, uint8_t filesToLoad);
    bool loadEdges(std::string edgesDirectory, uint8_t filesToLoad);
    std::vector<int16_t> addVertexProperties(std::string vertexHeaderLine);
    std::vector<int16_t> addEdgeProperties(std::string edgeHeaderLine);
    
    void executeQueryBI1(tm messageCreationDate, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    
    uint64_t getAdjacencyListSizeInBytes(){return this->adjacencyList.getAdjacencyListSizeInBytes();};
    uint64_t getEmergingSchemaSizeInBytes(){return this->emergingSchema.getEmergingSchemaSizeInBytes();};
    
    AdjacencyListEmergingSchemaManager(int bSize):batchSize(bSize){};
};

#endif /* ADJACENCYLISTEMERGINGSCHEMAMANAGER_HPP */

