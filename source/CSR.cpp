/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"CSR.hpp"

CSR::CSR() {
    this->vertexRowIndex.emplace_back(0);
}

bool CSR::insertVertex(std::string vertexId) {

    uint64_t vertexCount = this->vertexRowIndex.size() - 1;

    bool inserted = this->vertexIndexMap.insert(std::make_pair(vertexId, vertexCount)).second;

    if (inserted) {
        this->vertexRowIndex.emplace_back(this->vertexRowIndex.at(vertexCount));
        return true;
    } else {
        return false;
    }
}

void CSR::insertVertex(std::set<std::string> &vertexIds) {

    uint64_t vertexCount = this->vertexRowIndex.size() - 1;

    this->vertexRowIndex.resize(vertexCount + vertexIds.size() + 1, this->vertexRowIndex.at(vertexCount));

    //loop over all the vertexIds to index them
    for (std::set<std::string>::iterator it = vertexIds.begin(); it != vertexIds.end(); ++it) {
        std::string vertexId = *it;
        this->vertexIndexMap.insert(std::make_pair(vertexId, vertexCount++));
    }
}

/*
bool CSR::removeVeretex(std::string vertexId) {
    //check for the existence of the vertexId to be removed
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {
        uint64_t nodeIndex = getVertexIndexByVertexId(vertexId);

        //erase the vertex neighborhood flag from all the other vertices stored
        for (auto& nodeNeighbours : vertexAdjacencyMap) {
            nodeNeighbours.second.erase(nodeNeighbours.second.begin() + nodeIndex);
        }

        //erase the vertex row from the vertexAdjacencyMap and vertexIndexMap
        vertexAdjacencyMap.erase(vertexId);
        vertexIndexMap.erase(vertexId);
        for (auto& vertexIdToIndex : vertexIndexMap) {
            if (vertexIdToIndex.second > nodeIndex) {
                vertexIdToIndex.second-=1;
            }
        }

        return true;
    } else {
        return false;
    }
}
 */
bool CSR::addNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
    uint64_t sourceVertexIndex, targetVertexIndex;

    try {
        sourceVertexIndex = this->getVertexIndexByVertexId(vertexId);
    } catch (std::exception e) {
        return false;
        //this->insertVertex(vertexId);
        //sourceVertexIndex = this->getVertexIndexByVertexId(vertexId);
    }
    try {
        targetVertexIndex = this->getVertexIndexByVertexId(neighbourVertexId);
    } catch (std::exception e) {
        return false;
        //this->insertVertex(neighbourVertexId);
        //targetVertexIndex = this->getVertexIndexByVertexId(neighbourVertexId);
    }

    uint64_t idx;
    for (idx = this->vertexRowIndex.at(sourceVertexIndex);
            idx < this->vertexRowIndex.at(sourceVertexIndex + 1) && this->vertexColumnIndex.at(idx) < targetVertexIndex;
            idx++) {
    }

    if (idx == this->vertexRowIndex.at(sourceVertexIndex + 1) ||
            this->vertexColumnIndex.at(idx) > targetVertexIndex) {
        this->vertexColumnIndex.emplace(this->vertexColumnIndex.begin() + idx, targetVertexIndex);

        for (uint64_t i = sourceVertexIndex + 1; i<this->vertexRowIndex.size(); i++) {
            this->vertexRowIndex.at(i) += 1;
        }
        return true;
    } else {
        return false;
    }
}

void CSR::addNeighbourVertex(std::vector<std::pair<std::string, std::string> > &edges) {
    //std::vector<bool> result;
    //loop over all VertexNeighbourIds pairs and add them one by one
    for (std::vector<std::pair<std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::pair<std::string, std::string> neighbourPairIds = *it;
        this->addNeighbourVertex(neighbourPairIds.first, neighbourPairIds.second);
    }
    //return result;
}

/*
bool CSR::removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
    //check for the existence of the vertexId & neighbourVertexId
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {
        //set the neighborhood flag between the two vertices to false
        vertexAdjacencyMap.at(vertexId).at(getVertexIndexByVertexId(neighbourVertexId)) = false;
        return true;
    } else {
        return false;
    }
}

std::list<std::string> CSR::getNeighbourVertices(std::string vertexId) {
    std::list<std::string> neighbourList(0);
    //check for the existence of the vertexId
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {
        //loop over the vertex neighborhood flags to get the vertex neighbors
        for (uint64_t i = 0; i != vertexAdjacencyMap.at(vertexId).size(); i++) {
            if (vertexAdjacencyMap.at(vertexId)[i]) {
                std::string neighborVertexId = this->getVertexIdByVertexIndex(i);
                if (!neighborVertexId.empty()) {
                    neighbourList.push_back(neighborVertexId);
                }
            }
        }
    }

    return neighbourList;
}
 */
uint64_t CSR::getVertexIndexByVertexId(std::string vertexId) {
    return this->vertexIndexMap.at(vertexId);
}

std::string CSR::getVertexIdByVertexIndex(uint64_t vertexIndex) {
    for (auto& vertexIdToIndex : vertexIndexMap) {
        if (vertexIdToIndex.second == vertexIndex) {
            return vertexIdToIndex.first;
        }
    }
    return "";
}

uint64_t CSR::getCSRSize() {
    return this->vertexIndexMap.size();
}
