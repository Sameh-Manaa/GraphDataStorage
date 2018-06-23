/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"SchemaHashedTable.hpp"

SchemaHashedTable::SchemaHashedTable() {
    //this->edgeSchemaHashedMap.max_load_factor(10);
}

void SchemaHashedTable::upsertVertex(std::string vertexId, std::map<std::string, std::string> properties) {
    this->vertexSchemaHashedMap[vertexId] = properties;
}

void SchemaHashedTable::upsertVertex(std::map<std::string, std::map<std::string, std::string> > &vertexSchemaHashedMap) {
    /*if (this->vertexSchemaHashedMap.bucket_count() - this->vertexSchemaHashedMap.size() <
            vertexSchemaHashedMap.size()*2) {
        this->vertexSchemaHashedMap.reserve(this->vertexSchemaHashedMap.size() + (vertexSchemaHashedMap.size()*4));
    }*/
    this->vertexSchemaHashedMap.insert(vertexSchemaHashedMap.begin(), vertexSchemaHashedMap.end());
}

bool SchemaHashedTable::removeVertex(std::string vertexId) {
    if (this->vertexSchemaHashedMap.erase(vertexId) == 1) {
        return true;
    } else {
        return false;
    }
}

void SchemaHashedTable::upsertEdge(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::map<std::string, std::string> properties) {
    this->edgeSchemaHashedMap[std::make_pair(sourceVertexId, targetVertexId)][edgeLabel] = properties;
}

void SchemaHashedTable::upsertEdge(std::map<std::pair<std::string, std::string>, std::map<std::string, std::map<std::string, std::string> > > &edgeSchemaHashedMap) {
    /*if (this->edgeSchemaHashedMap.bucket_count() - this->edgeSchemaHashedMap.size() <
            edgeSchemaHashedMap.size()*2) {
        this->edgeSchemaHashedMap.reserve(this->edgeSchemaHashedMap.size() + (edgeSchemaHashedMap.size()*4));
    }*/
    this->edgeSchemaHashedMap.insert(edgeSchemaHashedMap.begin(), edgeSchemaHashedMap.end());
}

bool SchemaHashedTable::removeEdge(std::string sourceVertexId, std::string targetVertexId) {
    if (this->edgeSchemaHashedMap.erase(std::make_pair(sourceVertexId, targetVertexId)) == 1) {
        return true;
    } else {
        return false;
    }
}

std::string SchemaHashedTable::getVertexProperty(std::string vertexId, std::string propertyName) {
    return this->vertexSchemaHashedMap.at(vertexId).at(propertyName);
}

std::map<std::string, std::string> SchemaHashedTable::getVertexAllProperties(std::string vertexId) {
    return this->vertexSchemaHashedMap.at(vertexId);
}

/*
std::list<std::string> SchemaHashedTable::getQualifiedVertices(std::vector<std::string> selectiveProperties) {
    std::list<std::string> qualifiedVertices(0);
    for (auto const& vertex : this->vertexSchemaHashedMap) {
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
}*/

std::string SchemaHashedTable::getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::string propertyName) {
    return this->edgeSchemaHashedMap.at(std::make_pair(sourceVertexId, targetVertexId)).at(edgeLabel).at(propertyName);
}

std::map<std::string, std::string> SchemaHashedTable::getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel) {
    return this->edgeSchemaHashedMap.at(std::make_pair(sourceVertexId, targetVertexId)).at(edgeLabel);
}

/*
std::list<std::pair<std::string, std::string> > SchemaHashedTable::getQualifiedEdges(std::vector<std::string> selectiveProperties) {
    std::list<std::pair<std::string, std::string> > qualifiedEdges(0);
    for (auto const& edge : this->edgeSchemaHashedMap) {
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
}*/

uint64_t SchemaHashedTable::getVertexSchemaHashedTableSize() {
    return this->vertexSchemaHashedMap.size();
}

uint64_t SchemaHashedTable::getEdgeSchemaHashedTableSize() {
    return this->edgeSchemaHashedMap.size();
}