/* 
 * File:   ParallelSchemaHashedTable.hpp
 * Author: Sameh-Manaa
 *
 * Created on Jul 26, 2018, 11:55 AM
 */

#ifndef PARALLELSCHEMAHASHEDTABLE_HPP
#define PARALLELSCHEMAHASHEDTABLE_HPP

#include <vector>
#include <list>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <iostream>
#include <boost/functional/hash.hpp>
#include <shared_mutex>

class ParallelSchemaHashedTable {
private:
    std::shared_timed_mutex globalVSHTMutex, globalESHTMutex;
    //vertexUniversalTable: MAP<VERTEX_ID, map<[PROPERTY_NAME],[PROPERTY_VALUE]> >
    std::map<std::string, std::unordered_map<std::string, char*> > vertexSchemaHashedMap;
    //edgeUniversalTable: map< <vs_id, vt_id>, map<[EDGE_LABEL] , map<[PROPERTY_NAME],[PROPERTY_VALUE]> > >
    std::map<std::string, std::unordered_map<std::string, char*> > edgeSchemaHashedMap;

public:

    void upsertVertex(std::string vertexId, std::unordered_map<std::string, char*> properties);
    void upsertVertex(std::map<std::string,  std::unordered_map<std::string, char*> > &vertexSchemaHashedMap);
    void upsertVertex(std::vector< std::pair<std::string, std::unordered_map<std::string, char*> > > &vertexSchemaHashedMap);
    bool removeVertex(std::string vertexId);
    void upsertEdge(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::unordered_map<std::string, char*> properties);
    void upsertEdge(std::map<std::string, std::unordered_map<std::string, char*> > &edgeSchemaHashedMap);
    void upsertEdge(std::vector < std::pair< std::string, std::unordered_map<std::string, char*> > > &edgeSchemaHashedMap);
    bool removeEdge(std::string sourceVertexId, std::string targetVertexId);
    std::string getVertexProperty(std::string vertexId, std::string propertyName);
    std::unordered_map<std::string, char*> getVertexAllProperties(std::string vertexId);
    std::list<std::string> getQualifiedVertices(std::vector<std::string> &selectiveProperties);
    std::string getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::string propertyName);
    std::unordered_map<std::string, char*> getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel);
    std::list<std::pair<std::string, std::string> > getQualifiedEdges(std::vector<std::string> &selectiveProperties);
    uint64_t getVertexSchemaHashedTableSize();
    uint64_t getEdgeSchemaHashedTableSize();
    ParallelSchemaHashedTable();
    
    ~ParallelSchemaHashedTable();
};


#endif /* PARALLELSCHEMAHASHEDTABLE_HPP */

