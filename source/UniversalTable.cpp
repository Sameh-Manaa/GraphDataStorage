#include"UniversalTable.hpp"

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
            edge.second.push_back(nullptr);
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

void UniversalTable::upsertVertex(std::vector < std::pair < std::string, std::vector<char*> > > &vertexUniversalMap) {
    this->vertexUniversalMap.insert(vertexUniversalMap.begin(), vertexUniversalMap.end());
}

void UniversalTable::upsertEdge(std::vector < std::pair < std::string, std::vector<char*> > > &edgeUniversalMap) {
    this->edgeUniversalMap.insert(edgeUniversalMap.begin(), edgeUniversalMap.end());
}

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