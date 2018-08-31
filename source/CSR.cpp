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

    CSR_tuple &csrTuple = csrEntry.first->second;
    vertexIndexMap &viMap = std::get<0>(csrTuple);
    vertexRowIndex &vrIndex = std::get<1>(csrTuple);
    vertexColumnIndex &vcIndex = std::get<2>(csrTuple);

    if (csrEntry.second) {
        vrIndex.emplace_back(0);
    }

    uint32_t maxIndex = vrIndex.size() - 1;


    std::string sourceVertexId = vertexId;
    std::string targetVertexId = neighbourVertexId;

    std::pair<vertexMap_it, bool> tgtIt = viMap.insert(std::make_pair(targetVertexId, -1));
    vertexMap_it src = viMap.find(sourceVertexId);
    std::pair<vertexMap_it, bool> srcIt;
    if (src == viMap.end()) {
        srcIt = viMap.insert(std::make_pair(sourceVertexId, maxIndex));
        maxIndex++;
    } else if (src->second == -1) {
        src->second = maxIndex;
        maxIndex++;
        srcIt.first = src;
        srcIt.second = true;
    } else {
        srcIt.first = src;
        srcIt.second = false;
    }

    if (srcIt.second) {
        vrIndex.emplace_back(vrIndex.back() + 1);
        vcIndex.emplace_back(tgtIt.first);
    } else {
        bool inserted = false;
        for (uint32_t i = vrIndex.at(srcIt.first->second); i < vrIndex.at(srcIt.first->second + 1); i++) {
            if (vcIndex.at(i)->first < tgtIt.first->first) {
                continue;
            } else if (vcIndex.at(i)->first > tgtIt.first->first) {
                vcIndex.emplace(vcIndex.begin() + i, tgtIt.first);
                for (int i = srcIt.first->second + 1; i < vrIndex.size(); i++) {
                    vrIndex[i]++;
                }
                //vrIndex[srcIt.first->second + 1]++;
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

    return true;
}

void CSR::addNeighbourVertex(std::vector<std::tuple<std::string, std::string, std::string> > &edges) {

    if (edges.empty()) {
        return;
    }

    std::string& edgeLabel = std::get<1>(edges.front());

    std::pair<labeledCSR_it, bool> csrEntry = this->labeledCSR.emplace(edgeLabel, CSR_tuple());

    CSR_tuple &csrTuple = csrEntry.first->second;
    vertexIndexMap &viMap = std::get<0>(csrTuple);
    vertexRowIndex &vrIndex = std::get<1>(csrTuple);
    vertexColumnIndex &vcIndex = std::get<2>(csrTuple);

    if (vrIndex.capacity() < vrIndex.size() + edges.size()) {
        vrIndex.reserve(vrIndex.size() + edges.size());
    }

    if (vcIndex.capacity() < vcIndex.size() + edges.size()) {
        vcIndex.reserve(vcIndex.size() + edges.size());
    }

    if (csrEntry.second) {
        vrIndex.emplace_back(0);
    }

    uint32_t maxIndex = vrIndex.size() - 1;

    for (auto &edge : edges) {

        std::string& sourceVertexId = std::get<0>(edge);
        std::string& targetVertexId = std::get<2>(edge);

        std::pair<vertexMap_it, bool> tgtIt = viMap.insert(std::make_pair(targetVertexId, -1));
        vertexMap_it src = viMap.find(sourceVertexId);
        std::pair<vertexMap_it, bool> srcIt;
        if (src == viMap.end()) {
            srcIt = viMap.insert(std::make_pair(sourceVertexId, maxIndex));
            maxIndex++;
        } else if (src->second == -1) {
            src->second = maxIndex;
            maxIndex++;
            srcIt.first = src;
            srcIt.second = true;
        } else {
            srcIt.first = src;
            srcIt.second = false;
        }

        if (srcIt.second) {
            vrIndex.emplace_back(vrIndex.back() + 1);
            vcIndex.emplace_back(tgtIt.first);
        } else {
            bool inserted = false;
            for (uint32_t i = vrIndex.at(srcIt.first->second); i < vrIndex.at(srcIt.first->second + 1); i++) {
                if (vcIndex.at(i)->first < tgtIt.first->first) {
                    continue;
                } else if (vcIndex.at(i)->first > tgtIt.first->first) {
                    vcIndex.emplace(vcIndex.begin() + i, tgtIt.first);
                    for (int i = srcIt.first->second + 1; i < vrIndex.size(); i++) {
                        vrIndex[i]++;
                    }
                    //vrIndex[srcIt.first->second + 1]++;
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
}

void CSR::addNeighbourVertex(std::string edgeLabel, std::map<std::string, std::set<std::string> > &edges) {

    if (edges.empty()) {
        return;
    }

    //std::string& edgeLabel = std::get<1>(edges.front());

    std::pair<labeledCSR_it, bool> csrEntry = this->labeledCSR.emplace(edgeLabel, CSR_tuple());

    CSR_tuple &csrTuple = csrEntry.first->second;
    vertexIndexMap &viMap = std::get<0>(csrTuple);
    vertexRowIndex &vrIndex = std::get<1>(csrTuple);
    vertexColumnIndex &vcIndex = std::get<2>(csrTuple);

    /*
    if (vrIndex.capacity() < vrIndex.size() + edges.size()) {
        vrIndex.reserve(vrIndex.size() + edges.size());
    }

    if (vcIndex.capacity() < vcIndex.size() + edges.size()) {
        vcIndex.reserve(vcIndex.size() + edges.size());
    }
     */

    if (csrEntry.second) {
        vrIndex.emplace_back(0);
    }

    uint32_t maxIndex = vrIndex.size() - 1;

    for (auto &edge : edges) {

        std::string sourceVertexId = edge.first;

        vertexMap_it src = viMap.find(sourceVertexId);
        std::pair<vertexMap_it, bool> srcIt;
        if (src == viMap.end()) {
            srcIt = viMap.insert(std::make_pair(sourceVertexId, maxIndex));
            maxIndex++;
        } else if (src->second == -1) {
            src->second = maxIndex;
            maxIndex++;
            srcIt.first = src;
            srcIt.second = true;
        } else {
            srcIt.first = src;
            srcIt.second = false;
        }



        int insertions = 0;

        if (srcIt.second) {
            vrIndex.emplace_back(vrIndex.back());
            //vcIndex.emplace_back(tgtIt.first);
            vrIndex[srcIt.first->second + 1] += edge.second.size();
            //insertions++;
            srcIt.second = false;
        } else {
            for (int k = srcIt.first->second + 1; k < vrIndex.size(); k++) {
                vrIndex[k] += edge.second.size();
            }
        }

        uint32_t i = vrIndex.at(srcIt.first->second);

        for (auto& targetVertexId : edge.second) {

            std::pair<vertexMap_it, bool> tgtIt = viMap.insert(std::make_pair(targetVertexId, -1));

            bool inserted = false;
            for (; i < (vrIndex.at(srcIt.first->second + 1) - edge.second.size() + insertions); i++) {
                if (vcIndex.at(i)->first < targetVertexId) {
                    continue;
                } else if (vcIndex.at(i)->first > targetVertexId) {
                    vcIndex.emplace(vcIndex.begin() + i, tgtIt.first);
                    insertions++;
                    //vrIndex[srcIt.first->second + 1]++;
                    inserted = true;
                    break;
                } else {
                    for (int i = srcIt.first->second + 1; i < vrIndex.size(); i++) {
                        vrIndex[i]--;
                    }
                    inserted = true;
                    break;
                }
            }
            if (!inserted) {
                //std::cout << vrIndex[srcIt.first->second + 1] << std::endl;
                vcIndex.emplace(vcIndex.begin() + vrIndex[srcIt.first->second + 1] - edge.second.size() + insertions, tgtIt.first);
                insertions++;
                i++;
            }
        }
    }
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

uint64_t CSR::getCSRSizeInBytes() {
    uint64_t size = sizeof (this->labeledCSR);
    for (auto const& labeledEdge : this->labeledCSR) {
        size += sizeof (labeledEdge.first);
        size += labeledEdge.first.length() + 1;
        size += sizeof (labeledEdge.second);
        size += sizeof (vertexIndexMap);
        size += sizeof (vertexRowIndex);
        size += sizeof (vertexColumnIndex);
        for (auto const& vertexIndex : vertexIndexMap(std::get<0>(labeledEdge.second))) {
            size += sizeof (vertexIndex.first);
            size += vertexIndex.first.length() + 1;
            size += sizeof (vertexIndex.second);
        }
        for (auto const& vrIndex : vertexRowIndex(std::get<1>(labeledEdge.second))) {
            size += sizeof (vrIndex);
        }
        for (auto const& vcIndex : vertexColumnIndex(std::get<2>(labeledEdge.second))) {
            size += sizeof (vcIndex);
        }
    }

    return size * sizeof (char);
}

void CSR::getTargetVertex(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    labeledCSR_it labeledEdgesSet_it = this->labeledCSR.find(edgeLabel);
    vertexIndexMap& vi_map = std::get<0>(labeledEdgesSet_it->second);
    vertexRowIndex& vr_vec = std::get<1>(labeledEdgesSet_it->second);
    vertexColumnIndex& vc_vec = std::get<2>(labeledEdgesSet_it->second);


    for (uint32_t i = 0; i < resultSet.size(); i++) {
        std::string sourceVertexId = resultSet[i].first[0];
        vertexMap_it sourceVertexIndex = vi_map.find(sourceVertexId);
        while (sourceVertexIndex->second != -1) {
            sourceVertexIndex = vc_vec.at(vr_vec.at(sourceVertexIndex->second));
        }
        resultSet[i].first.emplace_back(sourceVertexIndex->first);
    }
}

void CSR::getTargetVertexWithReplacement(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    labeledCSR_it labeledEdgesSet_it = this->labeledCSR.find(edgeLabel);
    vertexIndexMap& vi_map = std::get<0>(labeledEdgesSet_it->second);
    vertexRowIndex& vr_vec = std::get<1>(labeledEdgesSet_it->second);
    vertexColumnIndex& vc_vec = std::get<2>(labeledEdgesSet_it->second);


    for (uint32_t i = 0; i < resultSet.size(); i++) {
        std::string sourceVertexId = resultSet[i].first[0];
        vertexMap_it sourceVertexIndex = vi_map.find(sourceVertexId);
        while (sourceVertexIndex->second != -1) {
            sourceVertexIndex = vc_vec.at(vr_vec.at(sourceVertexIndex->second));
        }
        resultSet[i].first[0] = sourceVertexIndex->first;
    }
}

void CSR::getAllEdges(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    for (auto const &labeledCSREntry : this->labeledCSR) {
        vertexIndexMap const &viMap = vertexIndexMap(std::get<0>(labeledCSREntry.second));
        vertexRowIndex const &vrIndex = vertexRowIndex(std::get<1>(labeledCSREntry.second));
        vertexColumnIndex const &vcIndex = vertexColumnIndex(std::get<2>(labeledCSREntry.second));
        for (auto const &sourceVertex : viMap) {
            if (sourceVertex.second != -1) {
                for (uint32_t i = vrIndex.at(sourceVertex.second); i < vrIndex.at(sourceVertex.second + 1); i++) {
                    resultSet.emplace_back(std::make_pair(std::vector<std::string>() = {"out", labeledCSREntry.first, sourceVertex.first}
                    , std::vector<double>() = {1}));
                    resultSet.emplace_back(std::make_pair(std::vector<std::string>() = {"in", labeledCSREntry.first, vcIndex.at(i)->first}
                    , std::vector<double>() = {1}));
                }
            }
        }
    }
}