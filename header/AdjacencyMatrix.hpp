/* 
 * File:   AdjacencyMatrix.hpp
 * Author: Sameh-Manaa
 *
 * Created on May 9, 2018, 12:45 PM
 */

#ifndef ADJACENCYMATRIX_HPP
#define ADJACENCYMATRIX_HPP

#include <vector>
#include <list>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>
#include <boost/dynamic_bitset.hpp>

class AdjacencyMatrix {
private:
    std::unordered_map<std::string, std::vector<bool> > vertexAdjacencyMap;

public:
    bool insertVertex(std::string vertexId);
    bool insertVertex(std::set<std::string> vertexIds);
    bool removeVeretex(std::string vertexId);
    bool addNeighbourVertex(std::string vertexId, std::string neighbourVertexId);
    bool addNeighbourVertex(std::set<std::pair<std::string,std::string> > neighbourIds);
    bool removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId);
    std::list<std::string> getNeighbourVertices(std::string vertexId);
    uint64_t getVertexIndex(std::string vertexId);
    uint64_t getAdjacencyMatrixSize();
    AdjacencyMatrix();
};


#endif /* ADJACENCYMATRIX_HPP */

