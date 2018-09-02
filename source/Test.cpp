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
#include <cstdlib>
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
#include "UtilityFunctions.hpp"

typedef std::vector<std::pair<std::vector<std::string>, std::vector<double> > > queryResultSet_vec;

void writeLoadingDurationsToFile(std::string file_name, std::vector<auto> &before_ts, std::vector<auto> &after_ts) {
    std::vector<uint32_t> duration(before_ts.size());
    for (int i = 0; i < before_ts.size(); i++) {
        duration[i] = std::chrono::duration_cast<std::chrono::milliseconds>(after_ts[i] - before_ts[i]).count();
    }
    std::sort(duration.begin(), duration.end());
    //Write Evaluation Data to file
    ofstream evalFile;
    auto t = std::chrono::steady_clock::now().time_since_epoch();
    evalFile.open("Evaluation-Results/" + file_name + "_" + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(t).count()) + ".csv");

    evalFile << "Iteration, LoadingTime(ms)" << endl;

    for (int i = 0; i < duration.size(); i++) {
        evalFile << i << ", " << duration[i] << endl;
    }

    evalFile << endl;

    evalFile.close();
}

void writeQueryDurationsToFile(std::string file_name, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query) {

    struct myclass {

        bool operator()(std::pair<uint32_t, uint32_t> i, std::pair<uint32_t, uint32_t> j) {
            return (i.first < j.first);
        }
    } myComparator;


    std::vector<std::pair<uint32_t, uint32_t> > duration(before_fetch.size());
    for (int i = 0; i < duration.size(); i++) {
        duration[i].first = std::chrono::duration_cast<std::chrono::milliseconds>(after_query[i] - before_fetch[i]).count();
        duration[i].second = std::chrono::duration_cast<std::chrono::milliseconds>(after_fetch[i] - before_fetch[i]).count();
    }
    std::sort(duration.begin(), duration.end(), myComparator);
    //Write Evaluation Data to file
    ofstream evalFile;
    auto t = std::chrono::steady_clock::now().time_since_epoch();
    evalFile.open("Evaluation-Results/" + file_name + "_" + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(t).count()) + ".csv");

    evalFile << "Iteration, FullQueryTime(ms), DataFetchTime(ms)" << endl;

    for (int i = 0; i < duration.size(); i++) {
        evalFile << i << ", " << duration[i].first << ", " << duration[i].second << endl;
    }

    evalFile << endl;

    evalFile.close();
}

void writeQueryResultToFile(std::string file_name, std::string headerLine, queryResultSet_vec &resultSet) {
    //Write Evaluation Data to file
    ofstream evalFile;
    auto t = std::chrono::steady_clock::now().time_since_epoch();
    evalFile.open("Evaluation-Results/" + file_name + "_" + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(t).count()) + ".csv");

    evalFile << headerLine << endl;

    for (int i = 0; i < resultSet.size(); i++) {
        for (int j = 0; j < resultSet[i].first.size(); j++) {
            evalFile << resultSet[i].first[j] << ", ";
        }

        for (int j = 0; j < resultSet[i].second.size(); j++) {
            evalFile << resultSet[i].second[j] << ", ";
        }

        evalFile << endl;
    }

    evalFile << endl;

    evalFile.close();
}

void testAdjacencyMatrix(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts, std::string scaleFactor = "1") {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: AdjacencyMatrix - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyMatrixUniversalTableManager adjMat(batchSize, true, false);
        before_ts[i] = std::chrono::steady_clock::now();
        adjMat.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges", 0);
        after_ts[i] = std::chrono::steady_clock::now();

        if (i == iterations - 1) {
            ofstream evalFile;
            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

            evalFile << scaleFactor << ", Adjacency Matrix, " << std::to_string(adjMat.getAdjacencyMatrixSizeInBytes()) << endl;

            evalFile.close();
        }
    }

    std::cout << "Loading End: AdjacencyMatrix - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;
}

