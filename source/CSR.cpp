/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"CSR.hpp"

CSR::CSR() {
    //this->vertexRowIndex.emplace_back(0);
}

CSR::~CSR() {
    /* 
     for (auto list_idx : this->vertexColumnIndex) {
         delete list_idx;
     }

     this->vertexColumnIndex.clear();
     this->vertexIndexMap.clear();
     this->vertexRowIndex.clear();
     */
}
/*
std::pair<CSR::vertexMap_it, bool> CSR::insertVertex(std::string vertexId) {

    
    MapItContainers targetVerticesVector;

    std::pair<vertexMap_it, bool> inserted = this->vertexIndexMap.insert(std::make_pair(vertexId, targetVerticesVector));

    return inserted;
     


    uint64_t vertexCount = this->vertexRowIndex.size() - 1;

    std::pair<vertexMap_it, bool> inserted = this->vertexIndexMap.insert(std::make_pair(vertexId, vertexCount));

    if (inserted.second) {
        this->vertexRowIndex.emplace_back(this->vertexRowIndex.at(vertexCount));
        this->vertexColumnIndex.resize(this->vertexColumnIndex.size() + 1);
    }
    return inserted;

}
 */
/*
void CSR::insertVertex(std::set<std::string> &vertexIds) {

    
    MapItContainers targetVerticesVector;
    
    for (std::set<std::string>::iterator it = vertexIds.begin(); it != vertexIds.end(); ++it) {
        this->vertexIndexMap.insert(std::make_pair(*it, targetVerticesVector));
    }
    



    uint64_t vertexCount = this->vertexRowIndex.size() - 1;
    uint64_t vertexCountBefore = vertexCount;

    //loop over all the vertexIds to index them
    for (std::set<std::string>::iterator it = vertexIds.begin(); it != vertexIds.end(); ++it) {
        std::string vertexId = *it;
        bool inserted = this->vertexIndexMap.insert(std::make_pair(vertexId, vertexCount)).second;
        if (inserted) {
            vertexCount++;
        }
    }

    this->vertexRowIndex.resize(vertexCount + 1, this->vertexRowIndex.at(vertexCountBefore));
    this->vertexColumnIndex.resize(vertexCount);

}
 */

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
bool CSR::addNeighbourVertex(std::string vertexId, std::string edgeLabel, std::string neighbourVertexId) {

    std::pair<labeledCSR_it, bool> csrEntry = this->labeledCSR.emplace(edgeLabel, CSR_tuple());

    CSR_tuple csrTuple = csrEntry.first->second;
    vertexIndexMap viMap = std::get<0>(csrTuple);
    vertexRowIndex vrIndex = std::get<1>(csrTuple);
    vertexColumnIndex vcIndex = std::get<2>(csrTuple);

    if (csrEntry.second) {
        vrIndex.emplace_back(0);
    }

    std::pair<vertexMap_it, bool> tgtIt = viMap.emplace(neighbourVertexId, viMap.size());
    std::pair<vertexMap_it, bool> srcIt = viMap.emplace(vertexId, viMap.size());

    if (tgtIt.second) {
        vrIndex.emplace_back(vrIndex.back());
    }

    if (srcIt.second) {
        vrIndex.emplace_back(vrIndex.back() + 1);
        vcIndex.emplace_back(tgtIt.first);
    } else {
        bool inserted = false;
        for (uint64_t i = vrIndex.at(srcIt.first->second); i < vrIndex.at(srcIt.first->second + 1); i++) {
            if (vcIndex.at(i)->second < tgtIt.first->second) {
                continue;
            } else if (vcIndex.at(i)->second > tgtIt.first->second) {
                vcIndex.emplace(vcIndex.begin() + i, tgtIt.first);
                vrIndex[srcIt.first->second + 1]++;
                inserted = true;
                break;
            } else {
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            vcIndex.emplace(vcIndex.begin() + vrIndex[srcIt.first->second + 1], tgtIt.first);
            vrIndex[srcIt.first->second + 1]++;
        }
    }
    return true;

    /*
    std::pair< vertexMap_it, bool> srcIt = this->insertVertex(vertexId);
    std::pair< vertexMap_it, bool> tgtIt = this->insertVertex(neighbourVertexId);
    
    //set the neighbourVertexId as a neighbor to the vertexId with the edgeLabel 
    srcIt.first->second.vec.push_back(tgtIt.first);
    return true;
     */

    /*
    uint64_t sourceVertexIndex, targetVertexIndex;

    sourceVertexIndex = this->getVertexIndexByVertexId(vertexId);
    targetVertexIndex = this->getVertexIndexByVertexId(neighbourVertexId);

    if (!this->vertexColumnIndex.at(sourceVertexIndex)) {
        this->vertexColumnIndex.at(sourceVertexIndex) = new std::list<uint64_t>;
    }

    uint64_t idx;
    std::list<uint64_t>::iterator list_it = this->vertexColumnIndex.at(sourceVertexIndex)->begin();

    for (idx = this->vertexRowIndex.at(sourceVertexIndex);
            idx < this->vertexRowIndex.at(sourceVertexIndex + 1) && *list_it < targetVertexIndex;
            idx++, list_it++) {
    }

    //col_it = this->vertexColumnIndex.begin();
    //std::advance(col_it, this->vertexRowIndex.at(sourceVertexIndex));

    if (idx == this->vertexRowIndex.at(sourceVertexIndex + 1) ||
     *list_it > targetVertexIndex) {
        this->vertexColumnIndex.at(sourceVertexIndex)->emplace(list_it, targetVertexIndex);

        for (uint64_t i = sourceVertexIndex + 1; i < this->vertexRowIndex.size(); i++) {
            this->vertexRowIndex.at(i) += 1;
        }
        return true;
    } else {
        return false;
    }
     */

}

void CSR::addNeighbourVertex(std::vector<std::tuple<std::string, std::string, std::string> > &edges) {

    if (edges.empty()) {
        return;
    }

    std::string edgeLabel = std::get<1>(edges.front());

    std::pair<labeledCSR_it, bool> csrEntry = this->labeledCSR.emplace(edgeLabel, CSR_tuple());

    CSR_tuple &csrTuple = csrEntry.first->second;
    vertexIndexMap &viMap = std::get<0>(csrTuple);
    vertexRowIndex &vrIndex = std::get<1>(csrTuple);
    vertexColumnIndex &vcIndex = std::get<2>(csrTuple);

    if (csrEntry.second) {
        vrIndex.emplace_back(0);
    }

    int countInserted = 0;

    for (auto &edge : edges) {

        std::string sourceVertexId = std::get<0>(edge);
        std::string targetVertexId = std::get<2>(edge);

        std::pair<vertexMap_it, bool> tgtIt = viMap.insert(std::make_pair(targetVertexId, viMap.size()));
        std::pair<vertexMap_it, bool> srcIt = viMap.insert(std::make_pair(sourceVertexId, viMap.size()));

        if (tgtIt.second) {
            vrIndex.emplace_back(vrIndex.back());
            countInserted++;
        }

        if (srcIt.second) {
            vrIndex.emplace_back(vrIndex.back() + 1);
            vcIndex.emplace_back(tgtIt.first);
            countInserted++;
        } else {
            bool inserted = false;
            for (uint64_t i = vrIndex.at(srcIt.first->second); i < vrIndex.at(srcIt.first->second + 1); i++) {
                if (vcIndex.at(i)->second < tgtIt.first->second) {
                    continue;
                } else if (vcIndex.at(i)->second > tgtIt.first->second) {
                    vcIndex.emplace(vcIndex.begin() + i, tgtIt.first);
                    vrIndex[srcIt.first->second + 1]++;
                    inserted = true;
                    break;
                } else {
                    inserted = true;
                    break;
                }
            }
            if (!inserted) {
                //std::cout << vrIndex[srcIt.first->second + 1] << std::endl;
                vcIndex.emplace(vcIndex.begin() + vrIndex[srcIt.first->second + 1], tgtIt.first);
                for (int i = srcIt.first->second + 1; i < vrIndex.size(); i++) {
                    vrIndex[i]++;
                }
            }
        }
    }

    /*
    //std::set<std::string> vertexIds;
    //std::vector<bool> result;
    //loop over all edges and add them one by one
    for (std::vector<std::pair<std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        //std::tuple<std::string, std::string, std::string> edge = *it;
        //vertexIds.emplace(std::get<0>(edge));
        //vertexIds.emplace(std::get<2>(edge));
        //check for the existence of the vertexId & neighbourVertexId
        std::pair< vertexMap_it, bool> srcIt = this->insertVertex(it->first);
        std::pair< vertexMap_it, bool> tgtIt = this->insertVertex(it->second);

        //set the neighbourVertexId as a neighbor to the vertexId with the edgeLabel 
        srcIt.first->second.vec.push_back(tgtIt.first);
    }
     */

    /*
    std::set<std::string> vertexIds;
    //std::vector<bool> result;
    //loop over all VertexNeighbourIds pairs and add them one by one
    for (std::vector<std::pair<std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::pair<std::string, std::string> neighbourPairIds = *it;

        std::pair< vertexMap_it, bool> srcIt = this->insertVertex(neighbourPairIds.first);
        std::pair< vertexMap_it, bool> tgtIt = this->insertVertex(neighbourPairIds.second);


        uint64_t sourceVertexIndex, targetVertexIndex;

        sourceVertexIndex = srcIt.first->second;
        targetVertexIndex = tgtIt.first->second;

        if (!this->vertexColumnIndex.at(sourceVertexIndex)) {
            this->vertexColumnIndex.at(sourceVertexIndex) = new std::list<uint64_t>;
        }

        uint64_t idx;
        std::list<uint64_t>::iterator list_it = this->vertexColumnIndex.at(sourceVertexIndex)->begin();

        for (idx = this->vertexRowIndex.at(sourceVertexIndex);
                idx < this->vertexRowIndex.at(sourceVertexIndex + 1) && *list_it < targetVertexIndex;
                idx++, list_it++) {
        }

        //col_it = this->vertexColumnIndex.begin();
        //std::advance(col_it, this->vertexRowIndex.at(sourceVertexIndex));

        if (idx == this->vertexRowIndex.at(sourceVertexIndex + 1) ||
     *list_it > targetVertexIndex) {
            this->vertexColumnIndex.at(sourceVertexIndex)->emplace(list_it, targetVertexIndex);

            for (uint64_t i = sourceVertexIndex + 1; i < this->vertexRowIndex.size(); i++) {
                this->vertexRowIndex.at(i) += 1;
            }
        }
    }
    



    std::set<std::string> vertexIds;
    //std::vector<bool> result;
    //loop over all VertexNeighbourIds pairs and add them one by one
    for (std::vector<std::pair<std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::pair<std::string, std::string> neighbourPairIds = *it;
        vertexIds.emplace(neighbourPairIds.first);
        vertexIds.emplace(neighbourPairIds.second);
    }

    this->insertVertex(vertexIds);

    for (std::vector<std::pair<std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::pair<std::string, std::string> neighbourPairIds = *it;
        this->addNeighbourVertex(neighbourPairIds.first, neighbourPairIds.second);
    }
    //return result;
     */

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
 */

uint64_t CSR::getCSRSize() {
    uint64_t size = 0;
    for (labeledCSR_it it = this->labeledCSR.begin(); it != this->labeledCSR.end(); it++) {
        size += vertexIndexMap(std::get<0>(it->second)).size();
    }
    return size;
}
