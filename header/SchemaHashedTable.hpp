/* 
 * File:   UniversalTable.hpp
 * Author: Sameh-Manaa
 *
 */

#ifndef SCHEMAHASHEDTABLE_HPP
#define SCHEMAHASHEDTABLE_HPP

#include <vector>
#include <list>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <cstring>
#include <iostream>

class SchemaHashedTable {
private:

    std::map<std::string, std::unordered_map<std::string, char*> > vertexSchemaHashedMap;
    std::map<std::string, std::unordered_map<std::string, char*> > edgeSchemaHashedMap;

public:

    void upsertVertex(std::vector< std::pair< std::string, std::unordered_map<std::string, char*> > > &vertexSchemaHashedMap);
    void upsertEdge(std::vector< std::pair< std::string, std::unordered_map<std::string, char*> > > &edgeSchemaHashedMap);
    uint64_t getVertexSchemaHashedTableSize();
    uint64_t getEdgeSchemaHashedTableSize();
    uint64_t getVertexSchemaHashedTableSizeInBytes();
    uint64_t getEdgeSchemaHashedTableSizeInBytes();
    uint64_t getSchemaHashedTableSizeInBytes();
    
    std::pair<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator, std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator>
    getVertices(std::string vertexType);
    
    std::vector<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator>
    getVertices(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    
    ~SchemaHashedTable();
};


#endif /* SCHEMAHASHEDTABLE_HPP */

