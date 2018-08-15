/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"AdjacencyMatrix.hpp"

void AdjacencyMatrix::resize(std::string edgeLabel) {
    if (this->labeledAdjacencyMatrix.at(edgeLabel).second.size() < this->labeledAdjacencyMatrix.at(edgeLabel).first.size()) {
        //construct a new vertex neighborhood row
        std::vector<bool> neighbours(this->labeledAdjacencyMatrix.at(edgeLabel).first.size() + this->batchSize, false);

        for (auto& nodeNeighbours : this->labeledAdjacencyMatrix.at(edgeLabel).second) {
            nodeNeighbours.resize(neighbours.size(), false);
        }

        this->labeledAdjacencyMatrix.at(edgeLabel).second.resize(neighbours.size(), neighbours);
    }
}

void AdjacencyMatrix::shrinkToFit() {
    for (auto &adjMatEntry : this->labeledAdjacencyMatrix) {
        if (adjMatEntry.second.second.size() > adjMatEntry.second.first.size()) {

            adjMatEntry.second.second.resize(adjMatEntry.second.first.size());

            for (auto& nodeNeighbours : adjMatEntry.second.second) {
                nodeNeighbours.resize(adjMatEntry.second.first.size());
            }

        }
    }
}

void AdjacencyMatrix::setBatchSize(int batchSize) {
    this->batchSize = batchSize;
}

/*
std::pair<AdjacencyMatrix::adjMat_it, bool> AdjacencyMatrix::insertVertex(std::string vertexId) {
    uint64_t nodesCount = vertexIndexMap.size();
    
    //insert the vertex neighborhood row along with its vertexId into the vertexAdjacencyMap
    //(IF NOT ALREADY THERE)
     
    //insert a pair of the new vertexId and its vertexIndex into the vertexIndexMap
    std::pair<adjMat_it, bool> insertResult = this->vertexIndexMap.insert(std::make_pair(vertexId, nodesCount));

    this->resize();

    return insertResult;
}

void AdjacencyMatrix::insertVertex(std::set<std::string> &vertexIds) {

    //loop over all the vertexIds to insert them into the data structures
    for (std::set<std::string>::iterator it = vertexIds.begin(); it != vertexIds.end(); ++it) {

        //insert a pair of the new vertexId and its vertexIndex into the vertexIndexMap
        this->vertexIndexMap.insert(std::make_pair(*it, this->vertexIndexMap.size()));
    }

    this->resize();

}


bool AdjacencyMatrix::removeVeretex(std::string vertexId) {
    //check for the existence of the vertexId to be removed
    if (vertexIndexMap.find(vertexId) != vertexIndexMap.end()) {
        uint64_t nodeIndex = getVertexIndexByVertexId(vertexId);

        //erase the vertex neighborhood flag from all the other vertices stored
        for (auto& nodeNeighbours : vertexAdjacencyMap) {
            nodeNeighbours.erase(nodeNeighbours.begin() + nodeIndex);
        }

        //erase the vertex row from the vertexAdjacencyMap and vertexIndexMap
        vertexAdjacencyMap.erase(vertexAdjacencyMap.begin() + nodeIndex);
        vertexIndexMap.erase(vertexId);
        for (auto& vertexIdToIndex : vertexIndexMap) {
            if (vertexIdToIndex.second > nodeIndex) {
                vertexIdToIndex.second -= 1;
            }
        }

        return true;
    } else {
        return false;
    }
}
 */

bool AdjacencyMatrix::addNeighbourVertex(std::string vertexId, std::string edgeLabel, std::string neighbourVertexId) {
    std::pair< labeledAdjMat_it, bool> adjMatEntry = this->labeledAdjacencyMatrix.emplace(edgeLabel, std::pair<vertexIndex_map, vertexAdjacency_map>());

    std::pair < vertexIndex_it, bool> srcIt =
            adjMatEntry.first->second.first.emplace(vertexId, adjMatEntry.first->second.first.size());
    std::pair < vertexIndex_it, bool> tgtIt =
            adjMatEntry.first->second.first.emplace(neighbourVertexId, adjMatEntry.first->second.first.size());

    this->resize(edgeLabel);

    adjMatEntry.first->second.second[srcIt.first->second][tgtIt.first->second] = true;

    //set the neighborhood flag between the two vertices to true
    return true;
}

void AdjacencyMatrix::addNeighbourVertex(std::vector<std::tuple<std::string, std::string, std::string> > &edges) {
    if (edges.empty()) {
        return;
    }

    std::string edgeLabel = std::get<1>(edges.front());
    std::pair< labeledAdjMat_it, bool> adjMatEntry = this->labeledAdjacencyMatrix.emplace(edgeLabel, std::pair<vertexIndex_map, vertexAdjacency_map>());


    for (std::vector<std::tuple<std::string, std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::pair < vertexIndex_it, bool> srcIt =
                adjMatEntry.first->second.first.emplace(std::get<0>(*it), adjMatEntry.first->second.first.size());
        std::pair < vertexIndex_it, bool> tgtIt =
                adjMatEntry.first->second.first.emplace(std::get<2>(*it), adjMatEntry.first->second.first.size());
    }
    this->resize(edgeLabel);

    for (std::vector<std::tuple<std::string, std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        uint64_t sourceVertexIndex = adjMatEntry.first->second.first.at(std::get<0>(*it));
        uint64_t targetVertexIndex = adjMatEntry.first->second.first.at(std::get<2>(*it));
        adjMatEntry.first->second.second[sourceVertexIndex][targetVertexIndex] = true;
    }
}

