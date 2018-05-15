/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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
    std::map<uint64_t, std::vector<bool> > nodeAdjacencyMap;
    uint64_t nextNodeId = 0;

public:
    uint64_t insertNode();
    bool removeNode(uint64_t nodeId);
    bool addNeighbour(uint64_t nodeId, uint64_t neighbourId);
    bool removeNeighbour(uint64_t nodeId, uint64_t neighbourId);
    std::list<uint64_t>* getNeighbours(uint64_t nodeId);
    uint64_t getNodeIndex(uint64_t nodeId);
    AdjacencyMatrix();
};


#endif /* ADJACENCYMATRIX_HPP */

