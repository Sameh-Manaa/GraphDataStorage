/* 
 * File:   Test.cpp
 * Author: Sameh-Manaa
 *
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


int main(int argc, char** argv) {
    std::vector<std::chrono::time_point<std::chrono::steady_clock> > before_ts, after_ts, before_fetch, after_fetch, after_query;

    const int dir_err = system("mkdir -p Evaluation-Results");
    if (-1 == dir_err) {
        printf("Error creating directory!n");
        exit(1);
    }

    for (int i = 1; i < argc; ++i) {
        cout << argv[i] << "\n";
        if (std::string(argv[i]) == "ParallelAdjacencyList") {
            for (double scaleFactor = 0.25; scaleFactor <= 4.0; scaleFactor *= 2) {
                for (int parallelismDegree = 1; parallelismDegree <= 16; parallelismDegree *= 2) {

                    before_ts.clear();
                    after_ts.clear();
                    before_ts.resize(20);
                    after_ts.resize(20);

                    std::cout << "==========================================================================" << std::endl;
                    std::cout << "Loading Start: ParallelAdjacencyList - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(1000)
                            << " - Parallelism: " << std::to_string(parallelismDegree)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    for (uint8_t i = 0; i < 20; i++) {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: ParallelAdjacencyList - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(1000)
                                << " - Parallelism: " << std::to_string(parallelismDegree)
                                << "Iteration: " << std::to_string(i) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        ParallelAdjacencyListSchemaHashedTableManager loader(1000, parallelismDegree, true, false);
                        before_ts[i] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes", "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges");
                        after_ts[i] = std::chrono::steady_clock::now();
                    }

                    ParallelAdjacencyListSchemaHashedTableManager loader(1000, parallelismDegree);
                    loader.loadGraph("..", "..");

                    std::cout << "Loading End: ParallelAdjacencyList - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(1000)
                            << " - Parallelism: " << std::to_string(parallelismDegree)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                    writeLoadingDurationsToFile("Q3_ParallelAdjacencyList_" + std::to_string(parallelismDegree) + "_"
                            + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);
                }
            }
        } else if (std::string(argv[i]) == "ParallelSchemaHashed") {
            for (double scaleFactor = 0.25; scaleFactor <= 4.0; scaleFactor *= 2) {
                for (int parallelismDegree = 1; parallelismDegree <= 16; parallelismDegree *= 2) {

                    before_ts.clear();
                    after_ts.clear();
                    before_ts.resize(20);
                    after_ts.resize(20);

                    std::cout << "==========================================================================" << std::endl;
                    std::cout << "Loading Start: ParallelSchemaHashed - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(1000)
                            << " - Parallelism: " << std::to_string(parallelismDegree)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    for (uint8_t i = 0; i < 20; i++) {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: ParallelSchemaHashed - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(1000)
                                << " - Parallelism: " << std::to_string(parallelismDegree)
                                << "Iteration: " << std::to_string(i) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        ParallelAdjacencyListSchemaHashedTableManager loader(1000, parallelismDegree, false, true);
                        before_ts[i] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes", "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges");
                        after_ts[i] = std::chrono::steady_clock::now();
                    }

                    ParallelAdjacencyListSchemaHashedTableManager loader(1000, parallelismDegree);
                    loader.loadGraph("..", "..");

                    std::cout << "Loading End: ParallelSchemaHashed - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(1000)
                            << " - Parallelism: " << std::to_string(parallelismDegree)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                    writeLoadingDurationsToFile("Q3_ParallelSchemaHashed_" + std::to_string(parallelismDegree) + "_"
                            + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_ts, after_ts);
                }
            }
        } else if (std::string(argv[i]) == "AdjacencyMatrix") {
            for (uint32_t batchSize = 1; batchSize <= 100000; batchSize *= 10) {
                before_ts.clear();
                after_ts.clear();
                before_ts.resize(20);
                after_ts.resize(20);

                std::cout << "==========================================================================" << std::endl;
                std::cout << "Loading Start: AdjacencyMatrix - Scale Factor: 0.25"
                        << " - Batch Size: " << std::to_string(batchSize)
                        << " - Iterations: " << std::to_string(20) << std::endl;
                for (uint8_t i = 0; i < 20; i++) {
                    std::cout << "==========================================================================" << std::endl;
                    std::cout << "Loading: AdjacencyMatrix - Scale Factor: 0.25"
                            << " - Batch Size: " << std::to_string(batchSize)
                            << "Iteration: " << std::to_string(i) << std::endl;
                    std::cout << "==========================================================================" << std::endl;
                    AdjacencyMatrixUniversalTableManager loader(batchSize, true, false);
                    before_ts[i] = std::chrono::steady_clock::now();
                    loader.loadGraph("data_0.25/vertexes", "data_0.25/edges");
                    after_ts[i] = std::chrono::steady_clock::now();

                    if (i == 19 && batchSize == 1000) {
                        ofstream evalFile;
                        evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

                        evalFile << "0.25, AdjacencyMatrix, " << std::to_string(loader.getAdjacencyMatrixSizeInBytes()) << endl;

                        evalFile.close();

                        before_fetch.clear();
                        after_fetch.clear();
                        after_query.clear();

                        before_fetch.resize(20);
                        after_fetch.resize(20);
                        after_query.resize(20);

                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Query: AdjacencyMatrix_Query_DC - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(20) << std::endl;
                        std::cout << "==========================================================================" << std::endl;

                        queryResultSet_vec resultSet;

                        std::vector<int> sortingDirection;
                        //index: group by operation: 0 count, 1 sum, 2 average
                        //value: storage index 0,1,2
                        std::vector<uint8_t> groupByOperation = {0};

                        for (uint8_t i = 0; i < 20; i++) {
                            std::cout << "Iteration: " << std::to_string(i) << std::endl;
                            std::cout << "==========================================================================" << std::endl;

                            resultSet.clear();

                            before_fetch[i] = std::chrono::steady_clock::now();

                            loader.executeQueryDC(resultSet);
                            after_fetch[i] = std::chrono::steady_clock::now();

                            UtilityFunctions::GroupByDcQuery(resultSet, groupByOperation);
                            UtilityFunctions::GroupBy(resultSet, groupByOperation);
                            //1 desc, -1 asc
                            sortingDirection = {-1, -1, -1};
                            MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

                            after_query[i] = std::chrono::steady_clock::now();
                        }
                        std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                        writeQueryDurationsToFile("Q6_Query_DC_AdjacencyMatrix_" + std::to_string(0.25).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                        writeQueryResultToFile("Q6_QueryResult_DC_AdjacencyMatrix_" + std::to_string(0.25).substr(0, 4) + "_1000", "EdgeDirection, EdgeLabel, EdgeCount, VertexCount", resultSet);
                    }
                }

                AdjacencyMatrixUniversalTableManager loader(batchSize);
                loader.loadGraph("..", "..");

                std::cout << "Loading End: AdjacencyMatrix - Scale Factor: 0.25"
                        << " - Batch Size: " << std::to_string(batchSize)
                        << " - Iterations: " << std::to_string(20) << std::endl;
                std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                writeLoadingDurationsToFile("Q2_AdjacencyMatrix_0.25_" + std::to_string(batchSize), before_ts, after_ts);
            }
        } else if (std::string(argv[i]) == "AdjacencyList") {
            for (double scaleFactor = 0.25; scaleFactor <= 4.0; scaleFactor *= 2) {
                uint32_t batchSize = 1000, batchSize_end = 1000;
                if (scaleFactor == 1) {
                    batchSize = 1;
                    batchSize_end = 100000;
                }
                for (; batchSize <= batchSize_end; batchSize *= 10) {
                    before_ts.clear();
                    after_ts.clear();
                    before_ts.resize(20);
                    after_ts.resize(20);

                    std::cout << "==========================================================================" << std::endl;
                    std::cout << "Loading Start: AdjacencyList - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    for (uint8_t i = 0; i < 20; i++) {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: AdjacencyList - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(batchSize)
                                << "Iteration: " << std::to_string(i) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        AdjacencyListUniversalTableManager loader(batchSize, true, false);
                        before_ts[i] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);
                        after_ts[i] = std::chrono::steady_clock::now();

                        if (i == 19 && batchSize == 1000) {
                            ofstream evalFile;
                            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

                            evalFile << std::to_string(scaleFactor).substr(0, 4) << ", AdjacencyList, " << std::to_string(loader.getAdjacencyListSizeInBytes()) << endl;

                            evalFile.close();

                            before_fetch.clear();
                            after_fetch.clear();
                            after_query.clear();

                            before_fetch.resize(20);
                            after_fetch.resize(20);
                            after_query.resize(20);

                            std::cout << "==========================================================================" << std::endl;
                            std::cout << "Query: AdjacencyList_Query_DC - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(20) << std::endl;
                            std::cout << "==========================================================================" << std::endl;

                            queryResultSet_vec resultSet;

                            std::vector<int> sortingDirection;
                            //index: group by operation: 0 count, 1 sum, 2 average
                            //value: storage index 0,1,2
                            std::vector<uint8_t> groupByOperation = {0};

                            for (uint8_t i = 0; i < 20; i++) {
                                std::cout << "Iteration: " << std::to_string(i) << std::endl;
                                std::cout << "==========================================================================" << std::endl;

                                resultSet.clear();

                                before_fetch[i] = std::chrono::steady_clock::now();

                                loader.executeQueryDC(resultSet);
                                after_fetch[i] = std::chrono::steady_clock::now();

                                UtilityFunctions::GroupByDcQuery(resultSet, groupByOperation);
                                UtilityFunctions::GroupBy(resultSet, groupByOperation);
                                //1 desc, -1 asc
                                sortingDirection = {-1, -1, -1};
                                MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

                                after_query[i] = std::chrono::steady_clock::now();
                            }
                            std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                            writeQueryDurationsToFile("Q6_Query_DC_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                            writeQueryResultToFile("Q6_QueryResult_DC_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "EdgeDirection, EdgeLabel, EdgeCount, VertexCount", resultSet);
                        }
                    }

                    AdjacencyListUniversalTableManager loader(batchSize);
                    loader.loadGraph("..", "..");

                    std::cout << "Loading End: AdjacencyList - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                    if (scaleFactor == 1) {
                        writeLoadingDurationsToFile("Q2_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);

                    }
                    if (batchSize == 1000) {
                        writeLoadingDurationsToFile("Q1_AdjacencyList_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                }
            }
        } else if (std::string(argv[i]) == "CSR") {
            //loading scalefactor 0.25, 0.50, 1.00
            for (double scaleFactor = 0.25; scaleFactor <= 1.0; scaleFactor *= 2) {
                uint32_t batchSize = 1000, batchSize_end = 1000;
                if (scaleFactor == 1) {
                    batchSize = 1;
                    batchSize_end = 100000;
                }
                for (; batchSize <= batchSize_end; batchSize *= 10) {
                    before_ts.clear();
                    after_ts.clear();
                    before_ts.resize(20);
                    after_ts.resize(20);

                    std::cout << "==========================================================================" << std::endl;
                    std::cout << "Loading Start: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    for (uint8_t i = 0; i < 20; i++) {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(batchSize)
                                << "Iteration: " << std::to_string(i) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        CSRUniversalTableManager loader(batchSize, true, false);
                        before_ts[i] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);
                        after_ts[i] = std::chrono::steady_clock::now();

                        if (i == 19 && batchSize == 1000) {
                            ofstream evalFile;
                            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

                            evalFile << std::to_string(scaleFactor).substr(0, 4) << ", CSR, " << std::to_string(loader.getCSRSizeInBytes()) << endl;

                            evalFile.close();

                            before_fetch.clear();
                            after_fetch.clear();
                            after_query.clear();

                            before_fetch.resize(20);
                            after_fetch.resize(20);
                            after_query.resize(20);

                            std::cout << "==========================================================================" << std::endl;
                            std::cout << "Query: CSR_Query_DC - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(20) << std::endl;
                            std::cout << "==========================================================================" << std::endl;

                            queryResultSet_vec resultSet;

                            std::vector<int> sortingDirection;
                            //index: group by operation: 0 count, 1 sum, 2 average
                            //value: storage index 0,1,2
                            std::vector<uint8_t> groupByOperation = {0};

                            for (uint8_t i = 0; i < 20; i++) {
                                std::cout << "Iteration: " << std::to_string(i) << std::endl;
                                std::cout << "==========================================================================" << std::endl;

                                resultSet.clear();

                                before_fetch[i] = std::chrono::steady_clock::now();

                                loader.executeQueryDC(resultSet);
                                after_fetch[i] = std::chrono::steady_clock::now();

                                UtilityFunctions::GroupByDcQuery(resultSet, groupByOperation);
                                UtilityFunctions::GroupBy(resultSet, groupByOperation);
                                //1 desc, -1 asc
                                sortingDirection = {-1, -1, -1};
                                MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

                                after_query[i] = std::chrono::steady_clock::now();
                            }
                            std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                            writeQueryDurationsToFile("Q6_Query_DC_CSR_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                            writeQueryResultToFile("Q6_QueryResult_DC_CSR_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "EdgeDirection, EdgeLabel, EdgeCount, VertexCount", resultSet);
                        }
                    }

                    CSRUniversalTableManager loader(batchSize);
                    loader.loadGraph("..", "..");

                    std::cout << "Loading End: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                    if (scaleFactor == 1) {
                        writeLoadingDurationsToFile("Q2_CSR_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                    if (batchSize == 1000) {
                        writeLoadingDurationsToFile("Q1_CSR_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                }
            }

            //loading scaleFactor 2.0
            {
                double scaleFactor = 2.0;
                uint32_t batchSize = 1000, batchSize_end = 1000;
                if (scaleFactor == 1) {
                    batchSize = 1;
                    batchSize_end = 100000;
                }
                for (; batchSize <= batchSize_end; batchSize *= 10) {
                    before_ts.clear();
                    after_ts.clear();
                    before_ts.resize(10);
                    after_ts.resize(10);

                    std::cout << "==========================================================================" << std::endl;
                    std::cout << "Loading Start: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(10) << std::endl;
                    for (uint8_t i = 0; i < 10; i++) {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(batchSize)
                                << "Iteration: " << std::to_string(i) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        CSRUniversalTableManager loader(batchSize, true, false);
                        before_ts[i] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);
                        after_ts[i] = std::chrono::steady_clock::now();

                        if (i == 9 && batchSize == 1000) {
                            ofstream evalFile;
                            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

                            evalFile << std::to_string(scaleFactor).substr(0, 4) << ", CSR, " << std::to_string(loader.getCSRSizeInBytes()) << endl;

                            evalFile.close();
                        }
                    }

                    std::cout << "Loading End: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(10) << std::endl;
                    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                    if (scaleFactor == 1) {
                        writeLoadingDurationsToFile("Q2_CSR_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                    if (batchSize == 1000) {
                        writeLoadingDurationsToFile("Q1_CSR_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                }
            }
            
            
            //loading scaleFactor 4.0
            {
                double scaleFactor = 4.0;
                uint32_t batchSize = 1000, batchSize_end = 1000;
                if (scaleFactor == 1) {
                    batchSize = 1;
                    batchSize_end = 100000;
                }
                for (; batchSize <= batchSize_end; batchSize *= 10) {
                    before_ts.clear();
                    after_ts.clear();
                    before_ts.resize(5);
                    after_ts.resize(5);

                    std::cout << "==========================================================================" << std::endl;
                    std::cout << "Loading Start: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(5) << std::endl;
                    for (uint8_t i = 0; i < 2; i++) {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(batchSize)
                                << "Iteration: " << std::to_string(i) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        CSRUniversalTableManager loader(batchSize, true, false);
                        before_ts[i] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);
                        after_ts[i] = std::chrono::steady_clock::now();

                        if (i == 0 && batchSize == 1000) {
                            ofstream evalFile;
                            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

                            evalFile << std::to_string(scaleFactor).substr(0, 4) << ", CSR, " << std::to_string(loader.getCSRSizeInBytes()) << endl;

                            evalFile.close();

                            before_fetch.clear();
                            after_fetch.clear();
                            after_query.clear();

                            before_fetch.resize(20);
                            after_fetch.resize(20);
                            after_query.resize(20);

                            std::cout << "==========================================================================" << std::endl;
                            std::cout << "Query: CSR_Query_DC - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(20) << std::endl;
                            std::cout << "==========================================================================" << std::endl;

                            queryResultSet_vec resultSet;

                            std::vector<int> sortingDirection;
                            //index: group by operation: 0 count, 1 sum, 2 average
                            //value: storage index 0,1,2
                            std::vector<uint8_t> groupByOperation = {0};

                            for (uint8_t i = 0; i < 20; i++) {
                                std::cout << "Iteration: " << std::to_string(i) << std::endl;
                                std::cout << "==========================================================================" << std::endl;

                                resultSet.clear();

                                before_fetch[i] = std::chrono::steady_clock::now();

                                loader.executeQueryDC(resultSet);
                                after_fetch[i] = std::chrono::steady_clock::now();

                                UtilityFunctions::GroupByDcQuery(resultSet, groupByOperation);
                                UtilityFunctions::GroupBy(resultSet, groupByOperation);
                                //1 desc, -1 asc
                                sortingDirection = {-1, -1, -1};
                                MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

                                after_query[i] = std::chrono::steady_clock::now();
                            }
                            std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                            writeQueryDurationsToFile("Q6_Query_DC_CSR_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                            writeQueryResultToFile("Q6_QueryResult_DC_CSR_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "EdgeDirection, EdgeLabel, EdgeCount, VertexCount", resultSet);
                        }
                    }

                    //loading full CSRUniversalTableManager for executing BI18
                    {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(batchSize)
                                << "Iteration: " << std::to_string(2) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        CSRUniversalTableManager loader(batchSize, true, false);
                        before_ts[2] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);
                        after_ts[2] = std::chrono::steady_clock::now();

                        loader.topologyLoad = false;
                        loader.propertiesLoad = true;

                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);

                        before_fetch.clear();
                        after_fetch.clear();
                        after_query.clear();

                        before_fetch.resize(20);
                        after_fetch.resize(20);
                        after_query.resize(20);

                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Query: CSRUniversalTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(20) << std::endl;
                        std::cout << "==========================================================================" << std::endl;

                        queryResultSet_vec resultSet;
                        tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-22T00:00:00");
                        uint16_t messageLength = 20;
                        std::vector<std::string> messageLanguages = {"ar"};

                        std::vector<int> sortingDirection = {-1};
                        //index: group by operation: 0 count, 1 sum, 2 average
                        //value: storage index 0,1,2
                        std::vector<uint8_t> groupByOperation = {0};

                        for (uint8_t i = 0; i < 20; i++) {
                            std::cout << "Iteration: " << std::to_string(i) << std::endl;
                            std::cout << "==========================================================================" << std::endl;

                            resultSet.clear();

                            before_fetch[i] = std::chrono::steady_clock::now();

                            loader.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
                            after_fetch[i] = std::chrono::steady_clock::now();

                            UtilityFunctions::GroupBy(resultSet, groupByOperation);
                            UtilityFunctions::GroupByValue(resultSet, groupByOperation);
                            //1 desc, -1 asc
                            sortingDirection = {1, 1};
                            MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

                            after_query[i] = std::chrono::steady_clock::now();
                        }
                        std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                        writeQueryDurationsToFile("Q5_Query_BI18_CSRUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                        writeQueryResultToFile("Q5_QueryResult_BI18_CSRUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);
                    }

                    //loading full CSRSchemaHashedTableManager for executing BI18
                    {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(batchSize)
                                << "Iteration: " << std::to_string(3) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        CSRSchemaHashedTableManager loader(batchSize, true, false);
                        before_ts[3] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);
                        after_ts[3] = std::chrono::steady_clock::now();

                        loader.topologyLoad = false;
                        loader.propertiesLoad = true;

                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);

                        before_fetch.clear();
                        after_fetch.clear();
                        after_query.clear();

                        before_fetch.resize(20);
                        after_fetch.resize(20);
                        after_query.resize(20);

                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Query: CSRSchemaHashedTable_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(20) << std::endl;
                        std::cout << "==========================================================================" << std::endl;

                        queryResultSet_vec resultSet;
                        tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-22T00:00:00");
                        uint16_t messageLength = 20;
                        std::vector<std::string> messageLanguages = {"ar"};

                        std::vector<int> sortingDirection = {-1};
                        //index: group by operation: 0 count, 1 sum, 2 average
                        //value: storage index 0,1,2
                        std::vector<uint8_t> groupByOperation = {0};

                        for (uint8_t i = 0; i < 20; i++) {
                            std::cout << "Iteration: " << std::to_string(i) << std::endl;
                            std::cout << "==========================================================================" << std::endl;

                            resultSet.clear();

                            before_fetch[i] = std::chrono::steady_clock::now();

                            loader.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
                            after_fetch[i] = std::chrono::steady_clock::now();

                            UtilityFunctions::GroupBy(resultSet, groupByOperation);
                            UtilityFunctions::GroupByValue(resultSet, groupByOperation);
                            //1 desc, -1 asc
                            sortingDirection = {1, 1};
                            MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

                            after_query[i] = std::chrono::steady_clock::now();
                        }
                        std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                        writeQueryDurationsToFile("Q5_Query_BI18_CSRSchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                        writeQueryResultToFile("Q5_QueryResult_BI18_CSRSchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);
                    }

                    //loading full CSREmergingSchemaManager for executing BI18
                    {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(batchSize)
                                << "Iteration: " << std::to_string(4) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        CSREmergingSchemaManager loader(batchSize, true, false);
                        before_ts[4] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);
                        after_ts[4] = std::chrono::steady_clock::now();

                        loader.topologyLoad = false;
                        loader.propertiesLoad = true;

                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);

                        before_fetch.clear();
                        after_fetch.clear();
                        after_query.clear();

                        before_fetch.resize(20);
                        after_fetch.resize(20);
                        after_query.resize(20);

                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Query: CSREmergingSchema_Query_BI18 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(20) << std::endl;
                        std::cout << "==========================================================================" << std::endl;

                        queryResultSet_vec resultSet;
                        tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-22T00:00:00");
                        uint16_t messageLength = 20;
                        std::vector<std::string> messageLanguages = {"ar"};

                        std::vector<int> sortingDirection = {-1};
                        //index: group by operation: 0 count, 1 sum, 2 average
                        //value: storage index 0,1,2
                        std::vector<uint8_t> groupByOperation = {0};

                        for (uint8_t i = 0; i < 20; i++) {
                            std::cout << "Iteration: " << std::to_string(i) << std::endl;
                            std::cout << "==========================================================================" << std::endl;

                            resultSet.clear();

                            before_fetch[i] = std::chrono::steady_clock::now();

                            loader.executeQueryBI18(messageCreationDate, messageLength, messageLanguages, resultSet);
                            after_fetch[i] = std::chrono::steady_clock::now();

                            UtilityFunctions::GroupBy(resultSet, groupByOperation);
                            UtilityFunctions::GroupByValue(resultSet, groupByOperation);
                            //1 desc, -1 asc
                            sortingDirection = {1, 1};
                            MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection, true, true);

                            after_query[i] = std::chrono::steady_clock::now();
                        }
                        std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                        writeQueryDurationsToFile("Q5_Query_BI18_CSREmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                        writeQueryResultToFile("Q5_QueryResult_BI18_CSREmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);
                    }

                    std::cout << "Loading End: CSR - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(10) << std::endl;
                    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                    if (scaleFactor == 1) {
                        writeLoadingDurationsToFile("Q2_CSR_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                    if (batchSize == 1000) {
                        writeLoadingDurationsToFile("Q1_CSR_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                }
            }
        } else if (std::string(argv[i]) == "UniversalTable") {
            for (double scaleFactor = 0.25; scaleFactor <= 4.0; scaleFactor *= 2) {
                uint32_t batchSize = 1000, batchSize_end = 1000;
                if (scaleFactor == 1) {
                    batchSize = 1;
                    batchSize_end = 100000;
                }
                for (; batchSize <= batchSize_end; batchSize *= 10) {
                    before_ts.clear();
                    after_ts.clear();
                    before_ts.resize(20);
                    after_ts.resize(20);

                    std::cout << "==========================================================================" << std::endl;
                    std::cout << "Loading Start: UniversalTable - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    for (uint8_t i = 0; i < 20; i++) {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: UniversalTable - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(batchSize)
                                << "Iteration: " << std::to_string(i) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        AdjacencyListUniversalTableManager loader(batchSize, false, true);
                        before_ts[i] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);
                        after_ts[i] = std::chrono::steady_clock::now();

                        if (i == 19 && batchSize == 1000) {
                            ofstream evalFile;
                            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

                            evalFile << std::to_string(scaleFactor).substr(0, 4) << ", UniversalTable, " << std::to_string(loader.getUniversalTableSizeInBytes()) << endl;

                            evalFile.close();

                            before_fetch.clear();
                            after_fetch.clear();
                            after_query.clear();

                            before_fetch.resize(20);
                            after_fetch.resize(20);
                            after_query.resize(20);

                            std::cout << "==========================================================================" << std::endl;
                            std::cout << "Query: UniversalTable_Query_BI1 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(20) << std::endl;
                            std::cout << "==========================================================================" << std::endl;

                            queryResultSet_vec resultSet;

                            tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-21T22:00:00");

                            //1 desc, -1 asc
                            std::vector<int> sortingDirection = {1, -1, -1};
                            //index: group by operation: 0 count, 1 sum, 2 average
                            //value: storage index 0,1,2
                            std::vector<uint8_t> groupByOperation = {0, 2, 1};

                            for (uint8_t i = 0; i < 20; i++) {
                                std::cout << "Iteration: " << std::to_string(i) << std::endl;
                                std::cout << "==========================================================================" << std::endl;
                                resultSet.clear();

                                before_fetch[i] = std::chrono::steady_clock::now();

                                loader.executeQueryBI1(messageCreationDate, resultSet);
                                after_fetch[i] = std::chrono::steady_clock::now();

                                UtilityFunctions::GroupBy(resultSet, groupByOperation);
                                UtilityFunctions::computePercentile(resultSet, 0);
                                MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

                                after_query[i] = std::chrono::steady_clock::now();
                            }
                            std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;
                            writeQueryDurationsToFile("Q4_Query_BI1_UniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                            writeQueryResultToFile("Q4_QueryResult_BI1_UniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "EdgeDirection, EdgeLabel, EdgeCount, VertexCount", resultSet);
                        }
                    }

                    AdjacencyListUniversalTableManager loader(batchSize);
                    loader.loadGraph("..", "..");

                    std::cout << "Loading End: UniversalTable - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                    if (scaleFactor == 1) {
                        writeLoadingDurationsToFile("Q2_UniversalTable_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                    if (batchSize == 1000) {
                        writeLoadingDurationsToFile("Q1_UniversalTable_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                }
            }
        } else if (std::string(argv[i]) == "SchemaHashed") {
            for (double scaleFactor = 0.25; scaleFactor <= 4.0; scaleFactor *= 2) {
                uint32_t batchSize = 1000, batchSize_end = 1000;
                if (scaleFactor == 1) {
                    batchSize = 1;
                    batchSize_end = 100000;
                }
                for (; batchSize <= batchSize_end; batchSize *= 10) {
                    before_ts.clear();
                    after_ts.clear();
                    before_ts.resize(20);
                    after_ts.resize(20);

                    std::cout << "==========================================================================" << std::endl;
                    std::cout << "Loading Start: SchemaHashed - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    for (uint8_t i = 0; i < 20; i++) {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: SchemaHashed - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(batchSize)
                                << "Iteration: " << std::to_string(i) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        AdjacencyListSchemaHashedTableManager loader(batchSize, false, true);
                        before_ts[i] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);
                        after_ts[i] = std::chrono::steady_clock::now();

                        if (i == 19 && batchSize == 1000) {
                            ofstream evalFile;
                            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

                            evalFile << std::to_string(scaleFactor).substr(0, 4) << ", SchemaHashed, " << std::to_string(loader.getSchemaHashedTableSizeInBytes()) << endl;

                            evalFile.close();

                            before_fetch.clear();
                            after_fetch.clear();
                            after_query.clear();

                            before_fetch.resize(20);
                            after_fetch.resize(20);
                            after_query.resize(20);

                            std::cout << "==========================================================================" << std::endl;
                            std::cout << "Query: SchemaHashed_Query_BI1 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(20) << std::endl;
                            std::cout << "==========================================================================" << std::endl;

                            queryResultSet_vec resultSet;

                            tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-21T22:00:00");

                            //1 desc, -1 asc
                            std::vector<int> sortingDirection = {1, -1, -1};
                            //index: group by operation: 0 count, 1 sum, 2 average
                            //value: storage index 0,1,2
                            std::vector<uint8_t> groupByOperation = {0, 2, 1};

                            for (uint8_t i = 0; i < 20; i++) {
                                std::cout << "Iteration: " << std::to_string(i) << std::endl;
                                std::cout << "==========================================================================" << std::endl;
                                resultSet.clear();

                                before_fetch[i] = std::chrono::steady_clock::now();

                                loader.executeQueryBI1(messageCreationDate, resultSet);
                                after_fetch[i] = std::chrono::steady_clock::now();

                                UtilityFunctions::GroupBy(resultSet, groupByOperation);
                                UtilityFunctions::computePercentile(resultSet, 0);
                                MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

                                after_query[i] = std::chrono::steady_clock::now();
                            }
                            std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;
                            writeQueryDurationsToFile("Q4_Query_BI1_SchemaHashed_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                            writeQueryResultToFile("Q4_QueryResult_BI1_SchemaHashed_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "EdgeDirection, EdgeLabel, EdgeCount, VertexCount", resultSet);
                        }
                    }

                    AdjacencyListSchemaHashedTableManager loader(batchSize);
                    loader.loadGraph("..", "..");

                    std::cout << "Loading End: SchemaHashed - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                    if (scaleFactor == 1) {
                        writeLoadingDurationsToFile("Q2_SchemaHashed_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                    if (batchSize == 1000) {
                        writeLoadingDurationsToFile("Q1_SchemaHashed_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                }
            }
        } else if (std::string(argv[i]) == "EmergingSchema") {
            for (double scaleFactor = 0.25; scaleFactor <= 4.0; scaleFactor *= 2) {
                uint32_t batchSize = 1000, batchSize_end = 1000;
                if (scaleFactor == 1) {
                    batchSize = 1;
                    batchSize_end = 100000;
                }
                for (; batchSize <= batchSize_end; batchSize *= 10) {
                    before_ts.clear();
                    after_ts.clear();
                    before_ts.resize(20);
                    after_ts.resize(20);

                    std::cout << "==========================================================================" << std::endl;
                    std::cout << "Loading Start: EmergingSchema - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    for (uint8_t i = 0; i < 20; i++) {
                        std::cout << "==========================================================================" << std::endl;
                        std::cout << "Loading: EmergingSchema - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                                << " - Batch Size: " << std::to_string(batchSize)
                                << "Iteration: " << std::to_string(i) << std::endl;
                        std::cout << "==========================================================================" << std::endl;
                        AdjacencyListEmergingSchemaManager loader(batchSize, false, true);
                        before_ts[i] = std::chrono::steady_clock::now();
                        loader.loadGraph("data_" + std::to_string(scaleFactor).substr(0, 4) + "/vertexes",
                                "data_" + std::to_string(scaleFactor).substr(0, 4) + "/edges", 0);
                        after_ts[i] = std::chrono::steady_clock::now();

                        if (i == 19 && batchSize == 1000) {
                            ofstream evalFile;
                            evalFile.open("Evaluation-Results/SizeInBytes.csv", std::ofstream::out | std::ofstream::app);

                            evalFile << std::to_string(scaleFactor).substr(0, 4) << ", EmergingSchema, " << std::to_string(loader.getEmergingSchemaSizeInBytes()) << endl;

                            evalFile.close();

                            before_fetch.clear();
                            after_fetch.clear();
                            after_query.clear();

                            before_fetch.resize(20);
                            after_fetch.resize(20);
                            after_query.resize(20);

                            std::cout << "==========================================================================" << std::endl;
                            std::cout << "Query: EmergingSchema_Query_BI1 - Batch Size: " << std::to_string(batchSize) << " - Iterations: " << std::to_string(20) << std::endl;
                            std::cout << "==========================================================================" << std::endl;

                            queryResultSet_vec resultSet;

                            tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-21T22:00:00");

                            //1 desc, -1 asc
                            std::vector<int> sortingDirection = {1, -1, -1};
                            //index: group by operation: 0 count, 1 sum, 2 average
                            //value: storage index 0,1,2
                            std::vector<uint8_t> groupByOperation = {0, 2, 1};

                            for (uint8_t i = 0; i < 20; i++) {
                                std::cout << "Iteration: " << std::to_string(i) << std::endl;
                                std::cout << "==========================================================================" << std::endl;
                                resultSet.clear();

                                before_fetch[i] = std::chrono::steady_clock::now();

                                loader.executeQueryBI1(messageCreationDate, resultSet);
                                after_fetch[i] = std::chrono::steady_clock::now();

                                UtilityFunctions::GroupBy(resultSet, groupByOperation);
                                UtilityFunctions::computePercentile(resultSet, 0);
                                MergeSort::mergeSort(resultSet, 0, resultSet.size() - 1, sortingDirection);

                                after_query[i] = std::chrono::steady_clock::now();
                            }
                            std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;
                            writeQueryDurationsToFile("Q4_Query_BI1_EmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                            writeQueryResultToFile("Q4_QueryResult_BI1_EmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "EdgeDirection, EdgeLabel, EdgeCount, VertexCount", resultSet);
                        }
                    }

                    AdjacencyListEmergingSchemaManager loader(batchSize);
                    loader.loadGraph("..", "..");

                    std::cout << "Loading End: EmergingSchema - Scale Factor: " << std::to_string(scaleFactor).substr(0, 4)
                            << " - Batch Size: " << std::to_string(batchSize)
                            << " - Iterations: " << std::to_string(20) << std::endl;
                    std::cout << "==========================================================================" << std::endl << std::endl << std::endl << std::endl;

                    if (scaleFactor == 1) {
                        writeLoadingDurationsToFile("Q2_EmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                    if (batchSize == 1000) {
                        writeLoadingDurationsToFile("Q1_EmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) +
                                "_" + std::to_string(batchSize), before_ts, after_ts);
                    }
                }
            }
        } else if (std::string(argv[i]) == "Query_BI18") {
            queryResultSet_vec resultSet;
            tm messageCreationDate = UtilityFunctions::getDateTime("2011-07-22T00:00:00");
            uint16_t messageLength = 20;
            std::vector<std::string> messageLanguages = {"ar"};


            for (double scaleFactor = 0.25; scaleFactor <= 4.0; scaleFactor *= 4) {
                testAdjacencyListUniversalTable_Query_BI18(1000, 20, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
                writeQueryDurationsToFile("Q5_Query_BI18_AdjacencyListUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                writeQueryResultToFile("Q5_QueryResult_BI18_AdjacencyListUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);

                testAdjacencyListSchemaHashedTable_Query_BI18(1000, 20, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
                writeQueryDurationsToFile("Q5_Query_BI18_AdjacencyListSchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                writeQueryResultToFile("Q5_QueryResult_BI18_AdjacencyListSchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);

                testAdjacencyListEmergingSchema_Query_BI18(1000, 20, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
                writeQueryDurationsToFile("Q5_Query_BI18_AdjacencyListEmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                writeQueryResultToFile("Q5_QueryResult_BI18_AdjacencyListEmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);

                /*
                testCSRUniversalTable_Query_BI18(1000, 20, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
                writeQueryDurationsToFile("Q5_Query_BI18_CSRUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                writeQueryResultToFile("Q5_QueryResult_BI18_CSRUniversalTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);

                testCSRSchemaHashedTable_Query_BI18(1000, 20, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
                writeQueryDurationsToFile("Q5_Query_BI18_CSRSchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                writeQueryResultToFile("Q5_QueryResult_BI18_CSRSchemaHashedTable_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);

                testCSREmergingSchema_Query_BI18(1000, 20, before_fetch, after_fetch, after_query, messageCreationDate, messageLength, messageLanguages, resultSet, std::to_string(scaleFactor).substr(0, 4));
                writeQueryDurationsToFile("Q5_Query_BI18_CSREmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", before_fetch, after_fetch, after_query);
                writeQueryResultToFile("Q5_QueryResult_BI18_CSREmergingSchema_" + std::to_string(scaleFactor).substr(0, 4) + "_1000", "messageCount, personCount", resultSet);
                 */
            }
        }
    }

    return (EXIT_SUCCESS);
}