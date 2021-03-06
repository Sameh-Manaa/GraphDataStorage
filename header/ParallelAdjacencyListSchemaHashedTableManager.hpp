/* 
 * File:   AdjacencyListSchemaHashedTableManager.hpp
 * Author: same7
 *
 * Created on Jul 26, 2018, 12:41 PM
 */

#ifndef PARALLELADJACENCYLISTSCHEMAHASHEDTABLEMANAGER_HPP
#define PARALLELADJACENCYLISTSCHEMAHASHEDTABLEMANAGER_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <dirent.h>
#include <thread>

#include "ParallelAdjacencyList.hpp"
#include "ParallelSchemaHashedTable.hpp"

class ParallelAdjacencyListSchemaHashedTableManager {
private:
    ParallelAdjacencyList parallelAdjacencyList;
    ParallelSchemaHashedTable parallelSchemaHashedTable;
    int batchSize;
    int parallelismDegree = 1;
    std::mutex fileReadMutex;
    bool topologyLoad, propertiesLoad;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::string &vertexFileName, std::map<int, std::string> &propertiesPositions, std::string &vertexType, int threadId);
    bool loadEdges(std::string &edgeFilePath, std::map<int, std::string> &propertiesPositions, std::string &edgeFileName, int threadId);
    std::map<int, std::string> getVertexProperties(std::string vertexHeaderLine);
    std::map<int, std::string> getEdgeProperties(std::string edgeHeaderLine);

    ParallelAdjacencyListSchemaHashedTableManager(int bSize, int parallilism, bool topology = true, bool properties = true) : batchSize(bSize), parallelismDegree(parallilism), topologyLoad(topology), propertiesLoad(properties) {
    };
};

#endif /* PARALLELADJACENCYLISTSCHEMAHASHEDTABLEMANAGER_HPP */