/*
bool AdjacencyMatrix::removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
    //check for the existence of the vertexId & neighbourVertexId
    if (vertexIndexMap.find(vertexId) != vertexIndexMap.end() &&
            vertexIndexMap.find(neighbourVertexId) != vertexIndexMap.end()) {
        //set the neighborhood flag between the two vertices to false
        vertexAdjacencyMap.at(getVertexIndexByVertexId(vertexId)).at(getVertexIndexByVertexId(neighbourVertexId)) = false;
        return true;
    } else {
        return false;
    }
}

std::list<std::string> AdjacencyMatrix::getNeighbourVertices(std::string vertexId) {
    std::list<std::string> neighbourList(0);
    //check for the existence of the vertexId
    if (vertexIndexMap.find(vertexId) != vertexIndexMap.end()) {
        //loop over the vertex neighborhood flags to get the vertex neighbors
        for (uint64_t i = 0; i != vertexIndexMap.size(); i++) {
            if (vertexAdjacencyMap.at(getVertexIndexByVertexId(vertexId))[i]) {
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
 */
std::string AdjacencyMatrix::getVertexIdByVertexIndex(uint64_t vertexIndex, std::string edgeLabel) {
    labeledAdjMat_it labeledEdgesSet_it = this->labeledAdjacencyMatrix.find(edgeLabel);
    for (auto& vertexIdToIndex : labeledEdgesSet_it->second.first) {
        if (vertexIdToIndex.second == vertexIndex) {
            return vertexIdToIndex.first;
        }
    }
    return "";
}

uint64_t AdjacencyMatrix::getAdjacencyMatrixSize() {
    uint64_t size = 0;
    for (labeledAdjMat_it it = this->labeledAdjacencyMatrix.begin(); it != this->labeledAdjacencyMatrix.end(); it++) {
        size += it->second.first.size();
    }
    return size;
}

void AdjacencyMatrix::getTargetVertex(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    labeledAdjMat_it labeledEdgesSet_it = this->labeledAdjacencyMatrix.find(edgeLabel);

    for (uint32_t i = 0; i < resultSet.size(); i++) {
        std::string sourceVertexId = resultSet[i].first[0];
        uint32_t sourceVertexIndex = labeledEdgesSet_it->second.first.at(sourceVertexId);
        std::vector<bool>& neighboursVector = labeledEdgesSet_it->second.second.at(sourceVertexIndex);
        int32_t targetVertexIndex = sourceVertexIndex;
        sourceVertexIndex = -1;
        for (uint32_t i = 0; i < neighboursVector.size(); i++) {
            if (neighboursVector[i]) {
                targetVertexIndex = i;
                sourceVertexIndex = i;
                break;
            }
        }

        while (sourceVertexIndex != -1) {
            std::vector<bool>& neighboursVector = labeledEdgesSet_it->second.second.at(sourceVertexIndex);
            sourceVertexIndex = -1;
            for (uint32_t i = 0; i < neighboursVector.size(); i++) {
                if (neighboursVector[i]) {
                    targetVertexIndex = i;
                    sourceVertexIndex = i;
                    break;
                }
            }
        }
        resultSet[i].first.emplace_back(getVertexIdByVertexIndex(targetVertexIndex, edgeLabel));
    }
}

void AdjacencyMatrix::getTargetVertexWithReplacement(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    labeledAdjMat_it labeledEdgesSet_it = this->labeledAdjacencyMatrix.find(edgeLabel);

    for (uint32_t i = 0; i < resultSet.size(); i++) {
        std::string sourceVertexId = resultSet[i].first[0];
        uint32_t sourceVertexIndex = labeledEdgesSet_it->second.first.at(sourceVertexId);
        std::vector<bool>& neighboursVector = labeledEdgesSet_it->second.second.at(sourceVertexIndex);
        int32_t targetVertexIndex = sourceVertexIndex;
        sourceVertexIndex = -1;
        for (uint32_t i = 0; i < neighboursVector.size(); i++) {
            if (neighboursVector[i]) {
                targetVertexIndex = i;
                sourceVertexIndex = i;
                break;
            }
        }

        while (sourceVertexIndex != -1) {
            std::vector<bool>& neighboursVector = labeledEdgesSet_it->second.second.at(sourceVertexIndex);
            sourceVertexIndex = -1;
            for (uint32_t i = 0; i < neighboursVector.size(); i++) {
                if (neighboursVector[i]) {
                    targetVertexIndex = i;
                    sourceVertexIndex = i;
                    break;
                }
            }
        }
        resultSet[i].first[0] = getVertexIdByVertexIndex(targetVertexIndex, edgeLabel);
    }
}

void AdjacencyMatrix::getAllEdges(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    for (auto const &labeledAdjMat : this->labeledAdjacencyMatrix) {
        vertexIndex_map const &viMap = labeledAdjMat.second.first;
        vertexAdjacency_map const &vaMap = labeledAdjMat.second.second;
        for (auto const &sourceVertex : viMap) {
            auto const &neighboursVector = vaMap.at(sourceVertex.second);
            for (uint32_t i = 0; i < neighboursVector.size() && neighboursVector.at(i); i++) {
                resultSet.emplace_back(std::make_pair(std::vector<std::string>() = {"out", labeledAdjMat.first, sourceVertex.first}
                , std::vector<double>() = {1}));
                resultSet.emplace_back(std::make_pair(std::vector<std::string>() = {"in", labeledAdjMat.first, this->getVertexIdByVertexIndex(i, labeledAdjMat.first)}
                , std::vector<double>() = {1}));
            }
        }
    }
}