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
    std::map<uint64_t, std::vector<bool> > veretexAdjacencyMap;
    uint64_t nextVertexId = 0;

public:
    uint64_t insertVertex();
    bool removeVeretex(uint64_t vertexId);
    bool addNeighbourVertex(uint64_t vertexId, uint64_t neighbourVertexId);
    bool removeNeighbourVertex(uint64_t vertexId, uint64_t neighbourVertexId);
    std::list<uint64_t> getNeighbourVertices(uint64_t vertexId);
    uint64_t getVertexIndex(uint64_t vertexId);
    AdjacencyMatrix();
};


#endif /* ADJACENCYMATRIX_HPP */

