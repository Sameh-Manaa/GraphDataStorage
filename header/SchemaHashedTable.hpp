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

    //vertexUniversalTable: MAP<VERTEX_ID, map<[PROPERTY_NAME],[PROPERTY_VALUE]> >
    std::map<std::string, std::unordered_map<std::string, char*> > vertexSchemaHashedMap;
    //edgeUniversalTable: map< <vs_id, vt_id>, map<[EDGE_LABEL] , map<[PROPERTY_NAME],[PROPERTY_VALUE]> > >
    std::map<std::string, std::unordered_map<std::string, char*> > edgeSchemaHashedMap;

public:

    void upsertVertex(std::string vertexId, std::unordered_map<std::string, char*> properties);
    void upsertVertex(std::map<std::string,  std::unordered_map<std::string, char*> > &vertexSchemaHashedMap);
    void upsertVertex(std::vector< std::pair< std::string, std::unordered_map<std::string, char*> > > &vertexSchemaHashedMap);
    bool removeVertex(std::string vertexId);
    void upsertEdge(std::string sourceVertexId, std::string edgeLabel, std::string targetVertexId, std::unordered_map<std::string, char*> properties);
    void upsertEdge(std::map<std::string, std::unordered_map<std::string, char*> > &edgeSchemaHashedMap);
    void upsertEdge(std::vector< std::pair< std::string, std::unordered_map<std::string, char*> > > &edgeSchemaHashedMap);
    bool removeEdge(std::string sourceVertexId, std::string targetVertexId);
    std::string getVertexProperty(std::string vertexId, std::string propertyName);
    std::unordered_map<std::string, char*> getVertexAllProperties(std::string vertexId);
    std::list<std::string> getQualifiedVertices(std::vector<std::string> &selectiveProperties);
    std::string getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::string propertyName);
    std::unordered_map<std::string, char*> getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel);
    std::list<std::pair<std::string, std::string> > getQualifiedEdges(std::vector<std::string> &selectiveProperties);
    uint64_t getVertexSchemaHashedTableSize();
    uint64_t getEdgeSchemaHashedTableSize();
    
    std::pair<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator, std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator>
    getVertices(std::string vertexType);
    
    std::vector<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator>
    getVertices(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    
    SchemaHashedTable();
    
    ~SchemaHashedTable();
};


#endif /* SCHEMAHASHEDTABLE_HPP */

