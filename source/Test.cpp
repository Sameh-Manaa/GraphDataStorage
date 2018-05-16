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
    adjacencyMatrix->insertNode();
    adjacencyMatrix->insertNode();
    adjacencyMatrix->insertNode();
    adjacencyMatrix->insertNode();
    adjacencyMatrix->insertNode();

    uint64_t nodeIdPre = adjacencyMatrix->getNodeIndex(2);
    adjacencyMatrix->removeNode(1);
    uint64_t nodeIdPost = adjacencyMatrix->getNodeIndex(2);

    std::cout << "Adjacency Matrix: \r\nPre Node ID: " << nodeIdPre << ", Post Node ID: " << nodeIdPost << std::endl;

    adjacencyMatrix->addNeighbour(0,4);
    adjacencyMatrix->addNeighbour(0,3);
    adjacencyMatrix->addNeighbour(0,2);
    
    std::cout<<adjacencyMatrix->getNeighbours(0).back()<<std::endl;
    
    adjacencyMatrix->removeNode(4);
    std::cout<<adjacencyMatrix->getNeighbours(0).back()<<std::endl;
    
    adjacencyMatrix->removeNeighbour(0,3);
    std::cout<<adjacencyMatrix->getNeighbours(0).back()<<std::endl;
    
    adjacencyMatrix->addNeighbour(0,3);
    std::cout<<adjacencyMatrix->getNeighbours(0).back()<<std::endl;
    
    adjacencyMatrix->removeNeighbour(0,2);
    adjacencyMatrix->insertNode();
    bool test = adjacencyMatrix->addNeighbour(0,5);
    std::cout<<adjacencyMatrix->getNeighbours(0).back()<<std::endl;
    
    
    /////////////////////////////////////////////////////

    AdjacencyList* adjacencyList = new AdjacencyList();
    adjacencyList->insertNode();
    adjacencyList->insertNode();
    adjacencyList->insertNode();
    adjacencyList->insertNode();
    adjacencyList->insertNode();

    nodeIdPre = adjacencyList->getNodeIndex(2);
    adjacencyList->removeNode(1);
    nodeIdPost = adjacencyList->getNodeIndex(2);
    
    std::cout << "Adjacency List: \r\nPre Node ID: " << nodeIdPre << ", Post Node ID: " << nodeIdPost << std::endl;
    
    adjacencyList->addNeighbour(0,4,"test");
    adjacencyList->addNeighbour(0,3,"test");
    adjacencyList->addNeighbour(0,2,"test");
    
    std::cout<<adjacencyList->getNeighbours(0).at("test").at(0)<<std::endl;
    
    adjacencyList->removeNode(4);
    std::cout<<adjacencyList->getNeighbours(0).at("test").at(0)<<std::endl;
    
    adjacencyList->removeNeighbour(0,3,"test");
    std::cout<<adjacencyList->getNeighbours(0).at("test").at(0)<<std::endl;
    
    adjacencyList->removeNeighbour(0,2,"test");
    adjacencyList->insertNode();
    adjacencyList->insertNode();
    test = adjacencyList->addNeighbour(0,5,"test");
    test = adjacencyList->addNeighbour(0,6,"test2");
    std::cout<<adjacencyList->getNeighbours(0).at("test").at(0)<<std::endl;
    std::cout<<adjacencyList->getNeighbours(0).at("test2").at(0)<<std::endl;
}

int main(int argc, char** argv) {
    testInsertNode();
    return (EXIT_SUCCESS);
}



