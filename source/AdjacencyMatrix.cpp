/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"AdjacencyMatrix.hpp"

AdjacencyMatrix::AdjacencyMatrix() {
}

bool AdjacencyMatrix::insertVertex(std::string vertexId) {
    uint64_t nodesCount = (vertexAdjacencyMap.empty()) ? 0 : vertexAdjacencyMap.begin()->second.size();

    //construct a new vertex neighborhood row
    std::vector<bool> neighbours(nodesCount, false);
    /*
     * insert the vertex neighborhood row along with its vertexId into the vertexAdjacencyMap
     * (IF NOT ALREADY THERE)
     */
    std::pair < std::map < std::string, std::vector<bool> >::iterator, bool> insertResult =
            vertexAdjacencyMap.insert(std::make_pair(vertexId, neighbours));

    //Check if the insertion occurred or not
    if (insertResult.second) {
        /*
         * push a false neighbor flag into all the vertices neighborhood rows
         * indicating a false neighborhood with the new vertex
         */
        for (auto& nodeNeighbours : vertexAdjacencyMap) {
            nodeNeighbours.second.push_back(false);
        }
        //insert a pair of the new vertexId and its vertexIndex into the vertexIndexMap
        this->vertexIndexMap.insert(std::make_pair(vertexId, nodesCount));
        return true;
    } else {
        return false;
    }

}

void AdjacencyMatrix::insertVertex(std::set<std::string> &vertexIds) {

    //reserve space in the data structures for inserting the new vertexIds
    //this->vertexAdjacencyMap.reserve(this->vertexAdjacencyMap.size() + vertexIds.size());
    //this->vertexIndexMap.reserve(this->vertexIndexMap.size() + vertexIds.size());

    uint64_t nodesCount = (vertexAdjacencyMap.empty()) ? 0 : vertexAdjacencyMap.begin()->second.size();

    std::vector<bool> neighbours(nodesCount, false);

    u_int64_t countInserted = 0;
    //loop over all the vertexIds to insert them into the data structures
    for (std::set<std::string>::iterator it = vertexIds.begin(); it != vertexIds.end(); ++it) {
        std::string vertexId = *it;
        /*
         * insert the vertex neighborhood row along with its vertexId into the vertexAdjacencyMap
         * (IF NOT ALREADY THERE)
         */
        std::pair < std::map < std::string, std::vector<bool> >::iterator, bool> insertResult =
                vertexAdjacencyMap.insert(std::make_pair(vertexId, neighbours));
        //Check if the insertion occurred or not
        if (insertResult.second) {
            //insert a pair of the new vertexId and its vertexIndex into the vertexIndexMap
            this->vertexIndexMap.insert(std::make_pair(vertexId, nodesCount + countInserted));
            countInserted++;
        }
    }

    nodesCount = (vertexAdjacencyMap.empty()) ? countInserted : vertexAdjacencyMap.begin()->second.size() + countInserted;

    /*
     * push a false neighbor flags into all the vertices neighborhood rows
     * indicating a false neighborhood with the new vertices
     */
    for (auto& nodeNeighbours : vertexAdjacencyMap) {
        nodeNeighbours.second.resize(nodesCount, false);
    }
}

bool AdjacencyMatrix::removeVeretex(std::string vertexId) {
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

bool AdjacencyMatrix::addNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
    //check for the existence of the vertexId & neighbourVertexId
    if ((vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() //|| this->insertVertex(vertexId)
            ) &&
            (vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end() //|| this->insertVertex(neighbourVertexId)
            )) {
        //set the neighborhood flag between the two vertices to true
        vertexAdjacencyMap.at(vertexId).at(getVertexIndexByVertexId(neighbourVertexId)) = true;
        return true;
    } else {
        return false;
    }
}

void AdjacencyMatrix::addNeighbourVertex(std::vector<std::pair<std::string, std::string> > &edges) {
    //std::vector<bool> result;
    //loop over all VertexNeighbourIds pairs and add them one by one
    for (std::vector<std::pair<std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::pair<std::string, std::string> neighbourPairIds = *it;
        this->addNeighbourVertex(neighbourPairIds.first, neighbourPairIds.second);
    }
    //return result;
}

bool AdjacencyMatrix::removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
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

std::list<std::string> AdjacencyMatrix::getNeighbourVertices(std::string vertexId) {
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

uint64_t AdjacencyMatrix::getVertexIndexByVertexId(std::string vertexId) {
    return this->vertexIndexMap.at(vertexId);
}

std::string AdjacencyMatrix::getVertexIdByVertexIndex(uint64_t vertexIndex) {
    for (auto& vertexIdToIndex : vertexIndexMap) {
        if (vertexIdToIndex.second == vertexIndex) {
            return vertexIdToIndex.first;
        }
    }
    return "";
}

uint64_t AdjacencyMatrix::getAdjacencyMatrixSize() {
    return this->vertexAdjacencyMap.size();
}