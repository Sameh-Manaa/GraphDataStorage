/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"UniversalTable.hpp"

UniversalTable::UniversalTable() {
    //this->edgeUniversalMap.max_load_factor(10);
}

uint64_t UniversalTable::addVertexProperty(std::string propertyName) {
    bool inserted = this->vertexPropertyOrder.insert(std::make_pair(propertyName, this->vertexPropertyOrder.size())).second;

    if (inserted) {
        for (auto& vertex : this->vertexUniversalMap) {
            vertex.second.push_back("");
        }
    }
    return this->vertexPropertyOrder.at(propertyName);
}

uint64_t UniversalTable::addEdgeProperty(std::string propertyName) {
    bool inserted = this->edgePropertyOrder.insert(std::make_pair(propertyName, this->edgePropertyOrder.size())).second;
    if (inserted) {
        for (auto& edge : this->edgeUniversalMap) {
            for (auto& labledEdge : edge.second) {
                labledEdge.second.push_back("");
            }
        }
    }
    return this->edgePropertyOrder.at(propertyName);
}

std::unordered_map<std::string, uint64_t> UniversalTable::getVertexPropertyOrder() {
    return this->vertexPropertyOrder;
}

std::unordered_map<std::string, uint64_t> UniversalTable::getEdgePropertyOrder() {
    return this->edgePropertyOrder;
}

void UniversalTable::upsertVertex(std::string vertexId, std::vector<std::string> properties) {
    this->vertexUniversalMap[vertexId] = properties;
}

void UniversalTable::upsertVertex(std::unordered_map<std::string, std::vector<std::string> > vertexUniversalMap) {
    this->vertexUniversalMap.reserve(this->vertexUniversalMap.size() + vertexUniversalMap.size());
    this->vertexUniversalMap.insert(vertexUniversalMap.begin(), vertexUniversalMap.end());
}

bool UniversalTable::removeVertex(std::string vertexId) {
    if (this->vertexUniversalMap.erase(vertexId) == 1) {
        return true;
    } else {
        return false;
    }
}

void UniversalTable::upsertEdge(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::vector<std::string> properties) {
    this->edgeUniversalMap[std::make_pair(sourceVertexId, targetVertexId)][edgeLabel] = properties;
}

void UniversalTable::upsertEdge(std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > edgeUniversalMap) {
    this->edgeUniversalMap.reserve(this->edgeUniversalMap.size() + edgeUniversalMap.size());
    this->edgeUniversalMap.insert(edgeUniversalMap.begin(), edgeUniversalMap.end());
}

bool UniversalTable::removeEdge(std::string sourceVertexId, std::string targetVertexId) {
    if (this->edgeUniversalMap.erase(std::make_pair(sourceVertexId, targetVertexId)) == 1) {
        return true;
    } else {
        return false;
    }
}

std::string UniversalTable::getVertexProperty(std::string vertexId, std::string propertyName) {
    return this->vertexUniversalMap.at(vertexId).at(this->vertexPropertyOrder.at(propertyName));
}

std::vector<std::string> UniversalTable::getVertexAllProperties(std::string vertexId) {
    return this->vertexUniversalMap.at(vertexId);
}

std::list<std::string> UniversalTable::getQualifiedVertices(std::vector<std::string> selectiveProperties) {
    std::list<std::string> qualifiedVertices(0);
    for (auto const& vertex : this->vertexUniversalMap) {
        for (uint64_t i = 0; i < this->vertexPropertyOrder.size(); i++) {
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

std::string UniversalTable::getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::string propertyName) {
    return this->edgeUniversalMap.at(std::make_pair(sourceVertexId, targetVertexId)).at(edgeLabel).at(this->edgePropertyOrder.at(propertyName));
}

std::vector<std::string> UniversalTable::getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel) {
    return this->edgeUniversalMap.at(std::make_pair(sourceVertexId, targetVertexId)).at(edgeLabel);
}

std::list<std::pair<std::string, std::string> > UniversalTable::getQualifiedEdges(std::vector<std::string> selectiveProperties) {
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

uint64_t UniversalTable::getVertexUniversalTableSize() {
    return this->vertexUniversalMap.size();
}

uint64_t UniversalTable::getEdgeUniversalTableSize() {
    return this->edgeUniversalMap.size();
}