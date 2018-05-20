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

class AdjacencyList {
private:
    std::map<uint64_t, std::map<std::string, std::vector<uint64_t> > > vertexAdjacencyMap;
    uint64_t nextVertexId = 0;

public:
    uint64_t insertVertex();
    bool removeVertex(uint64_t vertexId);
    bool addNeighbourVertex(uint64_t vertexId, uint64_t neighbourVertexId, std::string edgeLabel);
    bool removeNeighbourVertex(uint64_t vertexId, uint64_t neighbourVertexId);
    bool removeNeighbourVertex(uint64_t vertexId, uint64_t neighbourVertexId, std::string edgeLabel);
    std::map<std::string, std::vector<uint64_t> > getNeighbourVertices(uint64_t vertexId);
    uint64_t getVertexIndex(uint64_t vertexId);
    AdjacencyList();
};


#endif /* ADJACENCYLIST_HPP */

