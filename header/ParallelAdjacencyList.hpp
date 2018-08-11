/* 
 * File:   ParallelAdjacencyList.hpp
 * Author: Sameh-Manaa
 *
 * Created on Jul 25, 2018, 02:18 PM
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
    typedef std::map<std::string, std::pair<std::shared_ptr<std::shared_timed_mutex>, std::map<std::string, MapItContainers > > >::iterator AL_it;

    struct MapItContainers {
    public:
        std::shared_ptr<std::shared_timed_mutex> vecMutex = std::make_shared<std::shared_timed_mutex>();
        std::vector<MyMap_it> vec;
        ~MapItContainers(){
            //vecMutex.reset();
        }
    };
    
    std::shared_timed_mutex globalAlMutex;
    //vertexAdjacencyMap: map<[VERTEX_ID],map<[EDGE_LABEL],vector<[NEIGHBOUR_VERTEX_ID]> > >
    //std::map<std::string, std::pair<std::shared_ptr<std::shared_timed_mutex>, std::map<std::string, std::vector<std::string> > > > vertexAdjacencyMap;
    std::map<std::string, std::pair<std::shared_ptr<std::shared_timed_mutex>, std::map<std::string, MapItContainers > > > vertexAdjacencyMap;

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
    ParallelAdjacencyList();
    ~ParallelAdjacencyList();
};


#endif /* PARALLELADJACENCYLIST_HPP */

