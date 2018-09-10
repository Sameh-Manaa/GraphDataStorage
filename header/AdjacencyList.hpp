/* 
 * File:   AdjacencyList.hpp
 * Author: Sameh-Manaa
 *
 */

#ifndef ADJACENCYLIST_HPP
#define ADJACENCYLIST_HPP

#include <vector>
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

    std::unordered_map<std::string, std::map<std::string, MapItContainers > > vertexAdjacencyMap;

public:
    void addNeighbourVertex(std::string edgeLabel, std::map<std::string, std::set<std::string> > &edges);
    uint64_t getAdjacencyListSize();
    uint64_t getAdjacencyListSizeInBytes();
    void getTargetVertex(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getTargetVertexWithReplacement(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getAllEdges(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
};


#endif /* ADJACENCYLIST_HPP */

