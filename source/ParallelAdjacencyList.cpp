/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"ParallelAdjacencyList.hpp"

ParallelAdjacencyList::ParallelAdjacencyList() {
    //globalAlMutex.unlock();
}

ParallelAdjacencyList::~ParallelAdjacencyList() {
    //for (AL_it it = this->vertexAdjacencyMap.begin(); it != this->vertexAdjacencyMap.end(); it++) {
    //    it->second.first.reset();
    //}
}

/*
std::pair < ParallelAdjacencyList::MyMap_it, bool> ParallelAdjacencyList::insertVertex(std::string vertexId) {
    std::pair < MyMap_it, bool> insertResult;
    insertResult.second = false;

    std::shared_lock<std::shared_timed_mutex> lockGlobal(globalAlMutex);
    insertResult.first = vertexAdjacencyMap.find(vertexId);

    if (insertResult.first == vertexAdjacencyMap.end()) {
        lockGlobal.unlock();
        std::lock_guard<std::shared_timed_mutex> lock(globalAlMutex);

        //insert the new vertexId into the vertexAdjacencyMap
        std::unordered_map<std::string, MapItContainers > labeledNeighbourVertexMap;
        insertResult =
                vertexAdjacencyMap.emplace(vertexId, std::make_pair(std::make_shared<std::shared_timed_mutex>(), labeledNeighbourVertexMap));
    }
    return insertResult;
}

void ParallelAdjacencyList::insertVertex(std::set<std::string> &vertexIds) {
    std::vector<std::string> newVertexIds;

    std::shared_lock<std::shared_timed_mutex> lockGlobal(globalAlMutex);

    for (std::set<std::string>::iterator it = vertexIds.begin(); it != vertexIds.end(); ++it) {
        if (vertexAdjacencyMap.find(*it) == vertexAdjacencyMap.end()) {
            newVertexIds.emplace_back(*it);
        }
    }

    std::unordered_map<std::string, MapItContainers > labeledNeighbourVertexMap;
    if (!newVertexIds.empty()) {
        lockGlobal.unlock();
        std::lock_guard<std::shared_timed_mutex> lock(globalAlMutex);
        //loop over the vertexIds, and insert them one by one
        for (std::vector<std::string>::iterator it = newVertexIds.begin(); it != newVertexIds.end(); ++it) {
            this->vertexAdjacencyMap.emplace(*it, std::make_pair(std::make_shared<std::shared_timed_mutex>(), labeledNeighbourVertexMap));
        }
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
 */

bool ParallelAdjacencyList::addNeighbourVertex(std::string vertexId, std::string edgeLabel, std::string neighbourVertexId) {
    std::shared_lock<std::shared_timed_mutex> lockGlobal(globalAlMutex);
    AL_it edge_it = this->vertexAdjacencyMap.find(edgeLabel);
    std::pair< AL_it, bool> alEntry;
    if (edge_it == this->vertexAdjacencyMap.end()) {
        lockGlobal.unlock();
        std::lock_guard<std::shared_timed_mutex> lock(globalAlMutex);

        //check for the existence of the vertexId & neighbourVertexId
        alEntry = this->vertexAdjacencyMap.emplace(edgeLabel, std::make_pair(std::make_shared<std::shared_timed_mutex>(), std::map<std::string, MapItContainers >()));
        lockGlobal.lock();
    } else {
        alEntry.first = edge_it;
        alEntry.second = false;
    }

    std::shared_timed_mutex* innerMutex = alEntry.first->second.first.get();
    std::lock_guard<std::shared_timed_mutex> lock(*innerMutex);

    std::pair < MyMap_it, bool> srcIt =
            alEntry.first->second.second.emplace(vertexId, MapItContainers());
    std::pair < MyMap_it, bool> tgtIt =
            alEntry.first->second.second.emplace(neighbourVertexId, MapItContainers());


    //set the neighbourVertexId as a neighbor to the vertexId with the edgeLabel 
    bool inserted = false;
    for (int i = 0; i < srcIt.first->second.vec.size(); i++) {
        if (srcIt.first->second.vec.at(i)->first < tgtIt.first->first) {
            continue;
        } else if (srcIt.first->second.vec.at(i)->first > tgtIt.first->first) {
            srcIt.first->second.vec.emplace(srcIt.first->second.vec.begin() + i, tgtIt.first);
            inserted = true;
            break;
        } else {
            inserted = true;
            break;
        }
    }
    if (!inserted) {
        srcIt.first->second.vec.emplace_back(tgtIt.first);
    }

    return true;
}

