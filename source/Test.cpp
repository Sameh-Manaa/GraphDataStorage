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
#include "AdjacencyMatrix.hpp"
#include "AdjacencyList.hpp"
#include "AdjacencyMatrixUniversalTableManager.hpp"
#include "AdjacencyListUniversalTableManager.hpp"
#include "AdjacencyListSchemaHashedTableManager.hpp"
#include "AdjacencyMatrixSchemaHashedTableManager.hpp"
#include "CSRUniversalTableManager.hpp"
#include "CSRSchemaHashedTableManager.hpp"
#include "AdjacencyMatrixEmergingSchemaManager.hpp"
#include "AdjacencyListEmergingSchemaManager.hpp"
#include "CSREmergingSchemaManager.hpp"
#include "ParallelAdjacencyListSchemaHashedTableManager.hpp"
#include <chrono>
#include <unistd.h>

/*
 * Simple C++ Test Suite
 */

int batchSize = 500;

void testInsertNode() {
    AdjacencyMatrix* adjacencyMatrix = new AdjacencyMatrix();
    adjacencyMatrix->insertVertex("0");
    adjacencyMatrix->insertVertex("1");
    adjacencyMatrix->insertVertex("2");
    adjacencyMatrix->insertVertex("3");
    adjacencyMatrix->insertVertex("4");

    uint64_t nodeIdPre = adjacencyMatrix->getVertexIndexByVertexId("2");
    adjacencyMatrix->removeVeretex("1");
    uint64_t nodeIdPost = adjacencyMatrix->getVertexIndexByVertexId("2");

    std::cout << "Adjacency Matrix: \r\nPre Node ID: " << nodeIdPre << ", Post Node ID: " << nodeIdPost << std::endl;

    adjacencyMatrix->addNeighbourVertex("0", "4");
    adjacencyMatrix->addNeighbourVertex("0", "3");
    adjacencyMatrix->addNeighbourVertex("0", "2");

    std::cout << adjacencyMatrix->getNeighbourVertices("0").back() << std::endl;

    adjacencyMatrix->removeVeretex("4");
    std::cout << adjacencyMatrix->getNeighbourVertices("0").back() << std::endl;

    adjacencyMatrix->removeNeighbourVertex("0", "3");
    std::cout << adjacencyMatrix->getNeighbourVertices("0").back() << std::endl;

    adjacencyMatrix->addNeighbourVertex("0", "3");
    std::cout << adjacencyMatrix->getNeighbourVertices("0").back() << std::endl;

    adjacencyMatrix->removeNeighbourVertex("0", "2");
    adjacencyMatrix->insertVertex("5");
    bool test = adjacencyMatrix->addNeighbourVertex("0", "5");
    std::cout << adjacencyMatrix->getNeighbourVertices("0").back() << std::endl;


    /////////////////////////////////////////////////////

    AdjacencyList* adjacencyList = new AdjacencyList();
    adjacencyList->insertVertex("0");
    adjacencyList->insertVertex("1");
    adjacencyList->insertVertex("2");
    adjacencyList->insertVertex("3");
    adjacencyList->insertVertex("4");

    nodeIdPre = adjacencyList->getVertexIndexByVertexId("2");
    adjacencyList->removeVertex("1");
    nodeIdPost = adjacencyList->getVertexIndexByVertexId("2");

    std::cout << "Adjacency List: \r\nPre Node ID: " << nodeIdPre << ", Post Node ID: " << nodeIdPost << std::endl;

    adjacencyList->addNeighbourVertex("0", "4", "test");
    adjacencyList->addNeighbourVertex("0", "3", "test");
    adjacencyList->addNeighbourVertex("0", "2", "test");

    std::cout << adjacencyList->getNeighbourVertices("0").at("test").at(0) << std::endl;

    adjacencyList->removeVertex("4");
    std::cout << adjacencyList->getNeighbourVertices("0").at("test").at(0) << std::endl;

    adjacencyList->removeNeighbourVertex("0", "3", "test");
    std::cout << adjacencyList->getNeighbourVertices("0").at("test").at(0) << std::endl;

    adjacencyList->removeNeighbourVertex("0", "2", "test");
    adjacencyList->insertVertex("5");
    adjacencyList->insertVertex("6");
    test = adjacencyList->addNeighbourVertex("0", "5", "test");
    test = adjacencyList->addNeighbourVertex("0", "6", "test2");
    std::cout << adjacencyList->getNeighbourVertices("0").at("test").at(0) << std::endl;
    std::cout << adjacencyList->getNeighbourVertices("0").at("test2").at(0) << std::endl;
}

void testAdjacencyMatrixUniversalTable() {
    AdjacencyMatrixUniversalTableManager adjMatUniTblMgr(batchSize);
    adjMatUniTblMgr.loadGraph("data/vertexes", "data/edges");
}

void testAdjacencyListUniversalTable() {
    AdjacencyListUniversalTableManager adjLstUniTblMgr(batchSize);
    adjLstUniTblMgr.loadGraph("data/vertexes", "data/edges");
}

void testCSRUniversalTable() {
    CSRUniversalTableManager csrUniTblMgr(batchSize);
    csrUniTblMgr.loadGraph("data/vertexes", "data/edges");
}

void testAdjacencyMatrixSchemaHashedTable() {
    AdjacencyMatrixSchemaHashedTableManager adjMatSHahsedTblMgr(batchSize);
    adjMatSHahsedTblMgr.loadGraph("data/vertexes", "data/edges");
}

