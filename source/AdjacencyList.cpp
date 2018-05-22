/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"AdjacencyList.hpp"

AdjacencyList::AdjacencyList() {

}

bool AdjacencyList::insertVertex(std::string vertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {
        return false;
    }
    std::unordered_map<std::string, std::vector<std::string> > labeledNeighbourVertexMap;
    std::vector<std::string> neighbourVertexIds(0);
    labeledNeighbourVertexMap[""] = neighbourVertexIds;
    vertexAdjacencyMap[vertexId] = labeledNeighbourVertexMap;

    return true;
}

bool AdjacencyList::insertVertex(std::set<std::string> vertexIds) {
    for (std::set<std::string>::iterator it = vertexIds.begin(); it != vertexIds.end(); ++it) {
        std::string vertexId = *it;
        this->insertVertex(vertexId);
    }

    return true;
}

bool AdjacencyList::removeVertex(std::string vertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {

        for (auto& labeledNeighbourVertexMap : vertexAdjacencyMap) {
            for (auto& vertexLabeledNeighbourVertexs : labeledNeighbourVertexMap.second) {
                std::vector<std::string>::iterator it = std::find(vertexLabeledNeighbourVertexs.second.begin(), vertexLabeledNeighbourVertexs.second.end(), vertexId);
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

bool AdjacencyList::addNeighbourVertex(std::string vertexId, std::string neighbourVertexId, std::string edgeLabel) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {
        vertexAdjacencyMap.at(vertexId)[edgeLabel].push_back(neighbourVertexId);
        return true;
    } else {
        return false;
    }
}

bool AdjacencyList::removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {
        for (auto& vertexLabeledNeighbourVertexs : vertexAdjacencyMap.at(vertexId)) {
            std::vector<std::string>::iterator it = std::find(vertexLabeledNeighbourVertexs.second.begin(), vertexLabeledNeighbourVertexs.second.end(), neighbourVertexId);
            if (it != vertexLabeledNeighbourVertexs.second.end()) {
                vertexLabeledNeighbourVertexs.second.erase(it);
            }
        }
        return true;
    } else {
        return false;
    }
}

bool AdjacencyList::removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId, std::string edgeLabel) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {
        std::vector<std::string>::iterator it = std::find(vertexAdjacencyMap.at(vertexId).at(edgeLabel).begin(), vertexAdjacencyMap.at(vertexId).at(edgeLabel).end(), neighbourVertexId);
        if (it != vertexAdjacencyMap.at(vertexId).at(edgeLabel).end()) {
            vertexAdjacencyMap.at(vertexId).at(edgeLabel).erase(it);
        }
        return true;
    } else {
        return false;
    }
}

std::unordered_map<std::string, std::vector<std::string> > AdjacencyList::getNeighbourVertices(std::string vertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {
        return vertexAdjacencyMap.at(vertexId);
    } else {
        return {};
    }
}

uint64_t AdjacencyList::getVertexIndex(std::string vertexId) {
    return std::distance(vertexAdjacencyMap.begin(), vertexAdjacencyMap.find(vertexId));
}

uint64_t AdjacencyList::getAdjacencyListSize() {
    return this->vertexAdjacencyMap.size();
}