/* 
 * File:   UniversalTable.hpp
 * Author: Sameh-Manaa
 *
 */

#ifndef UNIVERSALTABLE_HPP
#define UNIVERSALTABLE_HPP

#include <vector>
#include <list>
#include <forward_list>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <cstring>
#include <iostream>

class UniversalTable {
private:
    friend class EmergingSchema;

    std::unordered_map<std::string, uint16_t> vertexPropertyIndex;
    std::unordered_map<std::string, uint16_t> edgePropertyIndex;
    std::map<std::string, std::vector<char*> > vertexUniversalMap;
    std::map<std::string, std::vector<char*> > edgeUniversalMap;

public:
    uint16_t addVertexProperty(std::string propertyName);
    uint16_t addEdgeProperty(std::string propertyName);
    std::unordered_map<std::string, uint16_t> getVertexPropertyIndex();
    std::unordered_map<std::string, uint16_t> getEdgePropertyIndex();

    void upsertVertex(std::vector < std::pair < std::string, std::vector<char*> > > &vertexUniversalMap);
    void upsertEdge(std::vector < std::pair < std::string, std::vector<char*> > > &edgeUniversalMap);
    uint64_t getVertexUniversalTableSize();
    uint64_t getEdgeUniversalTableSize();
    uint64_t getVertexUniversalTableSizeInBytes();
    uint64_t getUniversalTableSizeInBytes();
    uint64_t getEdgeUniversalTableSizeInBytes();
    void clearUniversalTable();

    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator>
    getVertices(std::string vertexType);

    std::vector<std::map<std::string, std::vector<char*> >::const_iterator>
    getVertices(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);

    ~UniversalTable();
};


#endif /* UNIVERSALTABLE_HPP */

