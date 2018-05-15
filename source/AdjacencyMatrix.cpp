/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"AdjacencyMatrix.hpp"
#include<iostream>

AdjacencyMatrix::AdjacencyMatrix() {

}

uint64_t AdjacencyMatrix::insertNode() {
    uint64_t nodesCount = (nodeAdjacencyMap.empty()) ? 1 : nodeAdjacencyMap.begin()->second.size() + 1;
    for (auto& nodeNeighbours : nodeAdjacencyMap) {
        nodeNeighbours.second.push_back(false);
    }
    std::vector<bool> neighbours(nodesCount, false);
    nodeAdjacencyMap[nextNodeId] = neighbours;
    nextNodeId++;

    return nextNodeId - 1;
}

bool AdjacencyMatrix::removeNode(uint64_t nodeId) {
    if (nodeAdjacencyMap.find(nodeId) != nodeAdjacencyMap.end()) {
        uint64_t nodeIndex = getNodeIndex(nodeId);

        for (auto& nodeNeighbours : nodeAdjacencyMap) {
            nodeNeighbours.second.erase(nodeNeighbours.second.begin() + nodeIndex);
        }

        nodeAdjacencyMap.erase(nodeId);

        return true;
    } else {
        return false;
    }
}

bool AdjacencyMatrix::addNeighbour(uint64_t nodeId, uint64_t neighbourId) {
    if (nodeAdjacencyMap.find(nodeId) != nodeAdjacencyMap.end() &&
            nodeAdjacencyMap.find(neighbourId) != nodeAdjacencyMap.end()) {
        nodeAdjacencyMap.at(nodeId).at(getNodeIndex(neighbourId)) = true;
        return true;
    } else {
        return false;
    }
}

bool AdjacencyMatrix::removeNeighbour(uint64_t nodeId, uint64_t neighbourId) {
    if (nodeAdjacencyMap.find(nodeId) != nodeAdjacencyMap.end() &&
            nodeAdjacencyMap.find(neighbourId) != nodeAdjacencyMap.end()) {
        nodeAdjacencyMap.at(nodeId).at(getNodeIndex(neighbourId)) = false;
        return true;
    } else {
        return false;
    }
}

std::list<uint64_t>* AdjacencyMatrix::getNeighbours(uint64_t nodeId) {
    std::list<uint64_t>* neighbourList;
    if (nodeAdjacencyMap.find(nodeId) != nodeAdjacencyMap.end()) {
        for (std::vector<uint64_t>::size_type i = 0; i != nodeAdjacencyMap.at(nodeId).size(); i++) {
            if (nodeAdjacencyMap.at(nodeId)[i]) {
                std::map<uint64_t, std::vector<bool> >::iterator it = nodeAdjacencyMap.begin();
                std::advance(it, i);
                neighbourList->push_back(it->first);
            }
        }
        return neighbourList;
    } else {
        return nullptr;
    }
}

uint64_t AdjacencyMatrix::getNodeIndex(uint64_t nodeId) {
    return std::distance(nodeAdjacencyMap.begin(), nodeAdjacencyMap.find(nodeId));
}