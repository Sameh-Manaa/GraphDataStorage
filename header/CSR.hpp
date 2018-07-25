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
    //vertexRowIndex: vector<[ROW_INDEX]>
    std::vector<uint64_t> vertexRowIndex;
    //vertexColumnIndex: vector<[COLUMN_INDEX]>
    std::vector<uint64_t> vertexColumnIndex;
    //vertexIndexMap: map<[VERTEX_ID],[VERTEX_INDEX]>  
    //Index of a vertex in the CSR (i.e. rowIndex & columnIndex)
    std::map<std::string, uint64_t> vertexIndexMap;

public:
    bool insertVertex(std::string vertexId);
    void insertVertex(std::set<std::string> &vertexIds);
    bool removeVeretex(std::string vertexId);
    bool addNeighbourVertex(std::string vertexId, std::string neighbourVertexId);
    void addNeighbourVertex(std::vector<std::pair<std::string,std::string> > &edges);
    bool removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId);
    std::list<std::string> getNeighbourVertices(std::string vertexId);
    uint64_t getVertexIndexByVertexId(std::string vertexId);
    std::string getVertexIdByVertexIndex(uint64_t vertexIndex);
    uint64_t getCSRSize();
    CSR();
};


#endif /* CSR_HPP */

