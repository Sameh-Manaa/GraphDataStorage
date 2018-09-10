#include"ParallelAdjacencyList.hpp"

void ParallelAdjacencyList::addNeighbourVertex(std::string edgeLabel, std::map<std::string, std::set<std::string> > &edges) {

    if (edges.empty()) {
        return;
    }

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

    std::shared_timed_mutex* innerMutex = alEntry.first->second.first.get();
    {
        std::lock_guard<std::shared_timed_mutex> lock(*innerMutex);
        for (std::map<std::string, std::set<std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {

            std::pair < MyMap_it, bool> srcIt, tgtIt;

            srcIt.first = alEntry.first->second.second.find(it->first);

            if (srcIt.first == alEntry.first->second.second.end()) {
                srcIt = alEntry.first->second.second.emplace(it->first, MapItContainers());
            }

            int i = 0;
            for (std::set<std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {

                tgtIt.first = alEntry.first->second.second.find(*it2);

                if (tgtIt.first == alEntry.first->second.second.end()) {
                    //std::lock_guard<std::shared_timed_mutex> lock(*innerMutex);
                    tgtIt = alEntry.first->second.second.emplace(*it2, MapItContainers());
                }

                {
                    //std::shared_timed_mutex* vecMutex = srcIt.first->second.vecMutex.get();
                    //std::shared_lock<std::shared_timed_mutex> vecLock(*vecMutex);
                    //set the neighbourVertexId as a neighbor to the vertexId with the edgeLabel 
                    bool inserted = false;
                    for (; i < srcIt.first->second.vec.size(); i++) {
                        if (srcIt.first->second.vec.at(i)->first < *it2) {
                            continue;
                        } else if (srcIt.first->second.vec.at(i)->first > *it2) {
                            //vecLock.unlock();
                            //std::lock_guard<std::shared_timed_mutex> lock(*vecMutex);
                            srcIt.first->second.vec.emplace(srcIt.first->second.vec.begin() + i, tgtIt.first);
                            inserted = true;
                            break;
                        } else {
                            inserted = true;
                            break;
                        }
                    }
                    if (!inserted) {
                        //vecLock.unlock();
                        //std::lock_guard<std::shared_timed_mutex> lock(*vecMutex);
                        srcIt.first->second.vec.emplace_back(tgtIt.first);
                        i++;
                    }
                }
            }
        }
    }
}

uint64_t ParallelAdjacencyList::getAdjacencyListSize() {
    std::shared_lock<std::shared_timed_mutex> lock(globalAlMutex);
    uint64_t size = 0;
    for (AL_it it = this->vertexAdjacencyMap.begin(); it != this->vertexAdjacencyMap.end(); it++) {
        size += it->second.second.size();
    }
    return size;
}
