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
#include <stdio.h>
#include "MergeSort.hpp"
#include "Aggregation.hpp"

int batchSize = 100;

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
    ParallelAdjacencyListSchemaHashedTableManager pAdjLstSHahsedTblMgr(batchSize, 4);
    pAdjLstSHahsedTblMgr.loadGraph("data/vertexes", "data/edges");
}

int main(int argc, char** argv) {
    int iterations = 1;

    std::vector< std::vector<int> > times(iterations + 1, std::vector<int>(20, 0));

    AdjacencyListUniversalTableManager* adjLstUniTblMgr = new AdjacencyListUniversalTableManager(batchSize);
    adjLstUniTblMgr->loadGraph("data/vertexes", "data/edges");

    AdjacencyListSchemaHashedTableManager* adjLstSHahsedTblMgr = new AdjacencyListSchemaHashedTableManager(batchSize);
    adjLstSHahsedTblMgr->loadGraph("data/vertexes", "data/edges");

    AdjacencyListEmergingSchemaManager* adjLstEsTblMgr = new AdjacencyListEmergingSchemaManager(batchSize);
    adjLstEsTblMgr->loadGraph("data/vertexes", "data/edges");


    //1 desc, -1 asc
    std::vector<int> sortingDirection = {1, -1, -1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0, 2, 1};

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

        //testAdjacencyListSchemaHashedTable();
        auto t6 = std::chrono::steady_clock::now();

        //testCSRSchemaHashedTable();
        auto t7 = std::chrono::steady_clock::now();

        //testAdjacencyMatrixEmergingSchema();
        auto t8 = std::chrono::steady_clock::now();

        //testAdjacencyListEmergingSchema();
        auto t9 = std::chrono::steady_clock::now();

        //testCSREmergingSchema();

        auto t10 = std::chrono::steady_clock::now();

        //testParallelAdjacencyListSchemaHashedTable();
        auto t11 = std::chrono::steady_clock::now();



        const char* str = "2022-01-16";

        tm tm1;

        sscanf(str, "%4d-%2d-%2d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);

        std::vector<std::pair<std::vector<std::string>, std::vector<double> > > resultSet;


        auto t12 = std::chrono::steady_clock::now();
        adjLstUniTblMgr->executeQueryBI1(tm1, resultSet);

        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

        Aggregation::GroupBy(resultSet, groupByOperation);
        Aggregation::computePercentile(resultSet, 0);
        auto t13 = std::chrono::steady_clock::now();

        resultSet.clear();


        auto t14 = std::chrono::steady_clock::now();
        adjLstSHahsedTblMgr->executeQueryBI1(tm1, resultSet);

        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

        Aggregation::GroupBy(resultSet, groupByOperation);
        Aggregation::computePercentile(resultSet, 0);
        auto t15 = std::chrono::steady_clock::now();

        resultSet.clear();


        auto t16 = std::chrono::steady_clock::now();
        adjLstEsTblMgr->executeQueryBI1(tm1, resultSet);

        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

        Aggregation::GroupBy(resultSet, groupByOperation);
        Aggregation::computePercentile(resultSet, 0);
        auto t17 = std::chrono::steady_clock::now();



        int duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        std::cout << "ElapsedTime(AdjacencyMatrixUniversalTable): " << duration << " ms\n";
        times[iterations][0] += duration;
        times[i][0] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
        std::cout << "ElapsedTime(AdjacencyListUniversalTable): " << duration << " ms\n";
        times[iterations][1] += duration;
        times[i][1] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
        std::cout << "ElapsedTime(CSRUniversalTable): " << duration << " ms\n";
        times[iterations][2] += duration;
        times[i][2] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t4).count();
        std::cout << "ElapsedTime(AdjacencyMatrixSchemaHashedTable): " << duration << " ms\n";
        times[iterations][3] += duration;
        times[i][3] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count();
        std::cout << "ElapsedTime(AdjacencyListSchemaHashedTable): " << duration << " ms\n";
        times[iterations][4] += duration;
        times[i][4] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t7 - t6).count();
        std::cout << "ElapsedTime(CSRSchemaHashedTable): " << duration << " ms\n";
        times[iterations][5] += duration;
        times[i][5] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7).count();
        std::cout << "ElapsedTime(AdjacencyMatrixEmergingSchema): " << duration << " ms\n";
        times[iterations][6] += duration;
        times[i][6] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t9 - t8).count();
        std::cout << "ElapsedTime(AdjacencyListEmergingSchema): " << duration << " ms\n";
        times[iterations][7] += duration;
        times[i][7] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9).count();
        std::cout << "ElapsedTime(CSREmergingSchema): " << duration << " ms\n";
        times[iterations][8] += duration;
        times[i][8] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t11 - t10).count();
        std::cout << "ElapsedTime(ParallelAdjacencyListSchemaHashedTable): " << duration << " ms\n";
        times[iterations][9] += duration;
        times[i][9] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t13 - t12).count();
        std::cout << "ElapsedTime(Universal Table Query Execution - BI 1): " << duration << " ms\n";
        times[iterations][10] += duration;
        times[i][10] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t15 - t14).count();
        std::cout << "ElapsedTime(Schema Hashed Table Query Execution - BI 1): " << duration << " ms\n";
        times[iterations][11] += duration;
        times[i][11] = duration;

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t17 - t16).count();
        std::cout << "ElapsedTime(Emerging Schema Query Execution - BI 1): " << duration << " ms\n";
        times[iterations][12] += duration;
        times[i][12] = duration;

    }
    delete adjLstUniTblMgr;
    delete adjLstSHahsedTblMgr;
    delete adjLstEsTblMgr;


    std::cout << "--------------------------------------------------------------------------\n";
    std::cout << "===> Final Result - " << iterations << " Iterations <===\n";
    std::cout << "--------------------------------------------------------------------------\n";

    std::cout << "ElapsedTime(AdjacencyMatrixUniversalTable): " << times[iterations][0] / iterations << " ms\n";
    std::cout << "ElapsedTime(AdjacencyListUniversalTable): " << times[iterations][1] / iterations << " ms\n";
    std::cout << "ElapsedTime(CSRUniversalTable): " << times[iterations][2] / iterations << " ms\n";
    std::cout << "ElapsedTime(AdjacencyMatrixSchemaHashedTable): " << times[iterations][3] / iterations << " ms\n";
    std::cout << "ElapsedTime(AdjacencyListSchemaHashedTable): " << times[iterations][4] / iterations << " ms\n";
    std::cout << "ElapsedTime(CSRSchemaHashedTable): " << times[iterations][5] / iterations << " ms\n";
    std::cout << "ElapsedTime(AdjacencyMatrixEmergingSchema): " << times[iterations][6] / iterations << " ms\n";
    std::cout << "ElapsedTime(AdjacencyListEmergingSchema): " << times[iterations][7] / iterations << " ms\n";
    std::cout << "ElapsedTime(CSREmergingSchema): " << times[iterations][8] / iterations << " ms\n";
    std::cout << "ElapsedTime(ParallelAdjacencyListSchemaHashedTable): " << times[iterations][9] / iterations << " ms\n";
    std::cout << "ElapsedTime(Universal Table Query Execution - BI 1): " << times[iterations][10] / iterations << " ms\n";
    std::cout << "ElapsedTime(Schema Hashed Table Query Execution - BI 1): " << times[iterations][11] / iterations << " ms\n";
    std::cout << "ElapsedTime(Emerging Schema Query Execution - BI 1): " << times[iterations][12] / iterations << " ms\n";

    //Write Evaluation Data to file
    ofstream evalFile;
    auto t = std::chrono::steady_clock::now().time_since_epoch();
    evalFile.open("Evaluation-Results/Evaluation-Data-" + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(t).count()) + ".csv");

    evalFile << "Iteration, "
            << "AdjacencyMatrixUniversalTable, AdjacencyListUniversalTable, CSRUniversalTable, "
            << "AdjacencyMatrixSchemaHashedTable, AdjacencyListSchemaHashedTable, CSRSchemaHashedTable, "
            << "AdjacencyMatrixEmergingSchema, AdjacencyListEmergingSchema, CSREmergingSchema, "
            << "ParallelAdjacencyListSchemaHashedTable, "
            << "Universal Table Query Execution - BI 1, Schema Hashed Table Query Execution - BI 1, Emerging Schema Query Execution - BI 1"
            << endl;

    for (int i = 0; i < iterations; i++) {
        evalFile << i << ", ";
        for (int j = 0; j < times.front().size(); j++) {
            evalFile << times[i][j];
            if (j != times.front().size() - 1) {
                evalFile << ", ";
            }
        }
        evalFile << endl;
    }

    evalFile << "Sum, ";
    for (int j = 0; j < times.front().size(); j++) {
        evalFile << times[iterations][j];
        if (j != times.front().size() - 1) {
            evalFile << ", ";
        }
    }
    evalFile << endl;

    evalFile.close();

    return (EXIT_SUCCESS);
}


/*
 * todo list:
 * ----------
 * query: bi 18
 * query: histogram
 * size in bytes
 * groupBy (done)
 * 
 */
