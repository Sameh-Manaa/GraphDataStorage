/* 
 * File:   EmergingSchema.hpp
 * Author: Sameh-Manaa
 *
 * Created on Jul 16, 2018, 02:45 PM
 */

#ifndef EMERGINGSCHEMA_HPP
#define EMERGINGSCHEMA_HPP

#include <vector>
#include <list>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <iostream>
#include <boost/functional/hash.hpp>
#include <Kmeans.hpp>

class EmergingSchema {
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
    std::map<std::string, std::vector<std::string> > vertexUniversalMap;
    //edgeUniversalTable: < <vs_id, vt_id>, <edge_lable , <property_value > > >
    std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > edgeUniversalMap;
    
    //vertexPropertyTable: <property_name, <table_id, property_order> >
    std::map<std::string, std::pair<uint64_t, uint64_t> > vertexPropertyEsOrder;
    //edgePropertyTable: <property_name, <table_id, property_order> >
    std::map<std::string, std::pair<uint64_t, uint64_t> > edgePropertyEsOrder;
    //vertexEmergingSchema: <table_id, <v_id, <property_value> > >
    std::map<uint64_t, std::map<std::string, std::vector<std::string> > > vertexEmergingSchema;
    //edgeEmergingSchema: <table_id, < <vs_id, vt_id>, <edge_lable , <property_value > > > >
    std::map<uint64_t, std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > > edgeEmergingSchema;

public:
    uint64_t addVertexProperty(std::string propertyName);
    uint64_t addEdgeProperty(std::string propertyName);
    std::map<std::string, uint64_t> getVertexPropertyOrder();
    std::map<std::string, uint64_t> getEdgePropertyOrder();

    void upsertVertex(std::string vertexId, std::vector<std::string> properties);
    void upsertVertex(std::map<std::string, std::vector<std::string> > &vertexUniversalMap);
    bool removeVertex(std::string vertexId);
    void upsertEdge(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::vector<std::string> properties);
    void upsertEdge(std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > &edgeUniversalMap);
    bool removeEdge(std::string sourceVertexId, std::string targetVertexId);
    std::string getVertexProperty(std::string vertexId, std::string propertyName);
    std::vector<std::string> getVertexAllProperties(std::string vertexId);
    std::list<std::string> getQualifiedVertices(std::vector<std::string> &selectiveProperties);
    std::string getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::string propertyName);
    std::vector<std::string> getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel);
    std::list<std::pair<std::string, std::string> > getQualifiedEdges(std::vector<std::string> &selectiveProperties);
    uint64_t getVertexEmergingSchemaSize();
    uint64_t getEdgeEmergingSchemaSize();
    void generateVerticesEmergingSchema();
    void generateEdgesEmergingSchema();
    void clearUniversalTable();
    EmergingSchema();
};


#endif /* EMERGINGSCHEMA_HPP */

