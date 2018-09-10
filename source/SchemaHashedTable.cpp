#include"SchemaHashedTable.hpp"

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

void SchemaHashedTable::upsertVertex(std::vector< std::pair< std::string, std::unordered_map<std::string, char*> > > &vertexSchemaHashedMap) {
    this->vertexSchemaHashedMap.insert(vertexSchemaHashedMap.begin(), vertexSchemaHashedMap.end());
}

void SchemaHashedTable::upsertEdge(std::vector < std::pair< std::string, std::unordered_map<std::string, char*> > > &edgeSchemaHashedMap) {
    this->edgeSchemaHashedMap.insert(edgeSchemaHashedMap.begin(), edgeSchemaHashedMap.end());
}

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