void testAdjacencyList(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts, std::string scaleFactor = "1") {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: AdjacencyList - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyListUniversalTableManager adjLst(batchSize, true, false);
        before_ts[i] = std::chrono::steady_clock::now();
        adjLst.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges", 0);
        after_ts[i] = std::chrono::steady_clock::now();

        if (i == iterations - 1) {
            ofstream evalFile;
            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

            evalFile << scaleFactor << ", Adjacency List, " << std::to_string(adjLst.getAdjacencyListSizeInBytes()) << endl;

            evalFile.close();
        }
    }
    std::cout << "Loading End: AdjacencyList - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyList2(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts, std::string scaleFactor = "1") {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: AdjacencyList - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyListSchemaHashedTableManager SHahsedTblMgr(batchSize, true, false);
        before_ts[i] = std::chrono::steady_clock::now();
        SHahsedTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges", 0);
        after_ts[i] = std::chrono::steady_clock::now();

        if (i == iterations - 1) {
            ofstream evalFile;
            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

            evalFile << scaleFactor << ", Adjacency List, " << std::to_string(SHahsedTblMgr.getAdjacencyListSizeInBytes()) << endl;

            evalFile.close();
        }
    }
    std::cout << "Loading End: AdjacencyList - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testCSR(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts, std::string scaleFactor = "1") {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: CSR - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        CSRUniversalTableManager csr(batchSize, true, false);
        before_ts[i] = std::chrono::steady_clock::now();
        csr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges", 0);
        after_ts[i] = std::chrono::steady_clock::now();

        if (i == iterations - 1) {
            ofstream evalFile;
            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

            evalFile << scaleFactor << ", CSR, " << std::to_string(csr.getCSRSizeInBytes()) << endl;

            evalFile.close();
        }
    }
    std::cout << "Loading End: CSR - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testCSR2(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts, std::string scaleFactor = "1") {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: CSR - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        CSRSchemaHashedTableManager SHahsedTblMgr(batchSize, true, false);
        before_ts[i] = std::chrono::steady_clock::now();
        SHahsedTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges", 0);
        after_ts[i] = std::chrono::steady_clock::now();

        if (i == iterations - 1) {
            ofstream evalFile;
            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

            evalFile << scaleFactor << ", CSR, " << std::to_string(SHahsedTblMgr.getCSRSizeInBytes()) << endl;

            evalFile.close();
        }
    }
    std::cout << "Loading End: CSR - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testUniversalTable(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts, std::string scaleFactor = "1") {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: UniversalTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyListUniversalTableManager UniTblMgr(batchSize, false, true);
        before_ts[i] = std::chrono::steady_clock::now();
        UniTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges", 0);
        after_ts[i] = std::chrono::steady_clock::now();

        if (i == iterations - 1) {
            ofstream evalFile;
            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

            evalFile << scaleFactor << ", Universal Table, " << std::to_string(UniTblMgr.getUniversalTableSizeInBytes()) << endl;

            evalFile.close();
        }
    }
    std::cout << "Loading End: UniversalTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testSchemaHashedTable(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts, std::string scaleFactor = "1") {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: SchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyListSchemaHashedTableManager SHahsedTblMgr(batchSize, false, true);
        before_ts[i] = std::chrono::steady_clock::now();
        SHahsedTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges", 0);
        after_ts[i] = std::chrono::steady_clock::now();

        if (i == iterations - 1) {
            ofstream evalFile;
            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

            evalFile << scaleFactor << ", Schema Hashed Table, " << std::to_string(SHahsedTblMgr.getSchemaHashedTableSizeInBytes()) << endl;

            evalFile.close();
        }
    }
    std::cout << "Loading End: SchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testEmergingSchema(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts, std::string scaleFactor = "1") {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: EmergingSchema - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyListEmergingSchemaManager EsMgr(batchSize, false, true);
        before_ts[i] = std::chrono::steady_clock::now();
        EsMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges", 0);
        after_ts[i] = std::chrono::steady_clock::now();

        if (i == iterations - 1) {
            ofstream evalFile;
            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

            evalFile << scaleFactor << ", Emerging Schema, " << std::to_string(EsMgr.getEmergingSchemaSizeInBytes()) << endl;

            evalFile.close();
        }
    }

    std::cout << "Loading End: EmergingSchema - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testParallelAdjacencyList(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts, uint8_t parallelismDegree, std::string scaleFactor = "1") {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: ParallelAdjacencyList - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << " - Parallelism: " << std::to_string(parallelismDegree) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        ParallelAdjacencyListSchemaHashedTableManager pAdjLst(batchSize, parallelismDegree, true, false);
        before_ts[i] = std::chrono::steady_clock::now();
        pAdjLst.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }

    ParallelAdjacencyListSchemaHashedTableManager pAdjLst(batchSize, parallelismDegree);
    pAdjLst.loadGraph("..", "..");

    std::cout << "Loading End: ParallelAdjacencyList - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << " - Parallelism: " << std::to_string(parallelismDegree) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testParallelSchemaHashedTable(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts, uint8_t parallelismDegree, std::string scaleFactor = "1") {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: ParallelSchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << " - Parallelism: " << std::to_string(parallelismDegree) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        ParallelAdjacencyListSchemaHashedTableManager pSHahsedTblMgr(batchSize, parallelismDegree, false, true);
        before_ts[i] = std::chrono::steady_clock::now();
        pSHahsedTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }

    ParallelAdjacencyListSchemaHashedTableManager pSHahsedTblMgr(batchSize, parallelismDegree);
    pSHahsedTblMgr.loadGraph("..", "..");

    std::cout << "Loading End: ParallelSchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << " - Parallelism: " << std::to_string(parallelismDegree) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testUniversalTable_Query_BI1(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: UniversalTable_Query_BI1 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyListUniversalTableManager adjLstUniTblMgr(batchSize);
    adjLstUniTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    //1 desc, -1 asc
    std::vector<int> sortingDirection = {1, -1, -1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0, 2, 1};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        adjLstUniTblMgr.executeQueryBI1(messageCreationDate, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::computePercentile(resultSet, 0);
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testSchemaHashedTable_Query_BI1(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: SchemaHashedTable_Query_BI1 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;

    std::cout << "==========================================================================" << std::endl;

    AdjacencyListSchemaHashedTableManager adjLstSHahsedTblMgr(batchSize);
    adjLstSHahsedTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    //1 desc, -1 asc
    std::vector<int> sortingDirection = {1, -1, -1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0, 2, 1};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        adjLstSHahsedTblMgr.executeQueryBI1(messageCreationDate, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::computePercentile(resultSet, 0);
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;
}

void testEmergingSchema_Query_BI1(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: EmergingSchema_Query_BI1 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyListEmergingSchemaManager adjLstEsMgr(batchSize);
    adjLstEsMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    //1 desc, -1 asc
    std::vector<int> sortingDirection = {1, -1, -1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0, 2, 1};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        adjLstEsMgr.executeQueryBI1(messageCreationDate, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::computePercentile(resultSet, 0);
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyMatrixUniversalTable_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyMatrixUniversalTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyMatrixUniversalTableManager adjMatUniTblMgr(batchSize, false, true);
    adjMatUniTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection = {-1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        adjMatUniTblMgr.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::GroupByValue(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {1, 1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyListUniversalTable_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyListUniversalTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyListUniversalTableManager adjLstUniTblMgr(batchSize);
    adjLstUniTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection = {-1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        adjLstUniTblMgr.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::GroupByValue(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {1, 1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testCSRUniversalTable_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: CSRUniversalTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    CSRUniversalTableManager csrUniTblMgr(batchSize);
    csrUniTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection = {-1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        csrUniTblMgr.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::GroupByValue(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {1, 1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyMatrixSchemaHashedTable_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);
    std::cout << "==========================================================================" << std::endl;

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyMatrixSchemaHashedTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;

    AdjacencyMatrixSchemaHashedTableManager adjMatSHashedTblMgr(batchSize);
    adjMatSHashedTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection = {-1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        adjMatSHashedTblMgr.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::GroupByValue(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {1, 1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyListSchemaHashedTable_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);
    std::cout << "==========================================================================" << std::endl;

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyListSchemaHashedTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;

    AdjacencyListSchemaHashedTableManager adjLstSHashedTblMgr(batchSize);
    adjLstSHashedTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection = {-1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        adjLstSHashedTblMgr.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::GroupByValue(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {1, 1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testCSRSchemaHashedTable_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: CSRSchemaHashedTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    CSRSchemaHashedTableManager csrSHashedTblMgr(batchSize);
    csrSHashedTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection = {-1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        csrSHashedTblMgr.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::GroupByValue(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {1, 1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyMatrixEmergingSchema_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyMatrixEmergingSchema_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyMatrixEmergingSchemaManager adjMatEsTblMgr(batchSize);
    adjMatEsTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection = {-1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        adjMatEsTblMgr.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::GroupByValue(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {1, 1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyListEmergingSchema_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyListEmergingSchema_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyListEmergingSchemaManager adjLstEsTblMgr(batchSize);
    adjLstEsTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection = {-1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        adjLstEsTblMgr.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::GroupByValue(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {1, 1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testCSREmergingSchema_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: CSREmergingSchema_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    CSREmergingSchemaManager csrEsTblMgr(batchSize);
    csrEsTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection = {-1};
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        csrEsTblMgr.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        UtilityFunctions::GroupByValue(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {1, 1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyMatrix_Query_DC(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyMatrix_Query_DC - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyMatrixUniversalTableManager adjMatUniTblMgr(batchSize);
    adjMatUniTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection;
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        adjMatUniTblMgr.executeQueryDC(resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupByDcQuery(resultSet, groupByOperation);
        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {-1, -1, -1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyList_Query_DC(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyList_Query_DC - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyListUniversalTableManager adjLstUniTblMgr(batchSize);
    adjLstUniTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection;
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        adjLstUniTblMgr.executeQueryDC(resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupByDcQuery(resultSet, groupByOperation);
        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {-1, -1, -1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testCSR_Query_DC(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, queryResultSet_vec &resultSet, std::string scaleFactor = "1") {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: CSR_Query_DC - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    CSRUniversalTableManager csrUniTblMgr(batchSize);
    csrUniTblMgr.loadGraph("data_" + scaleFactor + "/vertexes", "data_" + scaleFactor + "/edges");

    std::vector<int> sortingDirection;
    //index: group by operation: 0 count, 1 sum, 2 average
    //value: storage index 0,1,2
    std::vector<uint8_t> groupByOperation = {0};

    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;

        resultSet.clear();

        before_fetch[i] = std::chrono::steady_clock::now();

        csrUniTblMgr.executeQueryDC(resultSet);
        after_fetch[i] = std::chrono::steady_clock::now();

        UtilityFunctions::GroupByDcQuery(resultSet, groupByOperation);
        UtilityFunctions::GroupBy(resultSet, groupByOperation);
        //1 desc, -1 asc
        sortingDirection = {-1, -1, -1};
        MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

        after_query[i] = std::chrono::steady_clock::now();
    }
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

int main(int argc, char** argv) {
    int iterations = 20, iterations_query = 20;
    uint32_t batchSize_start = 1, batchSize_end = 100000;
    double ScaleFactor_start = 0.25, ScaleFactor_end = 4;
    double ScaleFactorAdjMatrix_start = 0.25, ScaleFactorAdjMatrix_end = 0.25;
    std::vector<std::chrono::time_point<std::chrono::steady_clock> > before_ts, after_ts, before_fetch, after_fetch, after_query;

    const int dir_err = system("mkdir -p Evaluation-Results");
    if (-1 == dir_err) {
        printf("Error creating directory!n");
        exit(1);
    }

    //
    //testAdjacencyList(1, 1, before_ts, after_ts, std::to_string(1.00).substr(0, 4));

    //testAdjacencyList2(1000, 1, before_ts, after_ts, std::to_string(1.00).substr(0, 4));

    //testCSR(100000, 1, before_ts, after_ts, std::to_string(1.00).substr(0, 4));

    //testCSR2(1000, 1, before_ts, after_ts, std::to_string(1.00).substr(0, 4));

    //return 0;

    //Machine 1 -- AdjacencyMatrix Machine


    /*
     * (Q1) How scalable are the data structures to accommodate graph data with 
     *      larger size (i.e. higher number of vertices and/or edges)?
     */

    {
        for (double scaleFactor = ScaleFactor_start; scaleFactor <= 1; scaleFactor *= 2) {

            testAdjacencyList(1000, iterations, before_ts, after_ts, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q1_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

        }

        for (double scaleFactor = 2; scaleFactor <= ScaleFactor_end; scaleFactor *= 2) {

            testAdjacencyList(1000, iterations, before_ts, after_ts, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q1_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testCSR(1000, iterations, before_ts, after_ts, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q1_CSR_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testUniversalTable(1000, iterations, before_ts, after_ts, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q1_UniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testSchemaHashedTable(1000, iterations, before_ts, after_ts, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q1_SchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testEmergingSchema(1000, iterations, before_ts, after_ts, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q1_EmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

        }
    }

    /*
     * (Q2) What is the effect of the batch size when loading the data into the 
     *      data structure in batches?
     */


    {
        for (uint32_t batchSize = batchSize_start; batchSize <= batchSize_end; batchSize *= 10) {

            testAdjacencyList(batchSize, iterations, before_ts, after_ts, "1.00");
            writeLoadingDurationsToFile("Q1_AdjacencyList_1.00_" + std::to_string(batchSize), before_ts, after_ts);

            testCSR(batchSize, iterations, before_ts, after_ts, "1.00");
            writeLoadingDurationsToFile("Q1_CSR_1.00_" + std::to_string(batchSize), before_ts, after_ts);

            testUniversalTable(batchSize, iterations, before_ts, after_ts, "1.00");
            writeLoadingDurationsToFile("Q1_UniversalTable_1.00_" + std::to_string(batchSize), before_ts, after_ts);

            testSchemaHashedTable(batchSize, iterations, before_ts, after_ts, "1.00");
            writeLoadingDurationsToFile("Q1_SchemaHashedTable_1.00_" + std::to_string(batchSize), before_ts, after_ts);

            testEmergingSchema(batchSize, iterations, before_ts, after_ts, "1.00");
            writeLoadingDurationsToFile("Q1_EmergingSchema_1.00_" + std::to_string(batchSize), before_ts, after_ts);
        }
    }




    /*
     * (Q3) Can loading the data in parallel improve the performance of loading?
     */

    {

        for (double scaleFactor = ScaleFactor_start; scaleFactor <= ScaleFactor_end; scaleFactor *= 4) {

            testParallelAdjacencyList(1000, iterations, before_ts, after_ts, 1, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q3_Parallel(1)_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testParallelAdjacencyList(1000, iterations, before_ts, after_ts, 2, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q3_Parallel(2)_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testParallelAdjacencyList(1000, iterations, before_ts, after_ts, 4, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q3_Parallel(4)_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testParallelAdjacencyList(1000, iterations, before_ts, after_ts, 8, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q3_Parallel(8)_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testParallelAdjacencyList(1000, iterations, before_ts, after_ts, 16, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q3_Parallel(16)_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testParallelSchemaHashedTable(1000, iterations, before_ts, after_ts, 1, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q3_Parallel(1)_SchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testParallelSchemaHashedTable(1000, iterations, before_ts, after_ts, 2, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q3_Parallel(2)_SchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testParallelSchemaHashedTable(1000, iterations, before_ts, after_ts, 4, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q3_Parallel(4)_SchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testParallelSchemaHashedTable(1000, iterations, before_ts, after_ts, 8, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q3_Parallel(8)_SchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);

            testParallelSchemaHashedTable(1000, iterations, before_ts, after_ts, 16, std::to_string(scaleFactor).substr(0, 4));
            writeLoadingDurationsToFile("Q3_Parallel(16)_SchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);
        }
    }



    /*
     * (Q4) Which data structure performs better in simple non-traversal queries?
     */

    {
        queryResultSet_vec resultSet;
        tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-21T22:00:00");


        for (double scaleFactor = ScaleFactor_start; scaleFactor <= ScaleFactor_end; scaleFactor *= 4) {

            testUniversalTable_Query_BI1(1000, iterations_query, before_fetch, after_fetch, after_query, messageCreationDate, resultSet, std::to_string(scaleFactor).substr(0, 4));
            writeQueryDurationsToFile("Q4_Query_BI1_UniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
            writeQueryResultToFile("Q4_QueryResult_BI1_UniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "year, isComment, lengthCategory, messageCount, averageMessageLength, sumMessageLength, percentageOfMessages", resultSet);

            testSchemaHashedTable_Query_BI1(1000, iterations_query, before_fetch, after_fetch, after_query, messageCreationDate, resultSet, std::to_string(scaleFactor).substr(0, 4));
            writeQueryDurationsToFile("Q4_Query_BI1_SchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
            writeQueryResultToFile("Q4_QueryResult_BI1_SchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "year, isComment, lengthCategory, messageCount, averageMessageLength, sumMessageLength, percentageOfMessages", resultSet);

            testEmergingSchema_Query_BI1(1000, iterations_query, before_fetch, after_fetch, after_query, messageCreationDate, resultSet, std::to_string(scaleFactor).substr(0, 4));
            writeQueryDurationsToFile("Q4_Query_BI1_EmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
            writeQueryResultToFile("Q4_QueryResult_BI1_EmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "year, isComment, lengthCategory, messageCount, averageMessageLength, sumMessageLength, percentageOfMessages", resultSet);
        }
    }




    /*
     * (Q5) Which data structure performs better in complex traversal queries?
     */

    {
        queryResultSet_vec resultSet;
        tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-22T00:00:00");
        uint16_t messageLength = 20;
        std::vector<std::string> messageLanguages = {"ar"};


        for (double scaleFactor = ScaleFactor_start; scaleFactor <= ScaleFactor_end; scaleFactor *= 4) {

            testAdjacencyListUniversalTable_Query_BI18(1000, iterations_query, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
            writeQueryDurationsToFile("Q5_Query_BI18_AdjacencyListUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
            writeQueryResultToFile("Q5_QueryResult_BI18_AdjacencyListUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);

            testAdjacencyListSchemaHashedTable_Query_BI18(1000, iterations_query, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
            writeQueryDurationsToFile("Q5_Query_BI18_AdjacencyListSchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
            writeQueryResultToFile("Q5_QueryResult_BI18_AdjacencyListSchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);

            testAdjacencyListEmergingSchema_Query_BI18(1000, iterations_query, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
            writeQueryDurationsToFile("Q5_Query_BI18_AdjacencyListEmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
            writeQueryResultToFile("Q5_QueryResult_BI18_AdjacencyListEmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);

            testCSRUniversalTable_Query_BI18(1000, iterations_query, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
            writeQueryDurationsToFile("Q5_Query_BI18_CSRUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
            writeQueryResultToFile("Q5_QueryResult_BI18_CSRUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);

            testCSRSchemaHashedTable_Query_BI18(1000, iterations_query, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
            writeQueryDurationsToFile("Q5_Query_BI18_CSRSchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
            writeQueryResultToFile("Q5_QueryResult_BI18_CSRSchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);

            testCSREmergingSchema_Query_BI18(1000, iterations_query, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
            writeQueryDurationsToFile("Q5_Query_BI18_CSREmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
            writeQueryResultToFile("Q5_QueryResult_BI18_CSREmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);
        }

    }



    /*
     * (Q6) Which data structure performs better in full scanning the graph topology?
     */

    {
        queryResultSet_vec resultSet;


        for (double scaleFactor = ScaleFactor_end; scaleFactor >= ScaleFactor_start; scaleFactor /= 4) {

            testAdjacencyList_Query_DC(1000, iterations_query, before_fetch, after_fetch, after_query, resultSet, std::to_string(scaleFactor).substr(0, 4));
            writeQueryDurationsToFile("Q6_Query_DC_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
            writeQueryResultToFile("Q6_QueryResult_DC_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "EdgeDirection, EdgeLabel, EdgeCount, VertexCount", resultSet);

            testCSR_Query_DC(1000, iterations_query, before_fetch, after_fetch, after_query, resultSet, std::to_string(scaleFactor).substr(0, 4));
            writeQueryDurationsToFile("Q6_Query_DC_CSR_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
            writeQueryResultToFile("Q6_QueryResult_DC_CSR_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "EdgeDirection, EdgeLabel, EdgeCount, VertexCount", resultSet);

        }
    }




    //Machine 2 -- AdjacencyList & CSR Machine


    /*
     * (Q1) How scalable are the data structures to accommodate graph data with 
     *      larger size (i.e. higher number of vertices and/or edges)?
     */
    //
    //    {
    //        for (double scaleFactor = ScaleFactor_start; scaleFactor <= ScaleFactor_end; scaleFactor *= 2) {
    //
    //            testAdjacencyListUniversalTable(1000, iterations, before_ts, after_ts, std::to_string(scaleFactor).substr(0, 4));
    //            writeLoadingDurationsToFile("Q1_AdjacencyListUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);
    //
    //            testCSRUniversalTable(1000, iterations, before_ts, after_ts, std::to_string(scaleFactor).substr(0, 4));
    //            writeLoadingDurationsToFile("Q1_CSRUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);
    //
    //        }
    //    }








    //
    //    /*
    //     * (Q1) How scalable are the data structures to accommodate graph data with 
    //     *      larger size (i.e. higher number of vertices and/or edges)?
    //     */
    //
    //    {
    //        //loading data without "comment" or "post" files -- filesToLoad = 2
    //
    //        //testAdjacencyMatrixUniversalTable(1000, iterations, before_ts, after_ts, 2);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyMatrixUniversalTable_2_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyListUniversalTable(1000, iterations, before_ts, after_ts, 2);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyListUniversalTable_2_" + to_string(1000), before_ts, after_ts);
    //
    //        testCSRUniversalTable(1000, iterations, before_ts, after_ts, 2);
    //        writeLoadingDurationsToFile("Q1_CSRUniversalTable_2_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyMatrixSchemaHashedTable(1000, iterations, before_ts, after_ts, 2);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyMatrixSchemaHashedTable_2_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyListSchemaHashedTable(1000, iterations, before_ts, after_ts, 2);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyListSchemaHashedTable_2_" + to_string(1000), before_ts, after_ts);
    //
    //        testCSRSchemaHashedTable(1000, iterations, before_ts, after_ts, 2);
    //        writeLoadingDurationsToFile("Q1_CSRSchemaHashedTable_2_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyMatrixEmergingSchema(1000, iterations, before_ts, after_ts, 2);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyMatrixEmergingSchema_2_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyListEmergingSchema(1000, iterations, before_ts, after_ts, 2);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyListEmergingSchema_2_" + to_string(1000), before_ts, after_ts);
    //
    //        testCSREmergingSchema(1000, iterations, before_ts, after_ts, 2);
    //        writeLoadingDurationsToFile("Q1_CSREmergingSchema_2_" + to_string(1000), before_ts, after_ts);
    //
    //        //======================================================================
    //
    //
    //
    //
    //        //loading data without "post" files -- filesToLoad = 1
    //
    //        //testAdjacencyMatrixUniversalTable(1000, iterations, before_ts, after_ts, 1);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyMatrixUniversalTable_1_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyListUniversalTable(1000, iterations, before_ts, after_ts, 1);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyListUniversalTable_1_" + to_string(1000), before_ts, after_ts);
    //
    //        testCSRUniversalTable(1000, iterations, before_ts, after_ts, 1);
    //        writeLoadingDurationsToFile("Q1_CSRUniversalTable_1_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyMatrixSchemaHashedTable(1000, iterations, before_ts, after_ts, 1);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyMatrixSchemaHashedTable_1_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyListSchemaHashedTable(1000, iterations, before_ts, after_ts, 1);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyListSchemaHashedTable_1_" + to_string(1000), before_ts, after_ts);
    //
    //        testCSRSchemaHashedTable(1000, iterations, before_ts, after_ts, 1);
    //        writeLoadingDurationsToFile("Q1_CSRSchemaHashedTable_1_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyMatrixEmergingSchema(1000, iterations, before_ts, after_ts, 1);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyMatrixEmergingSchema_1_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyListEmergingSchema(1000, iterations, before_ts, after_ts, 1);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyListEmergingSchema_1_" + to_string(1000), before_ts, after_ts);
    //
    //        testCSREmergingSchema(1000, iterations, before_ts, after_ts, 1);
    //        writeLoadingDurationsToFile("Q1_CSREmergingSchema_1_" + to_string(1000), before_ts, after_ts);
    //
    //        //======================================================================
    //
    //
    //
    //
    //        //loading data from all files -- filesToLoad = 0
    //
    //        //testAdjacencyMatrixUniversalTable(1000, iterations, before_ts, after_ts);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyMatrixUniversalTable_0_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyListUniversalTable(1000, iterations, before_ts, after_ts);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyListUniversalTable_0_" + to_string(1000), before_ts, after_ts);
    //
    //        testCSRUniversalTable(1000, iterations, before_ts, after_ts);
    //        writeLoadingDurationsToFile("Q1_CSRUniversalTable_0_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyMatrixSchemaHashedTable(1000, iterations, before_ts, after_ts);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyMatrixSchemaHashedTable_0_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyListSchemaHashedTable(1000, iterations, before_ts, after_ts);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyListSchemaHashedTable_0_" + to_string(1000), before_ts, after_ts);
    //
    //        testCSRSchemaHashedTable(1000, iterations, before_ts, after_ts);
    //        writeLoadingDurationsToFile("Q1_CSRSchemaHashedTable_0_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyMatrixEmergingSchema(1000, iterations, before_ts, after_ts);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyMatrixEmergingSchema_0_" + to_string(1000), before_ts, after_ts);
    //
    //        //testAdjacencyListEmergingSchema(1000, iterations, before_ts, after_ts);
    //        //writeLoadingDurationsToFile("Q1_AdjacencyListEmergingSchema_0_" + to_string(1000), before_ts, after_ts);
    //
    //        testCSREmergingSchema(1000, iterations, before_ts, after_ts);
    //        writeLoadingDurationsToFile("Q1_CSREmergingSchema_0_" + to_string(1000), before_ts, after_ts);
    //    }
    //
    //
    //    /*
    //     * (Q2) What is the effect of the batch size when loading the data into the 
    //     *      data structure in batches?
    //     */
    //
    //    {
    //        //loading data without "comment" or "post" files -- filesToLoad = 2
    //        for (uint32_t batchSize = batchSize_start; batchSize <= batchSize_end; batchSize *= 10) {
    //
    //            testAdjacencyMatrixUniversalTable(batchSize, iterations, before_ts, after_ts, 2);
    //            writeLoadingDurationsToFile("Q2_AdjacencyMatrixUniversalTable_2_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testAdjacencyListUniversalTable(batchSize, iterations, before_ts, after_ts, 2);
    //            writeLoadingDurationsToFile("Q2_AdjacencyListUniversalTable_2_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testCSRUniversalTable(batchSize, iterations, before_ts, after_ts, 2);
    //            writeLoadingDurationsToFile("Q2_CSRUniversalTable_2_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testAdjacencyMatrixSchemaHashedTable(batchSize, iterations, before_ts, after_ts, 2);
    //            writeLoadingDurationsToFile("Q2_AdjacencyMatrixSchemaHashedTable_2_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testAdjacencyListSchemaHashedTable(batchSize, iterations, before_ts, after_ts, 2);
    //            writeLoadingDurationsToFile("Q2_AdjacencyListSchemaHashedTable_2_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testCSRSchemaHashedTable(batchSize, iterations, before_ts, after_ts, 2);
    //            writeLoadingDurationsToFile("Q2_CSRSchemaHashedTable_2_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testAdjacencyMatrixEmergingSchema(batchSize, iterations, before_ts, after_ts, 2);
    //            writeLoadingDurationsToFile("Q2_AdjacencyMatrixEmergingSchema_2_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testAdjacencyListEmergingSchema(batchSize, iterations, before_ts, after_ts, 2);
    //            writeLoadingDurationsToFile("Q2_AdjacencyListEmergingSchema_2_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testCSREmergingSchema(batchSize, iterations, before_ts, after_ts, 2);
    //            writeLoadingDurationsToFile("Q2_CSREmergingSchema_2_" + to_string(batchSize), before_ts, after_ts);
    //        }
    //
    //        //======================================================================
    //
    //
    //
    //
    //        //loading data from all files -- filesToLoad = 0
    //
    //        for (uint32_t batchSize = batchSize_start; batchSize <= batchSize_end; batchSize *= 10) {
    //
    //            //testAdjacencyMatrixUniversalTable(batchSize, iterations, before_ts, after_ts);
    //            //writeLoadingDurationsToFile("Q2_AdjacencyMatrixUniversalTable_0_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testAdjacencyListUniversalTable(batchSize, iterations, before_ts, after_ts);
    //            writeLoadingDurationsToFile("Q2_AdjacencyListUniversalTable_0_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testCSRUniversalTable(batchSize, iterations, before_ts, after_ts);
    //            writeLoadingDurationsToFile("Q2_CSRUniversalTable_0_" + to_string(batchSize), before_ts, after_ts);
    //
    //            //testAdjacencyMatrixSchemaHashedTable(batchSize, iterations, before_ts, after_ts);
    //            //writeLoadingDurationsToFile("Q2_AdjacencyMatrixSchemaHashedTable_0_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testAdjacencyListSchemaHashedTable(batchSize, iterations, before_ts, after_ts);
    //            writeLoadingDurationsToFile("Q2_AdjacencyListSchemaHashedTable_0_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testCSRSchemaHashedTable(batchSize, iterations, before_ts, after_ts);
    //            writeLoadingDurationsToFile("Q2_CSRSchemaHashedTable_0_" + to_string(batchSize), before_ts, after_ts);
    //
    //            //testAdjacencyMatrixEmergingSchema(batchSize, iterations, before_ts, after_ts);
    //            //writeLoadingDurationsToFile("Q2_AdjacencyMatrixEmergingSchema_0_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testAdjacencyListEmergingSchema(batchSize, iterations, before_ts, after_ts);
    //            writeLoadingDurationsToFile("Q2_AdjacencyListEmergingSchema_0_" + to_string(batchSize), before_ts, after_ts);
    //
    //            testCSREmergingSchema(batchSize, iterations, before_ts, after_ts);
    //            writeLoadingDurationsToFile("Q2_CSREmergingSchema_0_" + to_string(batchSize), before_ts, after_ts);
    //        }
    //    }
    //
    //
    //
    //    /*
    //     * (Q3) Can loading the data in parallel improve the performance of loading?
    //     */
    //
    //    {
    //        //loading data from all files -- filesToLoad = 0
    //
    //        testParallelAdjacencyListSchemaHashedTable(1000, iterations, before_ts, after_ts, 1);
    //        writeLoadingDurationsToFile("Q3_Parallel(1)_AdjacencyListSchemaHashedTable_0_" + to_string(1000), before_ts, after_ts);
    //
    //        testParallelAdjacencyListSchemaHashedTable(1000, iterations, before_ts, after_ts, 2);
    //        writeLoadingDurationsToFile("Q3_Parallel(2)_AdjacencyListSchemaHashedTable_0_" + to_string(1000), before_ts, after_ts);
    //
    //        testParallelAdjacencyListSchemaHashedTable(1000, iterations, before_ts, after_ts, 4);
    //        writeLoadingDurationsToFile("Q3_Parallel(4)_AdjacencyListSchemaHashedTable_0_" + to_string(1000), before_ts, after_ts);
    //
    //        testParallelAdjacencyListSchemaHashedTable(1000, iterations, before_ts, after_ts, 8);
    //        writeLoadingDurationsToFile("Q3_Parallel(8)_AdjacencyListSchemaHashedTable_0_" + to_string(1000), before_ts, after_ts);
    //
    //        testParallelAdjacencyListSchemaHashedTable(1000, iterations, before_ts, after_ts, 16);
    //        writeLoadingDurationsToFile("Q3_Parallel(16)_AdjacencyListSchemaHashedTable_0_" + to_string(1000), before_ts, after_ts);
    //    }
    //
    //
    //
    //
    //
    //    /*
    //     * (Q4) Which data structure performs better in simple non-traversal queries?
    //     */
    //    iterations = 100;
    //
    //    {
    //        queryResultSet_vec resultSet;
    //        tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-21T22:00:00");
    //
    //        //loading data from all files -- filesToLoad = 0
    //
    //        testUniversalTable_Query_BI1(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, resultSet);
    //        writeQueryDurationsToFile("Q4_Query_BI1_UniversalTable_0_", before_fetch, after_fetch, after_query);
    //
    //        testSchemaHashedTable_Query_BI1(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, resultSet);
    //        writeQueryDurationsToFile("Q4_Query_BI1_SchemaHashedTable_0_", before_fetch, after_fetch, after_query);
    //
    //        testEmergingSchema_Query_BI1(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, resultSet);
    //        writeQueryDurationsToFile("Q4_Query_BI1_EmergingSchema_0_", before_fetch, after_fetch, after_query);
    //
    //
    //        writeQueryResultToFile("Q4_QueryResult_BI1_0", "year, isComment, lengthCategory, messageCount, averageMessageLength, sumMessageLength, percentageOfMessages", resultSet);
    //    }
    //
    //
    //
    //
    //    /*
    //     * (Q5) Which data structure performs better in complex traversal queries?
    //     */
    //
    //    {
    //        queryResultSet_vec resultSet;
    //        tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-22T00:00:00");
    //        uint16_t messageLength = 20;
    //        std::vector<std::string> messageLanguages = {"ar"};
    //
    //        //loading data from all files -- filesToLoad = 0
    //
    //        testAdjacencyListUniversalTable_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    //        writeQueryDurationsToFile("Q5_Query_BI18_AdjacencyListUniversalTable_0_", before_fetch, after_fetch, after_query);
    //
    //        testCSRUniversalTable_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    //        writeQueryDurationsToFile("Q5_Query_BI18_CSRUniversalTable_0_", before_fetch, after_fetch, after_query);
    //
    //        testAdjacencyListSchemaHashedTable_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    //        writeQueryDurationsToFile("Q5_Query_BI18_AdjacencyListSchemaHashedTable_0_", before_fetch, after_fetch, after_query);
    //
    //        testCSRSchemaHashedTable_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    //        writeQueryDurationsToFile("Q5_Query_BI18_CSRSchemaHashedTable_0_", before_fetch, after_fetch, after_query);
    //
    //        testAdjacencyListEmergingSchema_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    //        writeQueryDurationsToFile("Q5_Query_BI18_AdjacencyListEmergingSchema_0_", before_fetch, after_fetch, after_query);
    //
    //        testCSREmergingSchema_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    //        writeQueryDurationsToFile("Q5_Query_BI18_CSREmergingSchema_0_", before_fetch, after_fetch, after_query);
    //
    //
    //        writeQueryResultToFile("Q5_QueryResult_BI18_0", "messageCount, personCount", resultSet);
    //    }
    //
    //
    //
    //
    //    /*
    //     * (Q6) Which data structure performs better in full scanning the graph topology?
    //     */
    //
    //    {
    //        queryResultSet_vec resultSet;
    //
    //        //loading data from all files -- filesToLoad = 0
    //
    //        //testAdjacencyMatrix_Query_DC(1000, iterations, before_fetch, after_fetch, after_query, resultSet);
    //        //writeQueryResultsToFile("Q6_Query_DC_AdjacencyMatrix_0_", before_fetch, after_fetch, after_query);
    //
    //        testAdjacencyList_Query_DC(1000, iterations, before_fetch, after_fetch, after_query, resultSet);
    //        writeQueryDurationsToFile("Q6_Query_DC_AdjacencyList_0_", before_fetch, after_fetch, after_query);
    //
    //        testCSR_Query_DC(1000, iterations, before_fetch, after_fetch, after_query, resultSet);
    //        writeQueryDurationsToFile("Q6_Query_DC_CSR_0_", before_fetch, after_fetch, after_query);
    //
    //
    //        writeQueryResultToFile("Q6_QueryResult_DC_0", "EdgeDirection, EdgeLabel, EdgeCount, VertexCount", resultSet);
    //    }




    return (EXIT_SUCCESS);
}


/*
 * todo list:
 * ----------
 * size in bytes (done)
 * groupBy (done)
 * query: bi 18 (done) -- AdjMatSHashed, AdjMatEs
 * query: histogram (done)
 * 
 */
