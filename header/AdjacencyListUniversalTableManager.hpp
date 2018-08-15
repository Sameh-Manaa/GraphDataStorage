/* 
 * File:   AdjacencyListUniversalTableManager.hpp
 * Author: same7
 *
 * Created on May 23, 2018, 01:01 AM
 */

#ifndef ADJACENCYLISTUNIVERSALTABLEMANAGER_HPP
#define ADJACENCYLISTUNIVERSALTABLEMANAGER_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <dirent.h>
#include <stdio.h>
#include <chrono>
#include "UtilityFunctions.hpp"

#include "AdjacencyList.hpp"
#include "UniversalTable.hpp"

class AdjacencyListUniversalTableManager {
private:
    AdjacencyList adjacencyList;
    UniversalTable universalTable;
    int batchSize;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::string verticesDirectory);
    bool loadEdges(std::string edgesDirectory);
    std::vector<int16_t> addVertexProperties(std::string vertexHeaderLine);
    std::vector<int16_t> addEdgeProperties(std::string edgeHeaderLine);

    void executeQueryBI1(tm messageCreationDate, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void executeQueryDC(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);

    AdjacencyListUniversalTableManager(int bSize) : batchSize(bSize) {
    };

};

#endif /* ADJACENCYLISTUNIVERSALTABLEMANAGER_HPP */

