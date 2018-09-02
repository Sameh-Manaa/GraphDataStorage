/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"AdjacencyList.hpp"

AdjacencyList::AdjacencyList() {

}
/*
std::pair < AdjacencyList::MyMap_it, bool> AdjacencyList::insertVertex(std::string vertexId) {
    //insert the new vertexId into the vertexAdjacencyMap
    std::unordered_map<std::string, MapItContainers > labeledNeighbourVertexMap;
    std::pair < MyMap_it, bool> insertResult =
            vertexAdjacencyMap.insert(std::make_pair(vertexId, labeledNeighbourVertexMap));
    return insertResult;
}

void AdjacencyList::insertVertex(std::set<std::string> &vertexIds) {
    std::unordered_map<std::string, MapItContainers > labeledNeighbourVertexMap;
    //loop over the vertexIds, and insert them one by one
    for (std::set<std::string>::iterator it = vertexIds.begin(); it != vertexIds.end(); ++it) {
        this->vertexAdjacencyMap.emplace(*it, labeledNeighbourVertexMap);
    }
}
 */

/*
bool AdjacencyList::removeVertex(std::string vertexId) {
    //check for the existence of the vertexId to be removed
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {

        //remove the vertex from the neighborhood of any other vertex
        for (auto& vertexToLabeledNeighbourVertexMap : vertexAdjacencyMap) {
            for (auto& labelToNeighbourVertices : vertexToLabeledNeighbourVertexMap.second) {
                std::vector<std::string*>::iterator it = std::find(labelToNeighbourVertices.second.begin(), labelToNeighbourVertices.second.end(), vertexId);
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

bool AdjacencyList::addNeighbourVertex(std::string vertexId, std::string edgeLabel, std::string neighbourVertexId) {
    //check for the existence of the vertexId & neighbourVertexId
    std::pair< AL_it, bool> alEntry = this->vertexAdjacencyMap.emplace(edgeLabel, std::map<std::string, MapItContainers >());
    std::pair < MyMap_it, bool> srcIt =
            alEntry.first->second.emplace(vertexId, MapItContainers());
    std::pair < MyMap_it, bool> tgtIt =
            alEntry.first->second.emplace(neighbourVertexId, MapItContainers());

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

void AdjacencyList::addNeighbourVertex(std::vector<std::tuple<std::string, std::string, std::string> > &edges) {

    if (edges.empty()) {
        return;
    }

    std::string edgeLabel = std::get<1>(edges.front());

    std::pair< AL_it, bool> alEntry = this->vertexAdjacencyMap.emplace(edgeLabel, std::map<std::string, MapItContainers >());

    //loop over all edges and add them one by one
    for (std::vector<std::tuple<std::string, std::string, std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::pair < MyMap_it, bool> srcIt =
                alEntry.first->second.emplace(std::get<0>(*it), MapItContainers());
        std::pair < MyMap_it, bool> tgtIt =
                alEntry.first->second.emplace(std::get<2>(*it), MapItContainers());

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
    }
}

void AdjacencyList::addNeighbourVertex(std::string edgeLabel, std::map<std::string, std::set<std::string> > &edges) {

    if (edges.empty()) {
        return;
    }

    //std::string edgeLabel = std::get<1>(edges.front());

    std::pair< AL_it, bool> alEntry = this->vertexAdjacencyMap.emplace(edgeLabel, std::map<std::string, MapItContainers >());

    //loop over all edges and add them one by one
    for (std::map<std::string, std::set<std::string> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::pair < MyMap_it, bool> srcIt =
                alEntry.first->second.emplace(it->first, MapItContainers());
        int i = 0;
        for (std::set<std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            std::pair < MyMap_it, bool> tgtIt =
                    alEntry.first->second.emplace(*it2, MapItContainers());

            //set the neighbourVertexId as a neighbor to the vertexId with the edgeLabel 
            bool inserted = false;
            for (; i < srcIt.first->second.vec.size(); i++) {
                if (srcIt.first->second.vec.at(i)->first < *it2) {
                    continue;
                } else if (srcIt.first->second.vec.at(i)->first > *it2) {
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
                i++;
            }
        }
    }
}


/*
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
 */

/*
std::unordered_map<std::string, AdjacencyList::MapItContainers > AdjacencyList::getNeighbourVertices(std::string vertexId) {
    if (vertexAdjacencyMap.find(vertexId) != vertexAdjacencyMap.end()) {
        return vertexAdjacencyMap.at(vertexId);
    } else {
        return {};
    }
}
 */

uint64_t AdjacencyList::getVertexIndexByVertexId(std::string vertexId) {
    return std::distance(vertexAdjacencyMap.begin(), vertexAdjacencyMap.find(vertexId));
}

uint64_t AdjacencyList::getAdjacencyListSize() {
    uint64_t size = 0;
    for (AL_it it = this->vertexAdjacencyMap.begin(); it != this->vertexAdjacencyMap.end(); it++) {
        size += it->second.size();
    }
    return size;
}

uint64_t AdjacencyList::getAdjacencyListSizeInBytes() {
    uint64_t size = sizeof (this->vertexAdjacencyMap);
    for (auto const& labeledEdge : this->vertexAdjacencyMap) {
        size += sizeof (labeledEdge.first);
        size += labeledEdge.first.length() + 1;
        size += sizeof (labeledEdge.second);
        for (auto const& edge : labeledEdge.second) {
            size += sizeof (edge.first);
            size += edge.first.length() + 1;
            size += sizeof (edge.second);
            size += sizeof (edge.second.vec);
            for (auto const& it : edge.second.vec) {
                size += sizeof (it);
            }
        }
    }

    return size * sizeof (char);
}

void AdjacencyList::getTargetVertex(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    AL_it labeledEdgesSet_it = this->vertexAdjacencyMap.find(edgeLabel);

    for (uint32_t i = 0; i < resultSet.size(); i++) {
        std::string sourceVertexId = resultSet[i].first[0];
        MyMap_it innerMap_it = labeledEdgesSet_it->second.find(sourceVertexId);
        while (!innerMap_it->second.vec.empty()) {
            innerMap_it = innerMap_it->second.vec[0];
        }
        resultSet[i].first.emplace_back(innerMap_it->first);
    }
}

void AdjacencyList::getTargetVertexWithReplacement(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    AL_it labeledEdgesSet_it = this->vertexAdjacencyMap.find(edgeLabel);

    for (uint32_t i = 0; i < resultSet.size(); i++) {
        std::string sourceVertexId = resultSet[i].first[0];
        MyMap_it innerMap_it = labeledEdgesSet_it->second.find(sourceVertexId);
        //while (!innerMap_it->second.vec.empty()) {
        innerMap_it = innerMap_it->second.vec[0];
        //}
        resultSet[i].first[0] = innerMap_it->first;
    }
}

void AdjacencyList::getAllEdges(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    for (auto const &labeledEdges : this->vertexAdjacencyMap) {
        for (auto const &vertexOutEdges : labeledEdges.second) {
            for (auto const &targetVertex : vertexOutEdges.second.vec) {
                resultSet.emplace_back(std::make_pair(std::vector<std::string>() = {"out", labeledEdges.first, vertexOutEdges.first}
                , std::vector<double>() = {1}));
                resultSet.emplace_back(std::make_pair(std::vector<std::string>() = {"in", labeledEdges.first, targetVertex->first}
                , std::vector<double>() = {1}));
            }
        }
    }
}