void ParallelAdjacencyList::addNeighbourVertex(std::vector<std::tuple<std::string, std::string, std::string> > &edges) {

    if (edges.empty()) {
        return;
    }

    std::string edgeLabel = std::get<1>(edges.front());

    std::shared_lock<std::shared_timed_mutex> lockGlobal(globalAlMutex);

    AL_it edge_it = this->vertexAdjacencyMap.find(edgeLabel);
    lockGlobal.unlock();
    std::pair< AL_it, bool> alEntry;
    if (edge_it == this->vertexAdjacencyMap.end()) {
        std::lock_guard<std::shared_timed_mutex> lock(globalAlMutex);
        //check for the existence of the vertexId & neighbourVertexId
        alEntry = this->vertexAdjacencyMap.emplace(edgeLabel, std::make_pair(std::make_shared<std::shared_timed_mutex>(), std::map<std::string, MapItContainers >()));
    } else {
        alEntry.first = edge_it;
        alEntry.second = false;
    }

    for (std::vector<std::tuple<std::string, std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::shared_timed_mutex* innerMutex = alEntry.first->second.first.get();


        std::pair < MyMap_it, bool> srcIt, tgtIt;

        //std::shared_lock<std::shared_timed_mutex> innerLock(*innerMutex);
        srcIt.first = alEntry.first->second.second.find(std::get<0>(*it));
        tgtIt.first = alEntry.first->second.second.find(std::get<2>(*it));
        //innerLock.unlock();

        if (srcIt.first == alEntry.first->second.second.end()) {
            std::lock_guard<std::shared_timed_mutex> lock(*innerMutex);
            srcIt = alEntry.first->second.second.emplace(std::get<0>(*it), MapItContainers());
        }
        if (tgtIt.first == alEntry.first->second.second.end()) {
            std::lock_guard<std::shared_timed_mutex> lock(*innerMutex);
            tgtIt = alEntry.first->second.second.emplace(std::get<2>(*it), MapItContainers());
        }

        {
            std::shared_timed_mutex* vecMutex = srcIt.first->second.vecMutex.get();
            std::shared_lock<std::shared_timed_mutex> vecLock(*vecMutex);
            //set the neighbourVertexId as a neighbor to the vertexId with the edgeLabel 
            bool inserted = false;
            for (int i = 0; i < srcIt.first->second.vec.size(); i++) {
                if (srcIt.first->second.vec.at(i)->first < tgtIt.first->first) {
                    continue;
                } else if (srcIt.first->second.vec.at(i)->first > tgtIt.first->first) {
                    vecLock.unlock();
                    std::lock_guard<std::shared_timed_mutex> lock(*vecMutex);
                    srcIt.first->second.vec.emplace(srcIt.first->second.vec.begin() + i, tgtIt.first);
                    inserted = true;
                    break;
                } else {
                    inserted = true;
                    break;
                }
            }
            if (!inserted) {
                vecLock.unlock();
                std::lock_guard<std::shared_timed_mutex> lock(*vecMutex);
                srcIt.first->second.vec.emplace_back(tgtIt.first);
            }
        }
    }
}

/*
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
 

std::unordered_map<std::string, ParallelAdjacencyList::MapItContainers > ParallelAdjacencyList::getNeighbourVertices(std::string vertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {
        return vertexAdjacencyMap.at(vertexId).second;
    } else {
        return {};
    }
}


uint64_t ParallelAdjacencyList::getVertexIndexByVertexId(std::string vertexId) {
    return std::distance(vertexAdjacencyMap.begin(), vertexAdjacencyMap.find(vertexId));
}
 */

uint64_t ParallelAdjacencyList::getAdjacencyListSize() {
    std::shared_lock<std::shared_timed_mutex> lock(globalAlMutex);
    uint64_t size = 0;
    for (AL_it it = this->vertexAdjacencyMap.begin(); it != this->vertexAdjacencyMap.end(); it++) {
        size += it->second.second.size();
    }
    return size;
}
