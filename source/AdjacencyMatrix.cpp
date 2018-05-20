/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"AdjacencyMatrix.hpp"
#include<iostream>

AdjacencyMatrix::AdjacencyMatrix() {

}

uint64_t AdjacencyMatrix::insertVertex() {
    uint64_t nodesCount = (veretexAdjacencyMap.empty()) ? 1 : veretexAdjacencyMap.begin()->second.size() + 1;
    for (auto& nodeNeighbours : veretexAdjacencyMap) {
        nodeNeighbours.second.push_back(false);
    }
    std::vector<bool> neighbours(nodesCount, false);
    veretexAdjacencyMap[nextVertexId] = neighbours;
    nextVertexId++;

    return nextVertexId - 1;
}

bool AdjacencyMatrix::removeVeretex(uint64_t vertexId) {
    if (veretexAdjacencyMap.find(vertexId) != veretexAdjacencyMap.end()) {
        uint64_t nodeIndex = getVertexIndex(vertexId);

        for (auto& nodeNeighbours : veretexAdjacencyMap) {
            nodeNeighbours.second.erase(nodeNeighbours.second.begin() + nodeIndex);
        }

        veretexAdjacencyMap.erase(vertexId);

        return true;
    } else {
        return false;
    }
}

bool AdjacencyMatrix::addNeighbourVertex(uint64_t vertexId, uint64_t neighbourVertexId) {
    if (veretexAdjacencyMap.find(vertexId) != veretexAdjacencyMap.end() &&
            veretexAdjacencyMap.find(neighbourVertexId) != veretexAdjacencyMap.end()) {
        veretexAdjacencyMap.at(vertexId).at(getVertexIndex(neighbourVertexId)) = true;
        return true;
    } else {
        return false;
    }
}

bool AdjacencyMatrix::removeNeighbourVertex(uint64_t vertexId, uint64_t neighbourVertexId) {
    if (veretexAdjacencyMap.find(vertexId) != veretexAdjacencyMap.end() &&
            veretexAdjacencyMap.find(neighbourVertexId) != veretexAdjacencyMap.end()) {
        veretexAdjacencyMap.at(vertexId).at(getVertexIndex(neighbourVertexId)) = false;
        return true;
    } else {
        return false;
    }
}

std::list<uint64_t> AdjacencyMatrix::getNeighbourVertices(uint64_t vertexId) {
    std::list<uint64_t> neighbourList(0);
    if (veretexAdjacencyMap.find(vertexId) != veretexAdjacencyMap.end()) {
        for (std::vector<uint64_t>::size_type i = 0; i != veretexAdjacencyMap.at(vertexId).size(); i++) {
            if (veretexAdjacencyMap.at(vertexId)[i]) {
                std::map<uint64_t, std::vector<bool> >::iterator it = veretexAdjacencyMap.begin();
                std::advance(it, i);
                neighbourList.push_back(it->first);
            }
        }
    }

    return neighbourList;
}

uint64_t AdjacencyMatrix::getVertexIndex(uint64_t vertexId) {
    return std::distance(veretexAdjacencyMap.begin(), veretexAdjacencyMap.find(vertexId));
}