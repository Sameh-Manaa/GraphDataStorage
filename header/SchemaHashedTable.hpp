/* 
 * File:   UniversalTable.hpp
 * Author: Sameh-Manaa
 *
 * Created on Jun 20, 2018, 01:30 PM
 */

#ifndef SCHEMAHASHEDTABLE_HPP
#define SCHEMAHASHEDTABLE_HPP

#include <vector>
#include <list>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include<iostream>
#include <boost/functional/hash.hpp>

class SchemaHashedTable {
private:

    struct hash_Pair {

        std::size_t operator()(const std::pair<std::string, std::string>& m) const {
            std::size_t s = 0;
            boost::hash_combine(s, m.first);
            boost::hash_combine(s, m.second);
            return s;
        }
    };


    //vertexUniversalTable: MAP<VERTEX_ID, map<[PROPERTY_NAME],[PROPERTY_VALUE]> >
    std::map<std::string, std::map<std::string, std::string> > vertexSchemaHashedMap;
    //edgeUniversalTable: map< <vs_id, vt_id>, map<[EDGE_LABEL] , map<[PROPERTY_NAME],[PROPERTY_VALUE]> > >
    std::map<std::pair<std::string, std::string>, std::map<std::string, std::map<std::string, std::string> > > edgeSchemaHashedMap;

public:

    void upsertVertex(std::string vertexId, std::map<std::string, std::string> properties);
    void upsertVertex(std::map<std::string,  std::map<std::string, std::string> > &vertexSchemaHashedMap);
    bool removeVertex(std::string vertexId);
    void upsertEdge(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::map<std::string, std::string> properties);
    void upsertEdge(std::map<std::pair<std::string, std::string>, std::map<std::string, std::map<std::string, std::string> > > &edgeSchemaHashedMap);
    bool removeEdge(std::string sourceVertexId, std::string targetVertexId);
    std::string getVertexProperty(std::string vertexId, std::string propertyName);
    std::map<std::string, std::string> getVertexAllProperties(std::string vertexId);
    std::list<std::string> getQualifiedVertices(std::vector<std::string> &selectiveProperties);
    std::string getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::string propertyName);
    std::map<std::string, std::string> getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel);
    std::list<std::pair<std::string, std::string> > getQualifiedEdges(std::vector<std::string> &selectiveProperties);
    uint64_t getVertexSchemaHashedTableSize();
    uint64_t getEdgeSchemaHashedTableSize();
    SchemaHashedTable();
};


#endif /* SCHEMAHASHEDTABLE_HPP */

