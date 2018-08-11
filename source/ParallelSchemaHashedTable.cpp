/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"ParallelSchemaHashedTable.hpp"

ParallelSchemaHashedTable::ParallelSchemaHashedTable() {
    //this->edgeSchemaHashedMap.max_load_factor(10);
}

ParallelSchemaHashedTable::~ParallelSchemaHashedTable(){
    for (auto const& vertex : this->vertexSchemaHashedMap) {
        for (auto const& vertexProperty : vertex.second) {
            delete[] vertexProperty.second;
        }
    }
    
    
    for (auto const& edge : this->edgeSchemaHashedMap) {
        for (auto const& edgeProperty : edge.second) {
            delete[] edgeProperty.second;
        }
    }
    
    this->vertexSchemaHashedMap.clear();
    this->edgeSchemaHashedMap.clear();
}

void ParallelSchemaHashedTable::upsertVertex(std::string vertexId, std::unordered_map<std::string, char*> properties) {
    std::lock_guard<std::shared_timed_mutex> lock(globalVSHTMutex);
    this->vertexSchemaHashedMap[vertexId] = properties;
}

void ParallelSchemaHashedTable::upsertVertex(std::map<std::string, std::unordered_map<std::string, char*> > &vertexSchemaHashedMap) {
    std::lock_guard<std::shared_timed_mutex> lock(globalVSHTMutex);
    /*if (this->vertexSchemaHashedMap.bucket_count() - this->vertexSchemaHashedMap.size() <
            vertexSchemaHashedMap.size()*2) {
        this->vertexSchemaHashedMap.reserve(this->vertexSchemaHashedMap.size() + (vertexSchemaHashedMap.size()*4));
    }*/
    this->vertexSchemaHashedMap.insert(vertexSchemaHashedMap.begin(), vertexSchemaHashedMap.end());
}

void ParallelSchemaHashedTable::upsertVertex(std::vector< std::pair<std::string, std::unordered_map<std::string, char*> > > &vertexSchemaHashedMap) {
    std::lock_guard<std::shared_timed_mutex> lock(globalVSHTMutex);
    /*if (this->vertexSchemaHashedMap.bucket_count() - this->vertexSchemaHashedMap.size() <
            vertexSchemaHashedMap.size()*2) {
        this->vertexSchemaHashedMap.reserve(this->vertexSchemaHashedMap.size() + (vertexSchemaHashedMap.size()*4));
    }*/
    this->vertexSchemaHashedMap.insert(vertexSchemaHashedMap.begin(), vertexSchemaHashedMap.end());
}

bool ParallelSchemaHashedTable::removeVertex(std::string vertexId) {
    if (this->vertexSchemaHashedMap.erase(vertexId) == 1) {
        return true;
    } else {
        return false;
    }
}

void ParallelSchemaHashedTable::upsertEdge(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::unordered_map<std::string, char*> properties) {
    std::lock_guard<std::shared_timed_mutex> lock(globalESHTMutex);
    this->edgeSchemaHashedMap[sourceVertexId + "$" + targetVertexId + "$" + edgeLabel] = properties;
}

void ParallelSchemaHashedTable::upsertEdge(std::map<std::string, std::unordered_map<std::string, char*> > &edgeSchemaHashedMap) {
    std::lock_guard<std::shared_timed_mutex> lock(globalESHTMutex);
    /*if (this->edgeSchemaHashedMap.bucket_count() - this->edgeSchemaHashedMap.size() <
            edgeSchemaHashedMap.size()*2) {
        this->edgeSchemaHashedMap.reserve(this->edgeSchemaHashedMap.size() + (edgeSchemaHashedMap.size()*4));
    }*/
    this->edgeSchemaHashedMap.insert(edgeSchemaHashedMap.begin(), edgeSchemaHashedMap.end());
}

void ParallelSchemaHashedTable::upsertEdge(std::vector < std::pair< std::string, std::unordered_map<std::string, char*> > > &edgeSchemaHashedMap) {
    std::lock_guard<std::shared_timed_mutex> lock(globalESHTMutex);
    /*if (this->edgeSchemaHashedMap.bucket_count() - this->edgeSchemaHashedMap.size() <
            edgeSchemaHashedMap.size()*2) {
        this->edgeSchemaHashedMap.reserve(this->edgeSchemaHashedMap.size() + (edgeSchemaHashedMap.size()*4));
    }*/
    this->edgeSchemaHashedMap.insert(edgeSchemaHashedMap.begin(), edgeSchemaHashedMap.end());
}
/*
bool ParallelSchemaHashedTable::removeEdge(std::string sourceVertexId, std::string targetVertexId) {
    if (this->edgeSchemaHashedMap.erase(std::make_pair(sourceVertexId, targetVertexId)) == 1) {
        return true;
    } else {
        return false;
    }
}
*/

std::string ParallelSchemaHashedTable::getVertexProperty(std::string vertexId, std::string propertyName) {
    return this->vertexSchemaHashedMap.at(vertexId).at(propertyName);
}

std::unordered_map<std::string, char*> ParallelSchemaHashedTable::getVertexAllProperties(std::string vertexId) {
    return this->vertexSchemaHashedMap.at(vertexId);
}

/*
std::list<std::string> ParallelSchemaHashedTable::getQualifiedVertices(std::vector<std::string> selectiveProperties) {
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

std::string ParallelSchemaHashedTable::getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::string propertyName) {
    return this->edgeSchemaHashedMap.at(sourceVertexId + "$" + targetVertexId + "$" + edgeLabel).at(propertyName);
}

std::unordered_map<std::string, char*> ParallelSchemaHashedTable::getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel) {
    return this->edgeSchemaHashedMap.at(sourceVertexId + "$" + targetVertexId + "$" + edgeLabel);
}

/*
std::list<std::pair<std::string, std::string> > ParallelSchemaHashedTable::getQualifiedEdges(std::vector<std::string> selectiveProperties) {
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

uint64_t ParallelSchemaHashedTable::getVertexSchemaHashedTableSize() {
    std::shared_lock<std::shared_timed_mutex> lock(globalVSHTMutex);
    return this->vertexSchemaHashedMap.size();
}

uint64_t ParallelSchemaHashedTable::getEdgeSchemaHashedTableSize() {
    std::shared_lock<std::shared_timed_mutex> lock(globalESHTMutex);
    return this->edgeSchemaHashedMap.size();
}