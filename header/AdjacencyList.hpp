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
    std::map<uint64_t, std::map<std::string, std::vector<uint64_t> > > nodeAdjacencyMap;
    uint64_t nextNodeId = 0;

public:
    uint64_t insertNode();
    bool removeNode(uint64_t nodeId);
    bool addNeighbour(uint64_t nodeId, uint64_t neighbourId, std::string edgeLabel);
    bool removeNeighbour(uint64_t nodeId, uint64_t neighbourId);
    bool removeNeighbour(uint64_t nodeId, uint64_t neighbourId, std::string edgeLabel);
    std::map<std::string, std::vector<uint64_t> > getNeighbours(uint64_t nodeId);
    uint64_t getNodeIndex(uint64_t nodeId);
    AdjacencyList();
};


#endif /* ADJACENCYLIST_HPP */

