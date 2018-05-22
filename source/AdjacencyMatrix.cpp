/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"AdjacencyMatrix.hpp"

AdjacencyMatrix::AdjacencyMatrix() {
}

bool AdjacencyMatrix::insertVertex(std::string vertexId) {
    uint64_t nodesCount = (vertexAdjacencyMap.empty()) ? 1 : vertexAdjacencyMap.begin()->second.size() + 1;

    std::vector<bool> neighbours(nodesCount - 1, false);
    std::pair < std::unordered_map < std::string, std::vector<bool> >::iterator, bool> insertResult = vertexAdjacencyMap.insert(std::make_pair(vertexId, neighbours));

    if (insertResult.second) {
        for (auto& nodeNeighbours : vertexAdjacencyMap) {
            nodeNeighbours.second.push_back(false);
        }
        return true;
    } else {
        return false;
    }

}

bool AdjacencyMatrix::insertVertex(std::set<std::string> vertexIds) {
    uint64_t nodesCount = (vertexAdjacencyMap.empty()) ? vertexIds.size() : vertexAdjacencyMap.begin()->second.size() + vertexIds.size();
    for (auto& nodeNeighbours : vertexAdjacencyMap) {
        nodeNeighbours.second.resize(nodesCount, false);
    }
    std::vector<bool> neighbours(nodesCount, false);
    //std::vector<bool> neighbours(0);

    for (std::set<std::string>::iterator it = vertexIds.begin(); it != vertexIds.end(); ++it) {
        std::string vertexId = *it;
        vertexAdjacencyMap[vertexId] = neighbours;
    }
    //nextVertexId++;

    return true;
}

bool AdjacencyMatrix::removeVeretex(std::string vertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {
        uint64_t nodeIndex = getVertexIndex(vertexId);

        for (auto& nodeNeighbours : vertexAdjacencyMap) {
            nodeNeighbours.second.erase(nodeNeighbours.second.begin() + nodeIndex);
        }

        vertexAdjacencyMap.erase(vertexId);

        return true;
    } else {
        return false;
    }
}

bool AdjacencyMatrix::addNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {
        vertexAdjacencyMap.at(vertexId).at(getVertexIndex(neighbourVertexId)) = true;
        return true;
    } else {
        return false;
    }
}

bool AdjacencyMatrix::addNeighbourVertex(std::set<std::pair<std::string, std::string> > neighbourIds) {
    for (std::set<std::pair<std::string, std::string> >::iterator it = neighbourIds.begin(); it != neighbourIds.end(); ++it) {
        std::pair<std::string, std::string> neighbourPairIds = *it;
        this->addNeighbourVertex(neighbourPairIds.first,neighbourPairIds.second);
    }
    return true;
}

bool AdjacencyMatrix::removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {
        vertexAdjacencyMap.at(vertexId).at(getVertexIndex(neighbourVertexId)) = false;
        return true;
    } else {
        return false;
    }
}

std::list<std::string> AdjacencyMatrix::getNeighbourVertices(std::string vertexId) {
    std::list<std::string> neighbourList(0);
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {
        for (std::vector<uint64_t>::size_type i = 0; i != vertexAdjacencyMap.at(vertexId).size(); i++) {
            if (vertexAdjacencyMap.at(vertexId)[i]) {
                std::unordered_map < std::string, std::vector<bool> >::iterator it = vertexAdjacencyMap.begin();
                std::advance(it, i);
                neighbourList.push_back(it->first);
            }
        }
    }

    return neighbourList;
}

uint64_t AdjacencyMatrix::getVertexIndex(std::string vertexId) {
    return std::distance(vertexAdjacencyMap.begin(), vertexAdjacencyMap.find(vertexId));
}

uint64_t AdjacencyMatrix::getAdjacencyMatrixSize() {
    return this->vertexAdjacencyMap.size();
}