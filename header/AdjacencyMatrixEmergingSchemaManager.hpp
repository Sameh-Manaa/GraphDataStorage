/* 
 * File:   AdjacencyMatrixEmergingSchemaManager.hpp
 * Author: same7
 *
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
#include "UtilityFunctions.hpp"

#include "AdjacencyMatrix.hpp"
#include "UniversalTable.hpp"
#include "EmergingSchema.hpp"

class AdjacencyMatrixEmergingSchemaManager {
private:
    AdjacencyMatrix adjacencyMatrix;
    UniversalTable universalTable;
    EmergingSchema emergingSchema;
    int batchSize;
    bool topologyLoad, propertiesLoad;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory, uint8_t filesToLoad = 0);
    bool loadVertices(std::string verticesDirectory, uint8_t filesToLoad);
    bool loadEdges(std::string edgesDirectory, uint8_t filesToLoad);
    std::vector<int16_t> addVertexProperties(std::string vertexHeaderLine);
    std::vector<int16_t> addEdgeProperties(std::string edgeHeaderLine);

    void executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);

    uint64_t getAdjacencyMatrixSizeInBytes() {
        return this->adjacencyMatrix.getAdjacencyMatrixSizeInBytes();
    };

    uint64_t getEmergingSchemaSizeInBytes() {
        return this->emergingSchema.getEmergingSchemaSizeInBytes();
    };

    AdjacencyMatrixEmergingSchemaManager(int bSize, bool topology = true, bool properties = true) : batchSize(bSize), topologyLoad(topology), propertiesLoad(properties) {
        adjacencyMatrix.setBatchSize(bSize);
    };

};

#endif /* ADJACENCYMATRIXEMERGINGSCHEMAMANAGER_HPP */

