/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"AdjacencyList.hpp"

AdjacencyList::AdjacencyList() {

}

bool AdjacencyList::insertVertex(std::string vertexId) {
    //insert the new vertexId into the vertexAdjacencyMap
    std::map<std::string, std::vector<std::string> > labeledNeighbourVertexMap;
    std::pair < std::unordered_map<std::string, std::map<std::string, std::vector<std::string> > >::iterator, bool> insertResult =
            vertexAdjacencyMap.insert(std::make_pair(vertexId, labeledNeighbourVertexMap));
    if (insertResult.second) {
        return true;
    } else {
        return false;
    }
}

void AdjacencyList::insertVertex(std::set<std::string> &vertexIds) {
    //loop over the vertexIds, and insert them one by one
    for (std::set<std::string>::iterator it = vertexIds.begin(); it != vertexIds.end(); ++it) {
        std::string vertexId = *it;
        this->insertVertex(vertexId);
    }
}

bool AdjacencyList::removeVertex(std::string vertexId) {
    //check for the existence of the vertexId to be removed
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {

        //remove the vertex from the neighborhood of any other vertex
        for (auto& vertexToLabeledNeighbourVertexMap : vertexAdjacencyMap) {
            for (auto& labelToNeighbourVertices : vertexToLabeledNeighbourVertexMap.second) {
                std::vector<std::string>::iterator it = std::find(labelToNeighbourVertices.second.begin(), labelToNeighbourVertices.second.end(), vertexId);
                if (it != labelToNeighbourVertices.second.end()) {
                    labelToNeighbourVertices.second.erase(it);
                }
            }
        }

        //remove the vertex from the vertexAdjacencyMap
        vertexAdjacencyMap.erase(vertexId);

        return true;
    } else {
        return false;
    }
}

bool AdjacencyList::addNeighbourVertex(std::string vertexId, std::string edgeLabel, std::string neighbourVertexId) {
    //check for the existence of the vertexId & neighbourVertexId
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {
        //set the neighbourVertexId as a neighbor to the vertexId with the edgeLabel 
        vertexAdjacencyMap.at(vertexId)[edgeLabel].push_back(neighbourVertexId);
        return true;
    } else {
        return false;
    }
}

std::vector<bool> AdjacencyList::addNeighbourVertex(std::vector<std::tuple<std::string, std::string, std::string> > &edges) {
    std::vector<bool> result;
    //loop over all edges and add them one by one
    for (std::vector<std::tuple<std::string, std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::tuple<std::string, std::string, std::string> edge = *it;
        result.push_back(this->addNeighbourVertex(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge)));
    }
    return result;
}

bool AdjacencyList::removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
    //check for the existence of the vertexId & neighbourVertexId
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {

        //remove the vertex from the neighborhood of the neighbor vertex
        for (auto& labelToNeighbourVertices : vertexAdjacencyMap.at(vertexId)) {
            std::vector<std::string>::iterator it = std::find(labelToNeighbourVertices.second.begin(), labelToNeighbourVertices.second.end(), neighbourVertexId);
            if (it != labelToNeighbourVertices.second.end()) {
                labelToNeighbourVertices.second.erase(it);
            }
        }
        return true;
    } else {
        return false;
    }
}

bool AdjacencyList::removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId, std::string edgeLabel) {
    //check for the existence of the vertexId & neighbourVertexId
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {
        
        //remove the vertex from the neighborhood of the neighbor vertex
        std::vector<std::string>::iterator it = std::find(vertexAdjacencyMap.at(vertexId).at(edgeLabel).begin(), vertexAdjacencyMap.at(vertexId).at(edgeLabel).end(), neighbourVertexId);
        if (it != vertexAdjacencyMap.at(vertexId).at(edgeLabel).end()) {
            vertexAdjacencyMap.at(vertexId).at(edgeLabel).erase(it);
        }
        return true;
    } else {
        return false;
    }
}

std::map<std::string, std::vector<std::string> > AdjacencyList::getNeighbourVertices(std::string vertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {
        return vertexAdjacencyMap.at(vertexId);
    } else {
        return {};
    }
}

uint64_t AdjacencyList::getVertexIndexByVertexId(std::string vertexId) {
    return std::distance(vertexAdjacencyMap.begin(), vertexAdjacencyMap.find(vertexId));
}

uint64_t AdjacencyList::getAdjacencyListSize() {
    return this->vertexAdjacencyMap.size();
}
