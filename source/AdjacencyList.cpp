/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"AdjacencyList.hpp"
#include<iostream>
#include <algorithm>

AdjacencyList::AdjacencyList() {

}

uint64_t AdjacencyList::insertVertex() {
    std::map<std::string, std::vector<uint64_t> > labeledNeighbourVertexMap;
    std::vector<uint64_t> neighbourVertexIds(0);
    labeledNeighbourVertexMap[""] = neighbourVertexIds;
    vertexAdjacencyMap[nextVertexId] = labeledNeighbourVertexMap;
    nextVertexId++;

    return nextVertexId - 1;
}


bool AdjacencyList::removeVertex(uint64_t vertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {

        for (auto& labeledNeighbourVertexMap : vertexAdjacencyMap) {
            for (auto& vertexLabeledNeighbourVertexs : labeledNeighbourVertexMap.second) {
                std::vector<uint64_t>::iterator it = std::find(vertexLabeledNeighbourVertexs.second.begin(), vertexLabeledNeighbourVertexs.second.end(), vertexId);
                if (it != vertexLabeledNeighbourVertexs.second.end()) {
                    vertexLabeledNeighbourVertexs.second.erase(it);
                }
            }
        }

        vertexAdjacencyMap.erase(vertexId);

        return true;
    } else {
        return false;
    }
}


bool AdjacencyList::addNeighbourVertex(uint64_t vertexId, uint64_t neighbourVertexId, std::string edgeLabel) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {
        vertexAdjacencyMap.at(vertexId)[edgeLabel].push_back(neighbourVertexId);
        return true;
    } else {
        return false;
    }
}


bool AdjacencyList::removeNeighbourVertex(uint64_t vertexId, uint64_t neighbourVertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {
        for (auto& vertexLabeledNeighbourVertexs : vertexAdjacencyMap.at(vertexId)) {
            std::vector<uint64_t>::iterator it = std::find(vertexLabeledNeighbourVertexs.second.begin(), vertexLabeledNeighbourVertexs.second.end(), neighbourVertexId);
            if (it != vertexLabeledNeighbourVertexs.second.end()) {
                vertexLabeledNeighbourVertexs.second.erase(it);
            }
        }
        return true;
    } else {
        return false;
    }
}


bool AdjacencyList::removeNeighbourVertex(uint64_t vertexId, uint64_t neighbourVertexId, std::string edgeLabel) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {
        std::vector<uint64_t>::iterator it = std::find(vertexAdjacencyMap.at(vertexId).at(edgeLabel).begin(), vertexAdjacencyMap.at(vertexId).at(edgeLabel).end(), neighbourVertexId);
        if (it != vertexAdjacencyMap.at(vertexId).at(edgeLabel).end()) {
            vertexAdjacencyMap.at(vertexId).at(edgeLabel).erase(it);
        }
        return true;
    } else {
        return false;
    }
}


std::map<std::string, std::vector<uint64_t> > AdjacencyList::getNeighbourVertices(uint64_t vertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {
        return vertexAdjacencyMap.at(vertexId);
    } else {
        return {};
    }
}


uint64_t AdjacencyList::getVertexIndex(uint64_t vertexId) {
    return std::distance(vertexAdjacencyMap.begin(), vertexAdjacencyMap.find(vertexId));
}
