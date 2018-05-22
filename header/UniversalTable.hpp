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
#include <stdint.h>
#include <map>
#include <unordered_map>
#include<iostream>
#include <boost/functional/hash.hpp>

class UniversalTable {
private:

struct hash_Pair
{
    std::size_t operator() ( const std::pair<std::string, std::string>& m ) const
    {
        std::size_t s = 0 ;
        boost::hash_combine( s, m.first ) ;
        boost::hash_combine( s, m.second ) ;
        return s ;
    }
};
    
    
    //vertexPropertyOrder: <property_name, property_order>
    std::map<std::string, uint64_t> vertexPropertyOrder;
    //edgePropertyOrder: <property_name, property_order>
    std::map<std::string, uint64_t> edgePropertyOrder;
    //vertexUniversalTable: <v_id, <property_value> >
    std::unordered_map<std::string, std::vector<std::string> > vertexUniversalMap;
    //edgeUniversalTable: < <vs_id, vt_id>, <edge_lable , <property_value > > >
    std::unordered_map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > , hash_Pair> edgeUniversalMap;

public:
    uint64_t addVertexProperty(std::string propertyName);
    uint64_t addEdgeProperty(std::string propertyName);
    std::map<std::string, uint64_t> getVertexPropertyOrder();
    std::map<std::string, uint64_t> getEdgePropertyOrder();

    bool upsertVertex(std::string vertexId, std::vector<std::string> properties);
    bool upsertVertex(std::map<std::string, std::vector<std::string> > vertexUniversalMap);
    bool removeVertex(std::string vertexId);
    bool upsertEdge(std::string sourceVertexId, std::string targetVertexId, std::string edgeLable, std::vector<std::string> properties);
    bool upsertEdge(std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > edgeUniversalMap);
    bool removeEdge(std::string sourceVertexId, std::string targetVertexId);
    std::string getVertexProperty(std::string vertexId, std::string propertyName);
    std::vector<std::string> getVertexAllProperties(std::string vertexId);
    std::list<std::string> getQualifiedVertices(std::vector<std::string> selectiveProperties);
    std::string getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string edgeLable, std::string propertyName);
    std::vector<std::string> getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLable);
    std::list<std::pair<std::string, std::string> > getQualifiedEdges(std::vector<std::string> selectiveProperties);
    uint64_t getVertexUniversalTableSize();
    uint64_t getEdgeUniversalTableSize();
    UniversalTable();
};


#endif /* UNIVERSALTABLE_HPP */

