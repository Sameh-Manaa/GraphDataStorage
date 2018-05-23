/* 
 * File:   AdjacencyList.hpp
 * Author: Sameh-Manaa
 *
 * Created on May 15, 2018, 09:18 PM
 */

#ifndef ADJACENCYLIST_HPP
#define ADJACENCYLIST_HPP

#include <vector>
#include <list>
#include <stdint.h>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>

class AdjacencyList {
private:
    std::map<std::string, std::map<std::string, std::vector<std::string> > > vertexAdjacencyMap;

public:
    bool insertVertex(std::string vertexId);
    bool insertVertex(std::set<std::string> vertexIds);
    bool removeVertex(std::string vertexId);
    bool addNeighbourVertex(std::string vertexId, std::string neighbourVertexId, std::string edgeLabel);
    bool removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId);
    bool removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId, std::string edgeLabel);
    std::map<std::string, std::vector<std::string> > getNeighbourVertices(std::string vertexId);
    uint64_t getVertexIndex(std::string vertexId);
    uint64_t getAdjacencyListSize();
    AdjacencyList();
};


#endif /* ADJACENCYLIST_HPP */

