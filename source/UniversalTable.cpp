/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"UniversalTable.hpp"
#include<iostream>

UniversalTable::UniversalTable() {

}

bool UniversalTable::addVertexProperty(std::string propertyName) {
    if (this->vertexPropertyOrder.find(propertyName) != this->vertexPropertyOrder.end()) {
        return false;
    } else {
        this->vertexPropertyOrder[propertyName] = this->vertexPropertyOrder.size();
        for (auto& vertex : this->vertexUniversalMap) {
            vertex.second.push_back("");
        }
        return true;
    }
}

bool UniversalTable::addEdgeProperty(std::string propertyName) {
    if (this->edgePropertyOrder.find(propertyName) != this->edgePropertyOrder.end()) {
        return false;
    } else {
        this->edgePropertyOrder[propertyName] = this->edgePropertyOrder.size();
        for (auto& edge : this->edgeUniversalMap) {
            edge.second.push_back("");
        }
        return true;
    }
}

std::map<std::string, uint64_t> UniversalTable::getVertexPropertyOrder(){
    return this->vertexPropertyOrder;
}

std::map<std::string, uint64_t> UniversalTable::getEdgePropertyOrder(){
    return this->edgePropertyOrder;
}

bool UniversalTable::upsertVertex(std::string vertexId, std::vector<std::string> properties) {
    this->vertexUniversalMap[vertexId] = properties;
    return true;
}

bool UniversalTable::upsertVertex(std::map<std::string, std::vector<std::string> > vertexUniversalMap) {
    vertexUniversalMap.insert(this->vertexUniversalMap.begin(), this->vertexUniversalMap.end());
    this->vertexUniversalMap.swap(vertexUniversalMap);
    return true;
}

bool UniversalTable::removeVertex(std::string vertexId) {
    std::size_t countRemoved = this->vertexUniversalMap.erase(vertexId);
    if (countRemoved == 1) {
        return true;
    } else {
        return false;
    }
}

bool UniversalTable::upsertEdge(std::string sourceVertexId, std::string targetVertexId, std::vector<std::string> properties) {
    this->edgeUniversalMap[std::make_pair(sourceVertexId, targetVertexId)] = properties;
    return true;
}

bool UniversalTable::upsertEdge(std::map<std::pair<std::string, std::string>, std::vector<std::string> > edgeUniversalMap) {
    edgeUniversalMap.insert(this->edgeUniversalMap.begin(), this->edgeUniversalMap.end());
    this->edgeUniversalMap.swap(edgeUniversalMap);
    return true;
}

bool UniversalTable::removeEdge(std::string sourceVertexId, std::string targetVertexId) {
    std::size_t countRemoved = this->edgeUniversalMap.erase(std::make_pair(sourceVertexId, targetVertexId));
    if (countRemoved == 1) {
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

std::string UniversalTable::getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string propertyName) {
    return this->edgeUniversalMap.at(std::make_pair(sourceVertexId, targetVertexId)).at(this->edgePropertyOrder.at(propertyName));
}

std::vector<std::string> UniversalTable::getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId) {
    return this->edgeUniversalMap.at(std::make_pair(sourceVertexId, targetVertexId));
}

std::list<std::pair<std::string, std::string> > UniversalTable::getQualifiedEdges(std::vector<std::string> selectiveProperties) {
    std::list<std::pair<std::string, std::string> > qualifiedEdges(0);
    for (auto const& edge : this->edgeUniversalMap) {
        for (uint64_t i = 0; i < this->edgePropertyOrder.size(); i++) {
            if (selectiveProperties[i].empty()) {
                continue;
            } else if (selectiveProperties[i].compare(edge.second[i]) == 0) {
                qualifiedEdges.push_back(edge.first);
            } else {
                break;
            }
        }
    }
    return qualifiedEdges;
}