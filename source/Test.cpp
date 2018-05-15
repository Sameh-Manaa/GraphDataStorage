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

/*
 * Simple C++ Test Suite
 */


void testInsertNode() {
    AdjacencyMatrix* adjacencyMatrix = new AdjacencyMatrix();
    adjacencyMatrix->insertNode();
    adjacencyMatrix->insertNode();
    adjacencyMatrix->insertNode();
    adjacencyMatrix->insertNode();

    uint64_t nodeIdPre = adjacencyMatrix->getNodeIndex(2);
    adjacencyMatrix->removeNode(1);
    uint64_t nodeIdPost = adjacencyMatrix->getNodeIndex(2);

    std::cout << "Pre Node ID: " << nodeIdPre << ", Post Node ID: " << nodeIdPost << std::endl;
}

int main(int argc, char** argv) {
    testInsertNode();
    return (EXIT_SUCCESS);
}



