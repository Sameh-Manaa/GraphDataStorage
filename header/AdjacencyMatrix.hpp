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

class AdjacencyMatrix {
private:
    std::map<std::string, std::vector<bool> > veretexAdjacencyMap;

public:
    bool insertVertex(std::string vertexId);
    bool removeVeretex(std::string vertexId);
    bool addNeighbourVertex(std::string vertexId, std::string neighbourVertexId);
    bool removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId);
    std::list<std::string> getNeighbourVertices(std::string vertexId);
    uint64_t getVertexIndex(std::string vertexId);
    AdjacencyMatrix();
};


#endif /* ADJACENCYMATRIX_HPP */

