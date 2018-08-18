/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"UniversalTable.hpp"

UniversalTable::UniversalTable() {
    //this->edgeUniversalMap.max_load_factor(10);
}

UniversalTable::~UniversalTable() {
    for (auto const& vertex : this->vertexUniversalMap) {
        for (char* vertexProperty : vertex.second) {
            delete[] vertexProperty;
        }
    }


    for (auto const& edge : this->edgeUniversalMap) {
        for (char* edgeProperty : edge.second) {
            delete[] edgeProperty;
        }
    }

    this->clearUniversalTable();
}

uint16_t UniversalTable::addVertexProperty(std::string propertyName) {
    std::pair<std::string, uint16_t> pVal = std::make_pair(propertyName, (uint16_t)this->vertexPropertyIndex.size());
    bool inserted = this->vertexPropertyIndex.insert(pVal).second;

    if (inserted) {
        for (auto& vertex : this->vertexUniversalMap) {
            vertex.second.push_back(nullptr);
        }
    }
    return this->vertexPropertyIndex.at(propertyName);
}

uint16_t UniversalTable::addEdgeProperty(std::string propertyName) {
    bool inserted = this->edgePropertyIndex.insert(std::make_pair(propertyName, this->edgePropertyIndex.size())).second;
    if (inserted) {
        for (auto& edge : this->edgeUniversalMap) {
            //for (auto& labledEdge : edge.second) {
            //labledEdge.second.push_back("");
            edge.second.push_back(nullptr);
            //}
        }
    }
    return this->edgePropertyIndex.at(propertyName);
}

std::unordered_map<std::string, uint16_t> UniversalTable::getVertexPropertyIndex() {
    return this->vertexPropertyIndex;
}

std::unordered_map<std::string, uint16_t> UniversalTable::getEdgePropertyIndex() {
    return this->edgePropertyIndex;
}

void UniversalTable::upsertVertex(std::string vertexId, std::vector<char*> properties) {
    this->vertexUniversalMap[vertexId] = properties;
}

void UniversalTable::upsertVertex(std::map<std::string, std::vector<char*> > &vertexUniversalMap) {
    //this->vertexUniversalMap.reserve(this->vertexUniversalMap.size() + vertexUniversalMap.size());
    this->vertexUniversalMap.insert(vertexUniversalMap.begin(), vertexUniversalMap.end());
}

void UniversalTable::upsertVertex(std::vector < std::pair < std::string, std::vector<char*> > > &vertexUniversalMap) {
    //this->vertexUniversalMap.reserve(this->vertexUniversalMap.size() + vertexUniversalMap.size());
    this->vertexUniversalMap.insert(vertexUniversalMap.begin(), vertexUniversalMap.end());
}

bool UniversalTable::removeVertex(std::string vertexId) {
    if (this->vertexUniversalMap.erase(vertexId) == 1) {
        return true;
    } else {
        return false;
    }
}

void UniversalTable::upsertEdge(std::string sourceVertexId, std::string edgeLabel, std::string targetVertexId, std::vector<char*> properties) {
    this->edgeUniversalMap[sourceVertexId + "$" + targetVertexId + "$" + edgeLabel] = properties;
}

void UniversalTable::upsertEdge(std::map<std::string, std::vector<char*> > &edgeUniversalMap) {
    //this->edgeUniversalMap.reserve(this->edgeUniversalMap.size() + edgeUniversalMap.size());
    this->edgeUniversalMap.insert(edgeUniversalMap.begin(), edgeUniversalMap.end());
}

void UniversalTable::upsertEdge(std::vector < std::pair < std::string, std::vector<char*> > > &edgeUniversalMap) {
    //this->edgeUniversalMap.reserve(this->edgeUniversalMap.size() + edgeUniversalMap.size());
    this->edgeUniversalMap.insert(edgeUniversalMap.begin(), edgeUniversalMap.end());
}

bool UniversalTable::removeEdge(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel) {
    if (this->edgeUniversalMap.erase(sourceVertexId + "$" + targetVertexId + "$" + edgeLabel) == 1) {
        return true;
    } else {
        return false;
    }
}

std::string UniversalTable::getVertexProperty(std::string vertexId, std::string propertyName) {
    return this->vertexUniversalMap.at(vertexId).at(this->vertexPropertyIndex.at(propertyName));
}

std::vector<char*> UniversalTable::getVertexAllProperties(std::string vertexId) {
    return this->vertexUniversalMap.at(vertexId);
}

