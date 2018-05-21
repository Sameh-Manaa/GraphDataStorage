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

class UniversalTable {
private:
    //vertexPropertyOrder: <property_name, property_order>
    std::map<std::string, uint64_t> vertexPropertyOrder;
    //edgePropertyOrder: <property_name, property_order>
    std::map<std::string, uint64_t> edgePropertyOrder;
    //vertexUniversalTable: <v_id, <property_value> >
    std::map<std::string, std::vector<std::string> > vertexUniversalMap; 
    //edgeUniversalTable: < <vs_id, vt_id>, <property_value> >
    std::map<std::pair<std::string,std::string>, std::vector<std::string> > edgeUniversalMap; 

public:
    bool addVertexProperty(std::string propertyName);
    bool addEdgeProperty(std::string propertyName);
    std::map<std::string, uint64_t> getVertexPropertyOrder();
    std::map<std::string, uint64_t> getEdgePropertyOrder();
    
    bool upsertVertex(std::string vertexId, std::vector<std::string> properties);
    bool upsertVertex(std::map<std::string, std::vector<std::string> > vertexUniversalMap);
    bool removeVertex(std::string vertexId);
    bool upsertEdge(std::string sourceVertexId, std::string targetVertexId, std::vector<std::string> properties);
    bool upsertEdge(std::map<std::pair<std::string,std::string>, std::vector<std::string> > edgeUniversalMap);
    bool removeEdge(std::string sourceVertexId, std::string targetVertexId);
    std::string getVertexProperty(std::string vertexId, std::string propertyName);
    std::vector<std::string> getVertexAllProperties(std::string vertexId);
    std::list<std::string> getQualifiedVertices(std::vector<std::string> selectiveProperties);
    std::string getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string propertyName);
    std::vector<std::string> getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId);
    std::list<std::pair<std::string,std::string> > getQualifiedEdges(std::vector<std::string> selectiveProperties);
    UniversalTable();
};


#endif /* UNIVERSALTABLE_HPP */

