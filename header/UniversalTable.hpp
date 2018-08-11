/* 
 * File:   UniversalTable.hpp
 * Author: Sameh-Manaa
 *
 * Created on May 9, 2018, 12:45 PM
 */

#ifndef UNIVERSALTABLE_HPP
#define UNIVERSALTABLE_HPP

#include <vector>
#include <list>
#include <forward_list>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <iostream>
#include <boost/functional/hash.hpp>

class UniversalTable {
private:
    friend class EmergingSchema;

    struct cmpByStringValue {

        bool operator()(const char* a, const char* b) const {
            std::string a1, b1;
            a1.assign(a);
            b1.assign(b);
            return a1 < b1;
        }
    };


    //vertexPropertyOrder: <property_name, property_order>
    std::unordered_map<std::string, uint16_t> vertexPropertyIndex;
    //edgePropertyOrder: <property_name, property_order>
    std::unordered_map<std::string, uint16_t> edgePropertyIndex;
    //vertexUniversalTable: <v_id, <property_value> >
    std::map<std::string, std::vector<char*> > vertexUniversalMap;
    //edgeUniversalTable: < <vs_id, vt_id>, <edge_lable , <property_value > > >
    //std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > edgeUniversalMap;
    std::map<std::string, std::vector<char*> > edgeUniversalMap;

public:
    uint16_t addVertexProperty(std::string propertyName);
    uint16_t addEdgeProperty(std::string propertyName);
    std::unordered_map<std::string, uint16_t> getVertexPropertyIndex();
    std::unordered_map<std::string, uint16_t> getEdgePropertyIndex();

    void upsertVertex(std::string vertexId, std::vector<char*> properties);
    void upsertVertex(std::map<std::string, std::vector<char*> > &vertexUniversalMap);
    void upsertVertex(std::vector < std::pair < std::string, std::vector<char*> > > &vertexUniversalMap);
    bool removeVertex(std::string vertexId);
    void upsertEdge(std::string sourceVertexId, std::string edgeLabel, std::string targetVertexId, std::vector<char*> properties);
    void upsertEdge(std::map<std::string, std::vector<char*> > &edgeUniversalMap);
    void upsertEdge(std::vector < std::pair < std::string, std::vector<char*> > > &edgeUniversalMap);
    bool removeEdge(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel);
    std::string getVertexProperty(std::string vertexId, std::string propertyName);
    std::vector<char*> getVertexAllProperties(std::string vertexId);
    std::list<std::string> getQualifiedVertices(std::vector<std::string> &selectiveProperties);
    char* getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::string propertyName);
    std::vector<char*> getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel);
    std::list<std::pair<std::string, std::string> > getQualifiedEdges(std::vector<std::string> &selectiveProperties);
    uint64_t getVertexUniversalTableSize();
    uint64_t getEdgeUniversalTableSize();
    uint64_t getVertexUniversalTableSizeInBytes();
    uint64_t getEdgeUniversalTableSizeInBytes();
    void clearUniversalTable();

    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator>
    getVertices(std::string vertexType);

    UniversalTable();
    
    ~UniversalTable();
};


#endif /* UNIVERSALTABLE_HPP */

