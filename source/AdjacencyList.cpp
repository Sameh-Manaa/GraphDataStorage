#include"AdjacencyList.hpp"

void AdjacencyList::addNeighbourVertex(std::string edgeLabel, std::map<std::string, std::set<std::string> > &edges) {

    if (edges.empty()) {
        return;
    }

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
        innerMap_it = innerMap_it->second.vec[0];
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