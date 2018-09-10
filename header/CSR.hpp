/* 
 * File:   CSR.hpp
 * Author: Sameh-Manaa
 *
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

class CSR {
private:
    typedef std::map<std::string, int32_t> vertexIndexMap;
    typedef vertexIndexMap::iterator vertexMap_it;
    typedef std::vector<uint32_t> vertexRowIndex;
    typedef std::vector<vertexMap_it> vertexColumnIndex;
    typedef std::tuple<vertexIndexMap, vertexRowIndex, vertexColumnIndex> CSR_tuple;
    typedef std::unordered_map<std::string, CSR_tuple> labeledCSR_map;
    typedef labeledCSR_map::iterator labeledCSR_it;
    
    labeledCSR_map labeledCSR;

public:
    void addNeighbourVertex(std::string edgeLabel, std::map<std::string, std::set<std::string> > &edges);
    uint64_t getCSRSize();
    uint64_t getCSRSizeInBytes();
    void getTargetVertex(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getTargetVertexWithReplacement(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getAllEdges(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
};


#endif /* CSR_HPP */

