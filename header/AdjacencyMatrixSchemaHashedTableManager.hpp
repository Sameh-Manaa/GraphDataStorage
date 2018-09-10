/* 
 * File:   AdjacencyListSchemaHashedTableManager.hpp
 * Author: same7
 *
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
#include <algorithm>
#include "UtilityFunctions.hpp"

#include "AdjacencyMatrix.hpp"
#include "SchemaHashedTable.hpp"

class AdjacencyMatrixSchemaHashedTableManager {
private:
    AdjacencyMatrix adjacencyMatrix;
    SchemaHashedTable schemaHashedTable;
    int batchSize;
    bool topologyLoad, propertiesLoad;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory, uint8_t filesToLoad = 0);
    bool loadVertices(std::string verticesDirectory, uint8_t filesToLoad);
    bool loadEdges(std::string edgesDirectory, uint8_t filesToLoad);
    std::map<int, std::string> getVertexProperties(std::string vertexHeaderLine);
    std::map<int, std::string> getEdgeProperties(std::string edgeHeaderLine);

    void executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);

    uint64_t getAdjacencyMatrixSizeInBytes() {
        return this->adjacencyMatrix.getAdjacencyMatrixSizeInBytes();
    };

    uint64_t getSchemaHashedTableSizeInBytes() {
        return this->schemaHashedTable.getSchemaHashedTableSizeInBytes();
    };

    AdjacencyMatrixSchemaHashedTableManager(int bSize, bool topology = true, bool properties = true) : batchSize(bSize), topologyLoad(topology), propertiesLoad(properties) {
        adjacencyMatrix.setBatchSize(bSize);
    };
};

#endif /* ADJACENCYMATRIXSCHEMAHASHEDTABLEMANAGER_HPP */

