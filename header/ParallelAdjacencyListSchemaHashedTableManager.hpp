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
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::ifstream &vertexFile, std::map<int, std::string> &propertiesPositions, std::string &vertexType);
    bool loadEdges(std::ifstream &edgeFile, std::map<int, std::string> &propertiesPositions, std::string &edgeFileName);
    std::map<int, std::string> getVertexProperties(std::string vertexHeaderLine);
    std::map<int, std::string> getEdgeProperties(std::string edgeHeaderLine);

    ParallelAdjacencyListSchemaHashedTableManager(int bSize, int parallilism) : batchSize(bSize), parallelismDegree(parallilism) {
    };
};

#endif /* PARALLELADJACENCYLISTSCHEMAHASHEDTABLEMANAGER_HPP */

