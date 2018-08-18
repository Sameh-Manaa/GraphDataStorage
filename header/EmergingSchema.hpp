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

#include "UniversalTable.hpp"

class EmergingSchema {
private:
    
    //vertexPropertyTable: <property_name, <table_id, property_order> >
    std::unordered_map<std::string, std::pair<uint16_t, uint16_t> > vertexPropertyEsIndex;
    //edgePropertyTable: <property_name, <table_id, property_order> >
    std::unordered_map<std::string, std::pair<uint16_t, uint16_t> > edgePropertyEsIndex;
    //vertexEmergingSchema: <table_id, <v_id, <property_value> > >
    std::unordered_map<uint16_t, std::map<std::string, std::vector<char*> > > vertexEmergingSchema;
    //edgeEmergingSchema: <table_id, < <vs_id, vt_id>, <edge_lable , <property_value > > > >
    std::unordered_map<uint16_t, std::map<std::string, std::vector<char*> > > edgeEmergingSchema;

public:
    
    std::unordered_map<std::string, std::pair<uint16_t, uint16_t> > getVertexPropertyIndex();
    std::unordered_map<std::string, std::pair<uint16_t, uint16_t> > getEdgePropertyIndex();
    
    uint64_t getVertexEmergingSchemaSize();
    uint64_t getEdgeEmergingSchemaSize();
    uint64_t getVertexEmergingSchemaSizeInBytes();
    uint64_t getEdgeEmergingSchemaSizeInBytes();
    uint64_t getEmergingSchemaSizeInBytes();
    void generateVerticesEmergingSchema(UniversalTable &universalTable);
    void generateEdgesEmergingSchema(UniversalTable &universalTable);

    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator>
    getVertices(std::string vertexType, std::string propertyName);
    
    std::vector<std::map<std::string, std::vector<char*> >::const_iterator>
    getVertices(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet, std::string propertyName);
    
    EmergingSchema();
    ~EmergingSchema();
};


#endif /* EMERGINGSCHEMA_HPP */