std::list<std::string> UniversalTable::getQualifiedVertices(std::vector<std::string> &selectiveProperties) {
    std::list<std::string> qualifiedVertices(0);
    for (auto const& vertex : this->vertexUniversalMap) {
        for (uint64_t i = 0; i < this->vertexPropertyIndex.size(); i++) {
            if (selectiveProperties[i].empty()) {
                continue;
            } else if (selectiveProperties[i].compare(vertex.second[i]) == 0) {
                qualifiedVertices.push_back(vertex.first);
            } else {
                break;
            }
        }
    }
    return qualifiedVertices;
}

char* UniversalTable::getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::string propertyName) {
    return this->edgeUniversalMap.at(sourceVertexId + "$" + targetVertexId + "$" + edgeLabel).at(this->edgePropertyIndex.at(propertyName));
}

std::vector<char*> UniversalTable::getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel) {
    return this->edgeUniversalMap.at(sourceVertexId + "$" + targetVertexId + "$" + edgeLabel);
}

/*
std::list<std::pair<std::string, std::string> > UniversalTable::getQualifiedEdges(std::vector<std::string> &selectiveProperties) {
    std::list<std::pair<std::string, std::string> > qualifiedEdges(0);
    for (auto const& edge : this->edgeUniversalMap) {
        for (auto const& labledEdge : edge.second) {
            for (uint64_t i = 0; i < this->edgePropertyOrder.size(); i++) {
                if (selectiveProperties[i].empty()) {
                    continue;
                } else if (selectiveProperties[i].compare(labledEdge.second[i]) == 0) {
                    qualifiedEdges.push_back(edge.first);
                } else {
                    break;
                }
            }
        }
    }
    return qualifiedEdges;
}
 */

uint64_t UniversalTable::getVertexUniversalTableSize() {
    return this->vertexUniversalMap.size();
}

uint64_t UniversalTable::getEdgeUniversalTableSize() {
    return this->edgeUniversalMap.size();
}

uint64_t UniversalTable::getVertexUniversalTableSizeInBytes() {
    uint64_t size = sizeof (this->vertexUniversalMap);
    for (auto const& vertex : this->vertexUniversalMap) {
        size += sizeof (vertex.first);
        size += vertex.first.length() + 1;
        size += sizeof (vertex.second);
        for (char* const& vertexProperty : vertex.second) {
            size += sizeof (vertexProperty);
            if (vertexProperty)
                size += strlen(vertexProperty);
        }
    }

    size += sizeof (this->vertexPropertyIndex);
    for (auto const& vertexIndex : this->vertexPropertyIndex) {
        size += sizeof (vertexIndex.first);
        size += vertexIndex.first.length() + 1;
        size += sizeof (vertexIndex.second);
    }
    return size * sizeof (char);
}

uint64_t UniversalTable::getEdgeUniversalTableSizeInBytes() {
    uint64_t size = sizeof (this->edgeUniversalMap); //48 map
    for (auto const& edge : this->edgeUniversalMap) {
        size += sizeof (edge.first); //32 string
        size += edge.first.length() + 1;
        size += sizeof (edge.second); //24 vector
        for (char* const& edgeProperty : edge.second) {
            size += sizeof (edgeProperty);
            if (edgeProperty)
                size += strlen(edgeProperty);
        }
    }

    size += sizeof (this->edgePropertyIndex);
    for (auto const& edgeIndex : this->edgePropertyIndex) {
        size += sizeof (edgeIndex.first);
        size += edgeIndex.first.length() + 1;
        size += sizeof (edgeIndex.second);
    }
    return size * sizeof (char);
}

uint64_t UniversalTable::getUniversalTableSizeInBytes() {
    uint64_t size = this->getVertexUniversalTableSizeInBytes() + this->getEdgeUniversalTableSizeInBytes();
    return size;
}

void UniversalTable::clearUniversalTable() {
    this->vertexUniversalMap.clear();
    this->vertexPropertyIndex.clear();
    this->edgeUniversalMap.clear();
    this->edgePropertyIndex.clear();
}

std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator>
UniversalTable::getVertices(std::string vertexType) {
    std::string vertexTypeInc = vertexType;
    char lastVertexChar = vertexType.back();
    lastVertexChar++;
    vertexTypeInc[vertexTypeInc.size() - 1] = lastVertexChar;

    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> result;
    result.first = this->vertexUniversalMap.lower_bound(vertexType);
    result.second = this->vertexUniversalMap.upper_bound(vertexTypeInc);
    return result;
}

std::vector<std::map<std::string, std::vector<char*> >::const_iterator>
UniversalTable::getVertices(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    std::vector<std::map<std::string, std::vector<char*> >::const_iterator> returnList;
    for (uint32_t i = 0; i < resultSet.size(); i++) {
        returnList.emplace_back(this->vertexUniversalMap.find(resultSet[i].first.back()));
    }
    return returnList;
}