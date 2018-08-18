/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"SchemaHashedTable.hpp"

SchemaHashedTable::SchemaHashedTable() {
    //this->edgeSchemaHashedMap.max_load_factor(10);
}

SchemaHashedTable::~SchemaHashedTable() {
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

void SchemaHashedTable::upsertVertex(std::string vertexId, std::unordered_map<std::string, char*> properties) {
    this->vertexSchemaHashedMap[vertexId] = properties;
}

void SchemaHashedTable::upsertVertex(std::map<std::string, std::unordered_map<std::string, char*> > &vertexSchemaHashedMap) {
    /*if (this->vertexSchemaHashedMap.bucket_count() - this->vertexSchemaHashedMap.size() <
            vertexSchemaHashedMap.size()*2) {
        this->vertexSchemaHashedMap.reserve(this->vertexSchemaHashedMap.size() + (vertexSchemaHashedMap.size()*4));
    }*/
    this->vertexSchemaHashedMap.insert(vertexSchemaHashedMap.begin(), vertexSchemaHashedMap.end());
}

void SchemaHashedTable::upsertVertex(std::vector< std::pair< std::string, std::unordered_map<std::string, char*> > > &vertexSchemaHashedMap) {
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

void SchemaHashedTable::upsertEdge(std::string sourceVertexId, std::string edgeLabel, std::string targetVertexId, std::unordered_map<std::string, char*> properties) {
    this->edgeSchemaHashedMap[sourceVertexId + "$" + targetVertexId + "$" + edgeLabel] = properties;
}

void SchemaHashedTable::upsertEdge(std::map<std::string, std::unordered_map<std::string, char*> > &edgeSchemaHashedMap) {
    /*if (this->edgeSchemaHashedMap.bucket_count() - this->edgeSchemaHashedMap.size() <
            edgeSchemaHashedMap.size()*2) {
        this->edgeSchemaHashedMap.reserve(this->edgeSchemaHashedMap.size() + (edgeSchemaHashedMap.size()*4));
    }*/
    this->edgeSchemaHashedMap.insert(edgeSchemaHashedMap.begin(), edgeSchemaHashedMap.end());
}

void SchemaHashedTable::upsertEdge(std::vector < std::pair< std::string, std::unordered_map<std::string, char*> > > &edgeSchemaHashedMap) {
    /*if (this->edgeSchemaHashedMap.bucket_count() - this->edgeSchemaHashedMap.size() <
            edgeSchemaHashedMap.size()*2) {
        this->edgeSchemaHashedMap.reserve(this->edgeSchemaHashedMap.size() + (edgeSchemaHashedMap.size()*4));
    }*/
    this->edgeSchemaHashedMap.insert(edgeSchemaHashedMap.begin(), edgeSchemaHashedMap.end());
}

/*
bool SchemaHashedTable::removeEdge(std::string sourceVertexId, std::string targetVertexId) {
    if (this->edgeSchemaHashedMap.erase(std::make_pair(sourceVertexId, targetVertexId)) == 1) {
        return true;
    } else {
        return false;
    }
}
 */

std::string SchemaHashedTable::getVertexProperty(std::string vertexId, std::string propertyName) {
    return this->vertexSchemaHashedMap.at(vertexId).at(propertyName);
}

std::unordered_map<std::string, char*> SchemaHashedTable::getVertexAllProperties(std::string vertexId) {
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
    return this->edgeSchemaHashedMap.at(sourceVertexId + "$" + targetVertexId + "$" + edgeLabel).at(propertyName);
}

std::unordered_map<std::string, char*> SchemaHashedTable::getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel) {
    return this->edgeSchemaHashedMap.at(sourceVertexId + "$" + targetVertexId + "$" + edgeLabel);
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

uint64_t SchemaHashedTable::getVertexSchemaHashedTableSizeInBytes() {
    uint64_t size = sizeof (this->vertexSchemaHashedMap);
    for (auto const& vertex : this->vertexSchemaHashedMap) {
        size += sizeof (vertex.first);
        size += vertex.first.length() + 1;
        size += sizeof (vertex.second);
        for (auto const& vertexProperty : vertex.second) {
            size += sizeof (vertexProperty.first);
            size += vertexProperty.first.length() + 1;
            size += sizeof (vertexProperty.second);
            if (vertexProperty.second)
                size += strlen(vertexProperty.second);
        }
    }

    return size * sizeof (char);
}

uint64_t SchemaHashedTable::getEdgeSchemaHashedTableSizeInBytes() {
    uint64_t size = sizeof (this->edgeSchemaHashedMap);
    for (auto const& edge : this->edgeSchemaHashedMap) {
        size += sizeof (edge.first);
        size += edge.first.length() + 1;
        size += sizeof (edge.second);
        for (auto const& edgeProperty : edge.second) {
            size += sizeof (edgeProperty.first);
            size += edgeProperty.first.length() + 1;
            size += sizeof (edgeProperty.second);
            if (edgeProperty.second)
                size += strlen(edgeProperty.second);
        }
    }

    return size * sizeof (char);
}

uint64_t SchemaHashedTable::getSchemaHashedTableSizeInBytes() {
    uint64_t size = this->getVertexSchemaHashedTableSizeInBytes() + this->getEdgeSchemaHashedTableSizeInBytes();
    return size;
}

std::pair<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator, std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator>
SchemaHashedTable::getVertices(std::string vertexType) {
    std::string vertexTypeInc = vertexType;
    char lastVertexChar = vertexType.back();
    lastVertexChar++;
    vertexTypeInc[vertexTypeInc.size() - 1] = lastVertexChar;

    std::pair<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator, std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator> result;
    result.first = this->vertexSchemaHashedMap.lower_bound(vertexType);
    result.second = this->vertexSchemaHashedMap.upper_bound(vertexTypeInc);
    return result;
}

std::vector<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator>
SchemaHashedTable::getVertices(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    std::vector<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator> returnList;
    for (uint32_t i = 0; i < resultSet.size(); i++) {
        returnList.emplace_back(this->vertexSchemaHashedMap.find(resultSet[i].first.back()));
    }
    return returnList;
}