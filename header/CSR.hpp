/* 
 * File:   CSR.hpp
 * Author: Sameh-Manaa
 *
 * Created on May 9, 2018, 12:45 PM
 */

#ifndef CSR_HPP
#define CSR_HPP

#include <vector>
#include <list>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>
#include <boost/dynamic_bitset.hpp>

class CSR {
private:
    /*
    struct MapItContainers;
    typedef std::map<std::string, MapItContainers>::iterator vertexMap_it;

    struct MapItContainers {
    public:
        std::vector<vertexMap_it> vec;
    };
     */

    typedef std::map<std::string, int32_t> vertexIndexMap;
    typedef vertexIndexMap::iterator vertexMap_it;
    typedef std::vector<uint32_t> vertexRowIndex;
    typedef std::vector<vertexMap_it> vertexColumnIndex;
    typedef std::tuple<vertexIndexMap, vertexRowIndex, vertexColumnIndex> CSR_tuple;
    typedef std::unordered_map<std::string, CSR_tuple> labeledCSR_map;
    typedef labeledCSR_map::iterator labeledCSR_it;

    //vertexRowIndex: vector<[ROW_INDEX]>
    //std::vector<uint64_t> vertexRowIndex;

    //vertexColumnIndex: vector<[COLUMN_INDEX]>
    //std::vector<std::list<uint64_t>* > vertexColumnIndex;
    //std::vector<uint64_t> vertexColumnIndex;

    //vertexIndexMap: map<[VERTEX_ID],[VERTEX_INDEX]>  
    //Index of a vertex in the CSR (i.e. rowIndex & columnIndex)
    //std::map<std::string, uint64_t> vertexIndexMap;
    
    //std::map<std::string, MapItContainers> vertexIndexMap;
    
    labeledCSR_map labeledCSR;

public:
    std::pair<vertexMap_it, bool> insertVertex(std::string vertexId);
    void insertVertex(std::set<std::string> &vertexIds);
    bool removeVeretex(std::string vertexId);
    bool addNeighbourVertex(std::string vertexId, std::string edgeLabel, std::string neighbourVertexId);
    void addNeighbourVertex(std::vector<std::tuple<std::string, std::string, std::string> > &edges);
    void addNeighbourVertex(std::string edgeLabel, std::map<std::string, std::set<std::string> > &edges);
    bool removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId);
    std::list<std::string> getNeighbourVertices(std::string vertexId);
    uint64_t getVertexIndexByVertexId(std::string vertexId);
    std::string getVertexIdByVertexIndex(uint64_t vertexIndex);
    uint64_t getCSRSize();
    uint64_t getCSRSizeInBytes();
    void getTargetVertex(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getTargetVertexWithReplacement(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getAllEdges(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    CSR();
    ~CSR();
};


#endif /* CSR_HPP */

