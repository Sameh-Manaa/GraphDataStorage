/* 
 * File:   ParallelAdjacencyList.hpp
 * Author: Sameh-Manaa
 *
 */

#ifndef PARALLELADJACENCYLIST_HPP
#define PARALLELADJACENCYLIST_HPP

#include <vector>
#include <list>
#include <tuple>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>
#include <algorithm>
#include <shared_mutex>
#include <utility>

class ParallelAdjacencyList {
private:
    
    struct MapItContainers;
    typedef std::map<std::string, MapItContainers >::iterator MyMap_it;
    typedef std::unordered_map<std::string, std::pair<std::shared_ptr<std::shared_timed_mutex>, std::map<std::string, MapItContainers > > >::iterator AL_it;

    struct MapItContainers {
    public:
        std::shared_ptr<std::shared_timed_mutex> vecMutex = std::make_shared<std::shared_timed_mutex>();
        std::vector<MyMap_it> vec;
    };
    
    std::shared_timed_mutex globalAlMutex;
    std::unordered_map<std::string, std::pair<std::shared_ptr<std::shared_timed_mutex>, std::map<std::string, MapItContainers > > > vertexAdjacencyMap;

public:
    void addNeighbourVertex(std::string edgeLabel, std::map<std::string, std::set<std::string> > &edges);
    uint64_t getAdjacencyListSize();
};


#endif /* PARALLELADJACENCYLIST_HPP */

