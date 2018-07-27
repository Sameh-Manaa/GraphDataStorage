/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"ParallelAdjacencyList.hpp"

ParallelAdjacencyList::ParallelAdjacencyList() {
    //globalAlMutex.unlock();
}

bool ParallelAdjacencyList::insertVertex(std::string vertexId) {
    //insert the new vertexId into the vertexAdjacencyMap
    std::map<std::string, std::vector<std::string> > labeledNeighbourVertexMap;
    std::pair < std::map<std::string, std::pair<std::shared_ptr<std::shared_timed_mutex>, std::map<std::string, std::vector<std::string> > > >::iterator, bool> insertResult =
            vertexAdjacencyMap.emplace(vertexId, std::make_pair(std::make_shared<std::shared_timed_mutex>(),labeledNeighbourVertexMap));
    if (insertResult.second) {
        return true;
    } else {
        return false;
    }
}

void ParallelAdjacencyList::insertVertex(std::set<std::string> &vertexIds) {
    std::lock_guard<std::shared_timed_mutex> lock(globalAlMutex);
    //loop over the vertexIds, and insert them one by one
    for (std::set<std::string>::iterator it = vertexIds.begin(); it != vertexIds.end(); ++it) {
        std::string vertexId = *it;
        this->insertVertex(vertexId);
    }
}

bool ParallelAdjacencyList::removeVertex(std::string vertexId) {
    //check for the existence of the vertexId to be removed
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {

        //remove the vertex from the neighborhood of any other vertex
        for (auto& vertexToLabeledNeighbourVertexMap : vertexAdjacencyMap) {
            for (auto& labelToNeighbourVertices : vertexToLabeledNeighbourVertexMap.second.second) {
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

bool ParallelAdjacencyList::addNeighbourVertex(std::string vertexId, std::string edgeLabel, std::string neighbourVertexId) {
    //check for the existence of the vertexId & neighbourVertexId
    if ((vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() //|| this->insertVertex(vertexId)
            ) &&
            (vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end() //|| this->insertVertex(neighbourVertexId)
            )) {
        //set the neighbourVertexId as a neighbor to the vertexId with the edgeLabel 
        std::shared_timed_mutex* innerMutex = vertexAdjacencyMap.at(vertexId).first.get();
        std::lock_guard<std::shared_timed_mutex> lock(*innerMutex);
        vertexAdjacencyMap.at(vertexId).second[edgeLabel].emplace_back(neighbourVertexId);
        return true;
    } else {
        return false;
    }
}

void ParallelAdjacencyList::addNeighbourVertex(std::vector<std::tuple<std::string, std::string, std::string> > &edges) {
    std::shared_lock<std::shared_timed_mutex> lock(globalAlMutex);
    //std::vector<bool> result;
    //loop over all edges and add them one by one
    for (std::vector<std::tuple<std::string, std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::tuple<std::string, std::string, std::string> edge = *it;
        this->addNeighbourVertex(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
    }
    //return result;
}

bool ParallelAdjacencyList::removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId) {
    //check for the existence of the vertexId & neighbourVertexId
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {

        //remove the vertex from the neighborhood of the neighbor vertex
        for (auto& labelToNeighbourVertices : vertexAdjacencyMap.at(vertexId).second) {
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

bool ParallelAdjacencyList::removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId, std::string edgeLabel) {
    //check for the existence of the vertexId & neighbourVertexId
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end() &&
            vertexAdjacencyMap.find(neighbourVertexId) != vertexAdjacencyMap.end()) {

        //remove the vertex from the neighborhood of the neighbor vertex
        std::vector<std::string>::iterator it = std::find(vertexAdjacencyMap.at(vertexId).second.at(edgeLabel).begin(), vertexAdjacencyMap.at(vertexId).second.at(edgeLabel).end(), neighbourVertexId);
        if (it != vertexAdjacencyMap.at(vertexId).second.at(edgeLabel).end()) {
            vertexAdjacencyMap.at(vertexId).second.at(edgeLabel).erase(it);
        }
        return true;
    } else {
        return false;
    }
}

std::map<std::string, std::vector<std::string> > ParallelAdjacencyList::getNeighbourVertices(std::string vertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {
        return vertexAdjacencyMap.at(vertexId).second;
    } else {
        return {};
    }
}

uint64_t ParallelAdjacencyList::getVertexIndexByVertexId(std::string vertexId) {
    return std::distance(vertexAdjacencyMap.begin(), vertexAdjacencyMap.find(vertexId));
}

uint64_t ParallelAdjacencyList::getAdjacencyListSize() {
    std::shared_lock<std::shared_timed_mutex> lock(globalAlMutex);
    return this->vertexAdjacencyMap.size();
}
