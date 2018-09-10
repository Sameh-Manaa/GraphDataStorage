#include"ParallelSchemaHashedTable.hpp"

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

void ParallelSchemaHashedTable::upsertVertex(std::vector< std::pair<std::string, std::unordered_map<std::string, char*> > > &vertexSchemaHashedMap) {
    std::lock_guard<std::shared_timed_mutex> lock(globalVSHTMutex);
    this->vertexSchemaHashedMap.insert(vertexSchemaHashedMap.begin(), vertexSchemaHashedMap.end());
}

void ParallelSchemaHashedTable::upsertEdge(std::vector < std::pair< std::string, std::unordered_map<std::string, char*> > > &edgeSchemaHashedMap) {
    std::lock_guard<std::shared_timed_mutex> lock(globalESHTMutex);
    this->edgeSchemaHashedMap.insert(edgeSchemaHashedMap.begin(), edgeSchemaHashedMap.end());
}

uint64_t ParallelSchemaHashedTable::getVertexSchemaHashedTableSize() {
    std::shared_lock<std::shared_timed_mutex> lock(globalVSHTMutex);
    return this->vertexSchemaHashedMap.size();
}

uint64_t ParallelSchemaHashedTable::getEdgeSchemaHashedTableSize() {
    std::shared_lock<std::shared_timed_mutex> lock(globalESHTMutex);
    return this->edgeSchemaHashedMap.size();
}