/* 
 * File:   AdjacencyMatrix.hpp
 * Author: Sameh-Manaa
 *
 * Created on May 9, 2018, 12:45 PM
 */

#ifndef ADJACENCYMATRIX_HPP
#define ADJACENCYMATRIX_HPP

#include <vector>
#include <list>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>
#include <boost/dynamic_bitset.hpp>

class AdjacencyMatrix {
private:
    typedef std::map<std::string, uint32_t> vertexIndex_map;
    typedef vertexIndex_map::iterator vertexIndex_it;
    //vertexAdjacencyMap: map<[VERTEX_ID],vector<[NEIGHBOUR_VERTEX_FLAG]> >
    typedef std::vector< std::vector<bool> > vertexAdjacency_map;
    typedef vertexAdjacency_map::iterator vertexAdjacency_it;
    //vertexIndexMap: map<[VERTEX_ID],[VERTEX_INDEX]>  
    //Index of a vertex in the adjacency matrix (i.e. rowIndex & columnIndex)

    typedef std::unordered_map<std::string, std::pair<vertexIndex_map, vertexAdjacency_map> > labeledAdjMat_map;
    typedef labeledAdjMat_map::iterator labeledAdjMat_it;

    labeledAdjMat_map labeledAdjacencyMatrix;

    int batchSize;

    void resize(std::string edgeLabel);

public:
    std::pair<vertexIndex_it, bool> insertVertex(std::string vertexId);
    void insertVertex(std::set<std::string> &vertexIds);
    bool removeVeretex(std::string vertexId);
    bool addNeighbourVertex(std::string vertexId, std::string edgeLabel, std::string neighbourVertexId);
    void addNeighbourVertex(std::vector<std::tuple<std::string, std::string, std::string> > &edges);
    bool removeNeighbourVertex(std::string vertexId, std::string neighbourVertexId);
    std::list<std::string> getNeighbourVertices(std::string vertexId);
    uint64_t getVertexIndexByVertexId(std::string vertexId);
    std::string getVertexIdByVertexIndex(uint64_t vertexIndex, std::string edgeLabel);
    uint64_t getAdjacencyMatrixSize();
    void getTargetVertex(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getTargetVertexWithReplacement(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getAllEdges(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void shrinkToFit();
    void setBatchSize(int batchSize);

    AdjacencyMatrix() {
    };
};


#endif /* ADJACENCYMATRIX_HPP */