void testAdjacencyListSchemaHashedTable() {
    AdjacencyListSchemaHashedTableManager adjLstSHahsedTblMgr(batchSize);
    adjLstSHahsedTblMgr.loadGraph("data/vertexes", "data/edges");
}

void testCSRSchemaHashedTable() {
    CSRSchemaHashedTableManager csrSHashedTblMgr(batchSize);
    csrSHashedTblMgr.loadGraph("data/vertexes", "data/edges");
}

void testAdjacencyMatrixEmergingSchema() {
    AdjacencyMatrixEmergingSchemaManager adjMatEsMgr(batchSize);
    adjMatEsMgr.loadGraph("data/vertexes", "data/edges");
}

void testAdjacencyListEmergingSchema() {
    AdjacencyListEmergingSchemaManager adjLstEsMgr(batchSize);
    adjLstEsMgr.loadGraph("data/vertexes", "data/edges");
}

void testCSREmergingSchema() {
    CSREmergingSchemaManager csrEsMgr(batchSize);
    csrEsMgr.loadGraph("data/vertexes", "data/edges");
}

void testParallelAdjacencyListSchemaHashedTable() {
    ParallelAdjacencyListSchemaHashedTableManager pAdjLstSHahsedTblMgr(batchSize, 1);
    pAdjLstSHahsedTblMgr.loadGraph("data/vertexes", "data/edges");
}

int main(int argc, char** argv) {
    std::vector<int> times(10, 0);

    int iterations = 5;

    for (int i = 0; i < iterations; i++) {
        auto t1 = std::chrono::steady_clock::now();

        //testAdjacencyMatrixUniversalTable();
        auto t2 = std::chrono::steady_clock::now();

        //testAdjacencyListUniversalTable();
        auto t3 = std::chrono::steady_clock::now();

        //testCSRUniversalTable();
        auto t4 = std::chrono::steady_clock::now();

        //testAdjacencyMatrixSchemaHashedTable();
        auto t5 = std::chrono::steady_clock::now();

        testAdjacencyListSchemaHashedTable();
        auto t6 = std::chrono::steady_clock::now();

        //testCSRSchemaHashedTable();
        auto t7 = std::chrono::steady_clock::now();

        //testAdjacencyMatrixEmergingSchema();
        auto t8 = std::chrono::steady_clock::now();

        //testAdjacencyListEmergingSchema();
        auto t9 = std::chrono::steady_clock::now();

        //testCSREmergingSchema();
        auto t10 = std::chrono::steady_clock::now();

        testParallelAdjacencyListSchemaHashedTable();
        auto t11 = std::chrono::steady_clock::now();

        int duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        std::cout << "ElapsedTime(AdjacencyMatrixUniversalTable): " << duration << " ms\n";
        times[0] += duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
        std::cout << "ElapsedTime(AdjacencyListUniversalTable): " << duration << " ms\n";
        times[1] += duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
        std::cout << "ElapsedTime(CSRUniversalTable): " << duration << " ms\n";
        times[2] += duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t4).count();
        std::cout << "ElapsedTime(AdjacencyMatrixSchemaHashedTable): " << duration << " ms\n";
        times[3] += duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count();
        std::cout << "ElapsedTime(AdjacencyListSchemaHashedTable): " << duration << " ms\n";
        times[4] += duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t7 - t6).count();
        std::cout << "ElapsedTime(CSRSchemaHashedTable): " << duration << " ms\n";
        times[5] += duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7).count();
        std::cout << "ElapsedTime(AdjacencyMatrixEmergingSchema): " << duration << " ms\n";
        times[6] += duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t9 - t8).count();
        std::cout << "ElapsedTime(AdjacencyListEmergingSchema): " << duration << " ms\n";
        times[7] += duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9).count();
        std::cout << "ElapsedTime(CSREmergingSchema): " << duration << " ms\n";
        times[8] += duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t11 - t10).count();
        std::cout << "ElapsedTime(ParallelAdjacencyListSchemaHashedTable): " << duration << " ms\n";
        times[9] += duration;

    }


    std::cout << "--------------------------------------------------------------------------\n";
    std::cout << "===> Final Result - " << iterations << " Iterations <===\n";
    std::cout << "--------------------------------------------------------------------------\n";

    std::cout << "ElapsedTime(AdjacencyMatrixUniversalTable): " << times[0] / iterations << " ms\n";
    std::cout << "ElapsedTime(AdjacencyListUniversalTable): " << times[1] / iterations << " ms\n";
    std::cout << "ElapsedTime(CSRUniversalTable): " << times[2] / iterations << " ms\n";
    std::cout << "ElapsedTime(AdjacencyMatrixSchemaHashedTable): " << times[3] / iterations << " ms\n";
    std::cout << "ElapsedTime(AdjacencyListSchemaHashedTable): " << times[4] / iterations << " ms\n";
    std::cout << "ElapsedTime(CSRSchemaHashedTable): " << times[5] / iterations << " ms\n";
    std::cout << "ElapsedTime(AdjacencyMatrixEmergingSchema): " << times[6] / iterations << " ms\n";
    std::cout << "ElapsedTime(AdjacencyListEmergingSchema): " << times[7] / iterations << " ms\n";
    std::cout << "ElapsedTime(CSREmergingSchema): " << times[8] / iterations << " ms\n";
    std::cout << "ElapsedTime(ParallelAdjacencyListSchemaHashedTable): " << times[9] / iterations << " ms\n";

    return (EXIT_SUCCESS);
}