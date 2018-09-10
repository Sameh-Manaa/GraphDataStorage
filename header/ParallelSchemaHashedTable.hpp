/* 
 * File:   ParallelSchemaHashedTable.hpp
 * Author: Sameh-Manaa
 *
 */

#ifndef PARALLELSCHEMAHASHEDTABLE_HPP
#define PARALLELSCHEMAHASHEDTABLE_HPP

#include <vector>
#include <list>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <iostream>
#include <cstring>
#include <shared_mutex>

class ParallelSchemaHashedTable {
private:
    std::shared_timed_mutex globalVSHTMutex, globalESHTMutex;
    std::map<std::string, std::unordered_map<std::string, char*> > vertexSchemaHashedMap;
    std::map<std::string, std::unordered_map<std::string, char*> > edgeSchemaHashedMap;

public:

    void upsertVertex(std::vector< std::pair<std::string, std::unordered_map<std::string, char*> > > &vertexSchemaHashedMap);
    void upsertEdge(std::vector < std::pair< std::string, std::unordered_map<std::string, char*> > > &edgeSchemaHashedMap);
    uint64_t getVertexSchemaHashedTableSize();
    uint64_t getEdgeSchemaHashedTableSize();
    
    ~ParallelSchemaHashedTable();
};


#endif /* PARALLELSCHEMAHASHEDTABLE_HPP */

