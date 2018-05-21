/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Test.cpp
 * Author: Sameh-Manaa
 *
 * Created on May 9, 2018, 11:44 AM
 */

#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include"AdjacencyMatrix.hpp"
#include"AdjacencyList.hpp"

/*
 * Simple C++ Test Suite
 */


void testInsertNode() {
    AdjacencyMatrix* adjacencyMatrix = new AdjacencyMatrix();
    adjacencyMatrix->insertVertex("0");
    adjacencyMatrix->insertVertex("1");
    adjacencyMatrix->insertVertex("2");
    adjacencyMatrix->insertVertex("3");
    adjacencyMatrix->insertVertex("4");

    uint64_t nodeIdPre = adjacencyMatrix->getVertexIndex("2");
    adjacencyMatrix->removeVeretex("1");
    uint64_t nodeIdPost = adjacencyMatrix->getVertexIndex("2");

    std::cout << "Adjacency Matrix: \r\nPre Node ID: " << nodeIdPre << ", Post Node ID: " << nodeIdPost << std::endl;

    adjacencyMatrix->addNeighbourVertex("0","4");
    adjacencyMatrix->addNeighbourVertex("0","3");
    adjacencyMatrix->addNeighbourVertex("0","2");
    
    std::cout<<adjacencyMatrix->getNeighbourVertices("0").back()<<std::endl;
    
    adjacencyMatrix->removeVeretex("4");
    std::cout<<adjacencyMatrix->getNeighbourVertices("0").back()<<std::endl;
    
    adjacencyMatrix->removeNeighbourVertex("0","3");
    std::cout<<adjacencyMatrix->getNeighbourVertices("0").back()<<std::endl;
    
    adjacencyMatrix->addNeighbourVertex("0","3");
    std::cout<<adjacencyMatrix->getNeighbourVertices("0").back()<<std::endl;
    
    adjacencyMatrix->removeNeighbourVertex("0","2");
    adjacencyMatrix->insertVertex("5");
    bool test = adjacencyMatrix->addNeighbourVertex("0","5");
    std::cout<<adjacencyMatrix->getNeighbourVertices("0").back()<<std::endl;
    
    
    /////////////////////////////////////////////////////

    AdjacencyList* adjacencyList = new AdjacencyList();
    adjacencyList->insertVertex("0");
    adjacencyList->insertVertex("1");
    adjacencyList->insertVertex("2");
    adjacencyList->insertVertex("3");
    adjacencyList->insertVertex("4");

    nodeIdPre = adjacencyList->getVertexIndex("2");
    adjacencyList->removeVertex("1");
    nodeIdPost = adjacencyList->getVertexIndex("2");
    
    std::cout << "Adjacency List: \r\nPre Node ID: " << nodeIdPre << ", Post Node ID: " << nodeIdPost << std::endl;
    
    adjacencyList->addNeighbourVertex("0","4","test");
    adjacencyList->addNeighbourVertex("0","3","test");
    adjacencyList->addNeighbourVertex("0","2","test");
    
    std::cout<<adjacencyList->getNeighbourVertices("0").at("test").at(0)<<std::endl;
    
    adjacencyList->removeVertex("4");
    std::cout<<adjacencyList->getNeighbourVertices("0").at("test").at(0)<<std::endl;
    
    adjacencyList->removeNeighbourVertex("0","3","test");
    std::cout<<adjacencyList->getNeighbourVertices("0").at("test").at(0)<<std::endl;
    
    adjacencyList->removeNeighbourVertex("0","2","test");
    adjacencyList->insertVertex("5");
    adjacencyList->insertVertex("6");
    test = adjacencyList->addNeighbourVertex("0","5","test");
    test = adjacencyList->addNeighbourVertex("0","6","test2");
    std::cout<<adjacencyList->getNeighbourVertices("0").at("test").at(0)<<std::endl;
    std::cout<<adjacencyList->getNeighbourVertices("0").at("test2").at(0)<<std::endl;
}

int main(int argc, char** argv) {
    testInsertNode();
    return (EXIT_SUCCESS);
}



