/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"AdjacencyMatrix.hpp"
#include<iostream>

AdjacencyMatrix::AdjacencyMatrix() {

}

bool AdjacencyMatrix::insertVertex(std::string vertexId) {
    if(veretexAdjacencyMap.find(vertexId) != veretexAdjacencyMap.end()){
        return false;
    }
    uint64_t nodesCount = (veretexAdjacencyMap.empty()) ? 1 : veretexAdjacencyMap.begin()->second.size() + 1;
    for (auto& nodeNeighbours : veretexAdjacencyMap) {
        nodeNeighbours.second.push_back(false);
    }
    std::vector<bool> neighbours(nodesCount, false);
    veretexAdjacencyMap[vertexId] = neighbours;
    //nextVertexId++;

    return true;
}

bool AdjacencyMatrix::removeVeretex(std::string vertexId) {
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

bool AdjacencyMatrix::addNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
    if (veretexAdjacencyMap.find(vertexId) != veretexAdjacencyMap.end() &&
            veretexAdjacencyMap.find(neighbourVertexId) != veretexAdjacencyMap.end()) {
        veretexAdjacencyMap.at(vertexId).at(getVertexIndex(neighbourVertexId)) = true;
        return true;
    } else {
        return false;
    }
}

bool AdjacencyMatrix::removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
    if (veretexAdjacencyMap.find(vertexId) != veretexAdjacencyMap.end() &&
            veretexAdjacencyMap.find(neighbourVertexId) != veretexAdjacencyMap.end()) {
        veretexAdjacencyMap.at(vertexId).at(getVertexIndex(neighbourVertexId)) = false;
        return true;
    } else {
        return false;
    }
}

std::list<std::string> AdjacencyMatrix::getNeighbourVertices(std::string vertexId) {
    std::list<std::string> neighbourList(0);
    if (veretexAdjacencyMap.find(vertexId) != veretexAdjacencyMap.end()) {
        for (std::vector<uint64_t>::size_type i = 0; i != veretexAdjacencyMap.at(vertexId).size(); i++) {
            if (veretexAdjacencyMap.at(vertexId)[i]) {
                std::map<std::string, std::vector<bool> >::iterator it = veretexAdjacencyMap.begin();
                std::advance(it, i);
                neighbourList.push_back(it->first);
            }
        }
    }

    return neighbourList;
}

uint64_t AdjacencyMatrix::getVertexIndex(std::string vertexId) {
    return std::distance(veretexAdjacencyMap.begin(), veretexAdjacencyMap.find(vertexId));
}