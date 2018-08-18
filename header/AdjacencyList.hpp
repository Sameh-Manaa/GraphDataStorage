/* 
 * File:   AdjacencyList.hpp
 * Author: Sameh-Manaa
 *
 * Created on May 15, 2018, 09:18 PM
 */

#ifndef ADJACENCYLIST_HPP
#define ADJACENCYLIST_HPP

#include <vector>
#include <list>
#include <forward_list>
#include <tuple>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>
#include <algorithm>

class AdjacencyList {
private:

    struct MapItContainers;
    typedef std::map<std::string, MapItContainers >::iterator MyMap_it;
    typedef std::unordered_map<std::string, std::map<std::string, MapItContainers > >::iterator AL_it;

    struct MapItContainers {
    public:
        std::vector<MyMap_it> vec;
    };

    //vertexAdjacencyMap: map<[VERTEX_ID],map<[EDGE_LABEL],vector<[NEIGHBOUR_VERTEX_ID]> > >
    //std::map<std::string, std::map<std::string, std::vector<std::string> > > vertexAdjacencyMap;
    std::unordered_map<std::string, std::map<std::string, MapItContainers > > vertexAdjacencyMap;

public:
    std::pair < MyMap_it, bool> insertVertex(std::string vertexId);
    void insertVertex(std::set<std::string> &vertexIds);
    bool removeVertex(std::string vertexId);
    bool addNeighbourVertex(std::string vertexId, std::string edgeLabel, std::string neighbourVertexId);
    void addNeighbourVertex(std::vector<std::tuple<std::string, std::string, std::string> > &edges);
    bool removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId);
    bool removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId, std::string edgeLabel);
    std::unordered_map<std::string, MapItContainers > getNeighbourVertices(std::string vertexId);
    uint64_t getVertexIndexByVertexId(std::string vertexId);
    uint64_t getAdjacencyListSize();
    uint64_t getAdjacencyListSizeInBytes();
    void getTargetVertex(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getTargetVertexWithReplacement(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getAllEdges(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    AdjacencyList();
};


#endif /* ADJACENCYLIST_HPP */

