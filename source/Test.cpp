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

void testAdjacencyMatrixUniversalTable(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts) {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: AdjacencyMatrixUniversalTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyMatrixUniversalTableManager adjMatUniTblMgr(batchSize);
        before_ts[i] = std::chrono::steady_clock::now();
        adjMatUniTblMgr.loadGraph("data/vertexes", "data/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }

    std::cout << "Loading End: AdjacencyMatrixUniversalTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;
}

void testAdjacencyListUniversalTable(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts) {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: AdjacencyListUniversalTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyListUniversalTableManager adjLstUniTblMgr(batchSize);
        before_ts[i] = std::chrono::steady_clock::now();
        adjLstUniTblMgr.loadGraph("data/vertexes", "data/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }
    std::cout << "Loading End: AdjacencyListUniversalTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testCSRUniversalTable(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts) {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: CSRUniversalTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        CSRUniversalTableManager csrUniTblMgr(batchSize);
        before_ts[i] = std::chrono::steady_clock::now();
        csrUniTblMgr.loadGraph("data/vertexes", "data/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }
    std::cout << "Loading End: CSRUniversalTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyMatrixSchemaHashedTable(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts) {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: AdjacencyMatrixSchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyMatrixSchemaHashedTableManager adjMatSHahsedTblMgr(batchSize);
        before_ts[i] = std::chrono::steady_clock::now();
        adjMatSHahsedTblMgr.loadGraph("data/vertexes", "data/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }
    std::cout << "Loading End: AdjacencyMatrixSchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyListSchemaHashedTable(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts) {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: AdjacencyListSchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyListSchemaHashedTableManager adjLstSHahsedTblMgr(batchSize);
        before_ts[i] = std::chrono::steady_clock::now();
        adjLstSHahsedTblMgr.loadGraph("data/vertexes", "data/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }
    std::cout << "Loading End: AdjacencyListSchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testCSRSchemaHashedTable(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts) {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: CSRSchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        CSRSchemaHashedTableManager csrSHashedTblMgr(batchSize);
        before_ts[i] = std::chrono::steady_clock::now();
        csrSHashedTblMgr.loadGraph("data/vertexes", "data/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }
    std::cout << "Loading End: CSRSchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyMatrixEmergingSchema(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts) {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: AdjacencyMatrixEmergingSchema - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyMatrixEmergingSchemaManager adjMatEsMgr(batchSize);
        before_ts[i] = std::chrono::steady_clock::now();
        adjMatEsMgr.loadGraph("data/vertexes", "data/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }
    std::cout << "Loading End: AdjacencyMatrixEmergingSchema - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testAdjacencyListEmergingSchema(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts) {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: AdjacencyListEmergingSchema - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        AdjacencyListEmergingSchemaManager adjLstEsMgr(batchSize);
        before_ts[i] = std::chrono::steady_clock::now();
        adjLstEsMgr.loadGraph("data/vertexes", "data/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }

    std::cout << "Loading End: AdjacencyListEmergingSchema - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testCSREmergingSchema(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts) {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: CSREmergingSchema - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        CSREmergingSchemaManager csrEsMgr(batchSize);
        before_ts[i] = std::chrono::steady_clock::now();
        csrEsMgr.loadGraph("data/vertexes", "data/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }

    std::cout << "Loading End: CSREmergingSchema - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testParallelAdjacencyListSchemaHashedTable(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_ts, std::vector<auto> &after_ts, uint8_t parallelismDegree) {
    before_ts.resize(iterations);
    after_ts.resize(iterations);
    std::cout << "==========================================================================" << std::endl;
    std::cout << "Loading Start: ParallelAdjacencyListSchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << " - Parallelism: " << std::to_string(parallelismDegree) << std::endl;
    for (uint8_t i = 0; i < iterations; i++) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Iteration: " << std::to_string(i) << std::endl;
        std::cout << "==========================================================================" << std::endl;
        ParallelAdjacencyListSchemaHashedTableManager pAdjLstSHahsedTblMgr(batchSize, parallelismDegree);
        before_ts[i] = std::chrono::steady_clock::now();
        pAdjLstSHahsedTblMgr.loadGraph("data/vertexes", "data/edges");
        after_ts[i] = std::chrono::steady_clock::now();
    }

    ParallelAdjacencyListSchemaHashedTableManager pAdjLstSHahsedTblMgr(batchSize, parallelismDegree);
    pAdjLstSHahsedTblMgr.loadGraph("..", "..");

    std::cout << "Loading End: ParallelAdjacencyListSchemaHashedTable - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << " - Parallelism: " << std::to_string(parallelismDegree) << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

}

void testUniversalTable_Query_BI1(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: UniversalTable_Query_BI1 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyListUniversalTableManager adjLstUniTblMgr(batchSize);
    adjLstUniTblMgr.loadGraph("data/vertexes", "data/edges");

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
    
    ofstream evalFile;
    evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::trunc);

    evalFile << "DataStructure, Size(Byte)" << endl;

    evalFile << "Universal Table, " << std::to_string(adjLstUniTblMgr.getUniversalTableSizeInBytes()) << endl;

    evalFile.close();

}

void testSchemaHashedTable_Query_BI1(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: SchemaHashedTable_Query_BI1 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;

    std::cout << "==========================================================================" << std::endl;

    AdjacencyListSchemaHashedTableManager adjLstSHahsedTblMgr(batchSize);
    adjLstSHahsedTblMgr.loadGraph("data/vertexes", "data/edges");

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

    
    ofstream evalFile;
    evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

    evalFile << "Schema Hashed Table, " << std::to_string(adjLstSHahsedTblMgr.getSchemaHashedTableSizeInBytes()) << endl;

    evalFile.close();
}

void testEmergingSchema_Query_BI1(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: EmergingSchema_Query_BI1 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyListEmergingSchemaManager adjLstEsMgr(batchSize);
    adjLstEsMgr.loadGraph("data/vertexes", "data/edges");

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

    
    
    ofstream evalFile;
    evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

    evalFile << "Emerging Schema, " << std::to_string(adjLstEsMgr.getEmergingSchemaSizeInBytes()) << endl;

    evalFile.close();
}

void testAdjacencyListUniversalTable_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyListUniversalTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyListUniversalTableManager adjLstUniTblMgr(batchSize);
    adjLstUniTblMgr.loadGraph("data/vertexes", "data/edges");

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

void testCSRUniversalTable_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: CSRUniversalTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    CSRUniversalTableManager csrUniTblMgr(batchSize);
    csrUniTblMgr.loadGraph("data/vertexes", "data/edges");

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

void testAdjacencyListSchemaHashedTable_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);
    std::cout << "==========================================================================" << std::endl;

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyListSchemaHashedTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;

    AdjacencyListSchemaHashedTableManager adjLstSHashedTblMgr(batchSize);
    adjLstSHashedTblMgr.loadGraph("data/vertexes", "data/edges");

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

void testCSRSchemaHashedTable_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: CSRSchemaHashedTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    CSRSchemaHashedTableManager csrSHashedTblMgr(batchSize);
    csrSHashedTblMgr.loadGraph("data/vertexes", "data/edges");

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

void testAdjacencyListEmergingSchema_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyListEmergingSchema_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyListEmergingSchemaManager adjLstEsTblMgr(batchSize);
    adjLstEsTblMgr.loadGraph("data/vertexes", "data/edges");

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

void testCSREmergingSchema_Query_BI18(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: CSREmergingSchema_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    CSREmergingSchemaManager csrEsTblMgr(batchSize);
    csrEsTblMgr.loadGraph("data/vertexes", "data/edges");

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

void testAdjacencyMatrix_Query_DC(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyMatrix_Query_DC - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyMatrixUniversalTableManager adjMatUniTblMgr(batchSize);
    adjMatUniTblMgr.loadGraph("data/vertexes", "data/edges");

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

    
    ofstream evalFile;
    evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

    evalFile << "Adjacency Matrix, " << std::to_string(adjMatUniTblMgr.getAdjacencyMatrixSizeInBytes()) << endl;

    evalFile.close();
}

void testAdjacencyList_Query_DC(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: AdjacencyList_Query_DC - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    AdjacencyListUniversalTableManager adjLstUniTblMgr(batchSize);
    adjLstUniTblMgr.loadGraph("data/vertexes", "data/edges");

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

    
    ofstream evalFile;
    evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

    evalFile << "Adjacency List, " << std::to_string(adjLstUniTblMgr.getAdjacencyListSizeInBytes()) << endl;

    evalFile.close();
}

void testCSR_Query_DC(uint32_t batchSize, uint8_t iterations, std::vector<auto> &before_fetch, std::vector<auto> &after_fetch, std::vector<auto> &after_query, queryResultSet_vec &resultSet) {
    before_fetch.resize(iterations);
    after_fetch.resize(iterations);
    after_query.resize(iterations);

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Query: CSR_Query_DC - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(iterations) << std::endl;
    std::cout << "==========================================================================" << std::endl;

    CSRUniversalTableManager csrUniTblMgr(batchSize);
    csrUniTblMgr.loadGraph("data/vertexes", "data/edges");

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

    
    
    ofstream evalFile;
    evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

    evalFile << "CSR, " << std::to_string(csrUniTblMgr.getCSRSizeInBytes()) << endl;

    evalFile.close();
}

int main(int argc, char** argv) {
    int iterations = 2;
    uint32_t batchSize_start = 1000, batchSize_end = 1000;
    std::vector<std::chrono::time_point<std::chrono::steady_clock> > before_ts, after_ts, before_fetch, after_fetch, after_query;

    const int dir_err = system("mkdir -p Evaluation-Results");
    if (-1 == dir_err) {
        printf("Error creating directory!n");
        exit(1);
    }


    for (uint32_t batchSize = batchSize_start; batchSize <= batchSize_end; batchSize *= 10) {

        testParallelAdjacencyListSchemaHashedTable(batchSize, iterations, before_ts, after_ts, 1);
        writeLoadingDurationsToFile("Loading_Parallel(1)_AdjacencyListSchemaHashedTable_" + to_string(batchSize), before_ts, after_ts);

        testParallelAdjacencyListSchemaHashedTable(batchSize, iterations, before_ts, after_ts, 2);
        writeLoadingDurationsToFile("Loading_Parallel(2)_AdjacencyListSchemaHashedTable_" + to_string(batchSize), before_ts, after_ts);

        testParallelAdjacencyListSchemaHashedTable(batchSize, iterations, before_ts, after_ts, 4);
        writeLoadingDurationsToFile("Loading_Parallel(4)_AdjacencyListSchemaHashedTable_" + to_string(batchSize), before_ts, after_ts);

        testParallelAdjacencyListSchemaHashedTable(batchSize, iterations, before_ts, after_ts, 8);
        writeLoadingDurationsToFile("Loading_Parallel(8)_AdjacencyListSchemaHashedTable_" + to_string(batchSize), before_ts, after_ts);

        testParallelAdjacencyListSchemaHashedTable(batchSize, iterations, before_ts, after_ts, 16);
        writeLoadingDurationsToFile("Loading_Parallel(16)_AdjacencyListSchemaHashedTable_" + to_string(batchSize), before_ts, after_ts);

        //testAdjacencyMatrixUniversalTable(batchSize, iterations, before_ts, after_ts);
        //writeLoadingResultsToFile("Loading_AdjacencyMatrixUniversalTable_" + to_string(batchSize), before_ts, after_ts);

        testAdjacencyListUniversalTable(batchSize, iterations, before_ts, after_ts);
        writeLoadingDurationsToFile("Loading_AdjacencyListUniversalTable_" + to_string(batchSize), before_ts, after_ts);

        testCSRUniversalTable(batchSize, iterations, before_ts, after_ts);
        writeLoadingDurationsToFile("Loading_CSRUniversalTable_" + to_string(batchSize), before_ts, after_ts);

        //testAdjacencyMatrixSchemaHashedTable(batchSize, iterations, before_ts, after_ts);
        //writeLoadingResultsToFile("Loading_AdjacencyMatrixSchemaHashedTable_" + to_string(batchSize), before_ts, after_ts);

        testAdjacencyListSchemaHashedTable(batchSize, iterations, before_ts, after_ts);
        writeLoadingDurationsToFile("Loading_AdjacencyListSchemaHashedTable_" + to_string(batchSize), before_ts, after_ts);

        testCSRSchemaHashedTable(batchSize, iterations, before_ts, after_ts);
        writeLoadingDurationsToFile("Loading_CSRSchemaHashedTable_" + to_string(batchSize), before_ts, after_ts);

        //testAdjacencyMatrixEmergingSchema(batchSize, iterations, before_ts, after_ts);
        //writeLoadingResultsToFile("Loading_AdjacencyMatrixEmergingSchema_" + to_string(batchSize), before_ts, after_ts);

        testAdjacencyListEmergingSchema(batchSize, iterations, before_ts, after_ts);
        writeLoadingDurationsToFile("Loading_AdjacencyListEmergingSchema_" + to_string(batchSize), before_ts, after_ts);

        testCSREmergingSchema(batchSize, iterations, before_ts, after_ts);
        writeLoadingDurationsToFile("Loading_CSREmergingSchema_" + to_string(batchSize), before_ts, after_ts);
    }

    //return (EXIT_SUCCESS); 

    iterations = 2;

    queryResultSet_vec resultSet;
    tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-21T22:00:00");

    testUniversalTable_Query_BI1(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, resultSet);
    writeQueryDurationsToFile("Query_BI1_UniversalTable_", before_fetch, after_fetch, after_query);

    testSchemaHashedTable_Query_BI1(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, resultSet);
    writeQueryDurationsToFile("Query_BI1_SchemaHashedTable_", before_fetch, after_fetch, after_query);

    testEmergingSchema_Query_BI1(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, resultSet);
    writeQueryDurationsToFile("Query_BI1_EmergingSchema_", before_fetch, after_fetch, after_query);


    writeQueryResultToFile("QueryResult_BI1", "year, isComment, lengthCategory, messageCount, averageMessageLength, sumMessageLength, percentageOfMessages", resultSet);




    messageCreationDate = UtilityFunctions::getDateTime("2011-07-22T00:00:00");
    uint16_t messageLength = 20;
    std::vector<std::string> messageLanguages = {"ar"};

    testAdjacencyListUniversalTable_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    writeQueryDurationsToFile("Query_BI18_AdjacencyListUniversalTable_", before_fetch, after_fetch, after_query);

    testCSRUniversalTable_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    writeQueryDurationsToFile("Query_BI18_CSRUniversalTable_", before_fetch, after_fetch, after_query);

    testAdjacencyListSchemaHashedTable_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    writeQueryDurationsToFile("Query_BI18_AdjacencyListSchemaHashedTable_", before_fetch, after_fetch, after_query);

    testCSRSchemaHashedTable_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    writeQueryDurationsToFile("Query_BI18_CSRSchemaHashedTable_", before_fetch, after_fetch, after_query);

    testAdjacencyListEmergingSchema_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    writeQueryDurationsToFile("Query_BI18_AdjacencyListEmergingSchema_", before_fetch, after_fetch, after_query);

    testCSREmergingSchema_Query_BI18(1000, iterations, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet);
    writeQueryDurationsToFile("Query_BI18_CSREmergingSchema_", before_fetch, after_fetch, after_query);


    writeQueryResultToFile("QueryResult_BI18", "messageCount, personCount", resultSet);





    //testAdjacencyMatrix_Query_DC(1000, iterations, before_fetch, after_fetch, after_query, resultSet);
    //writeQueryResultsToFile("Query_DC_AdjacencyMatrix_", before_fetch, after_fetch, after_query);

    testAdjacencyList_Query_DC(1000, iterations, before_fetch, after_fetch, after_query, resultSet);
    writeQueryDurationsToFile("Query_DC_AdjacencyList_", before_fetch, after_fetch, after_query);

    testCSR_Query_DC(1000, iterations, before_fetch, after_fetch, after_query, resultSet);
    writeQueryDurationsToFile("Query_DC_CSR_", before_fetch, after_fetch, after_query);


    writeQueryResultToFile("QueryResult_DC", "EdgeDirection, EdgeLabel, EdgeCount, VertexCount", resultSet);



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
