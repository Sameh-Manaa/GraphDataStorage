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
#include <sstream>
#include <string>
#include <filesystem>

#include "AdjacencyList.hpp"
#include "SchemaHashedTable.hpp"



class AdjacencyListSchemaHashedTableManager {
private:
    AdjacencyList adjacencyList;
    SchemaHashedTable schemaHashedTable;
    int batchSize = 500;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::string verticesDirectory);
    bool loadEdges(std::string edgesDirectory);
    std::map<int, std::string> getVertexProperties(std::string vertexHeaderLine);
    std::map<int, std::string> getEdgeProperties(std::string edgeHeaderLine);
};

#endif /* ADJACENCYLISTSCHEMAHASHEDTABLEMANAGER_HPP */

