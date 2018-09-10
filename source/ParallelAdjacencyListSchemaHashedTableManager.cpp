/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ParallelAdjacencyListSchemaHashedTableManager.hpp"

bool ParallelAdjacencyListSchemaHashedTableManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory) {
    std::vector<std::thread> threads;

    DIR *pdir = NULL;
    struct dirent *pent = NULL;

    if (this->propertiesLoad) {
        pdir = opendir(verticesDirectory.data());

        while ((pent = readdir(pdir)) != NULL) {

            if (std::string(pent->d_name).empty() || std::string(pent->d_name).at(0) == '.') {
                continue;
            }
            std::cout << verticesDirectory + "/" + pent->d_name << std::endl;
            std::string vertexFileName = verticesDirectory + "/" + pent->d_name;
            std::ifstream vertexFile(verticesDirectory + "/" + pent->d_name);

            std::string headerLine;
            std::getline(vertexFile, headerLine);

            std::map<int, std::string> propertiesPositions;
            if (this->propertiesLoad) {
                propertiesPositions = this->getVertexProperties(headerLine);
            }

            std::istringstream iss(pent->d_name);
            std::string vertexType;
            getline(iss, vertexType, '_');


            for (int i = 0; i < this->parallelismDegree; i++) {
                threads.push_back(std::thread([i, &vertexFileName, &propertiesPositions, &vertexType, this]() {
                    this->loadVertices(vertexFileName, propertiesPositions, vertexType, i);
                }));
            }


            for (auto& thread : threads) {
                thread.join();
            }
            threads.clear();
        }

        closedir(pdir);
    }


    pdir = opendir(edgesDirectory.data());

    while ((pent = readdir(pdir)) != NULL) {
        if (std::string(pent->d_name).empty() || std::string(pent->d_name).at(0) == '.') {
            continue;
        }
        
        std::cout << edgesDirectory + "/" + pent->d_name << std::endl;
        std::string edgeFilePath = edgesDirectory + "/" + pent->d_name;
        std::ifstream edgeFile(edgesDirectory + "/" + pent->d_name);

        std::string headerLine;
        std::getline(edgeFile, headerLine);

        std::map<int, std::string> propertiesPositions = this->getEdgeProperties(headerLine);

        std::string edgeFileName = pent->d_name;

        for (int i = 0; i < this->parallelismDegree; i++) {
            threads.push_back(std::thread([i, &edgeFilePath, &propertiesPositions, &edgeFileName, this]() {
                this->loadEdges(edgeFilePath, propertiesPositions, edgeFileName, i);
            }));
        }


        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();
    }

    closedir(pdir);

    delete pent;

    std::cout << "==========================================================================" << std::endl;
    std::cout << "Parallel Adjacency List Size: " << this->parallelAdjacencyList.getAdjacencyListSize() << std::endl;
    std::cout << "Parallel Vertex Schema Hashed Table Size: " << this->parallelSchemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
    std::cout << "Parallel Edge Schema Hashed Table Size: " << this->parallelSchemaHashedTable.getEdgeSchemaHashedTableSize() << std::endl;
    std::cout << "==========================================================================" << std::endl;


    return true;

}

bool ParallelAdjacencyListSchemaHashedTableManager::loadVertices(std::string &vertexFileName, std::map<int, std::string> &propertiesPositions, std::string &vertexType, int threadId) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;


    std::ifstream vertexFile(vertexFileName);

    std::string headerLine;
    std::getline(vertexFile, headerLine);

    std::vector < std::pair<std::string, std::unordered_map<std::string, char*> > > vertexSchemaHashedMap;

    rowCount = 0;

    std::unordered_map<std::string, char*> properties;

    std::string vertexLine;

    while (std::getline(vertexFile, vertexLine)) {
        rowCount++;
        if (rowCount % this->parallelismDegree != threadId) {
            continue;
        }

        std::istringstream iss(vertexLine);
        std::string property, vertexOriginalId;

        int propertyCounter = 0;

        while (getline(iss, property, '|')) {
            if (propertiesPositions[propertyCounter] == "id") {
                vertexOriginalId = property;
                propertyCounter++;
                continue;
            }
            char * propertyVal = new char [property.length() + 1];
            strcpy(propertyVal, property.c_str());
            properties[propertiesPositions[propertyCounter++]] = propertyVal;
        }
        vertexSchemaHashedMap.emplace_back(vertexType + "_" + vertexOriginalId, properties);

        if (++loadCounter % batchSize == 0) {

            this->parallelSchemaHashedTable.upsertVertex(vertexSchemaHashedMap);
            vertexSchemaHashedMap.clear();

        }
    }

    this->parallelSchemaHashedTable.upsertVertex(vertexSchemaHashedMap);
    vertexSchemaHashedMap.clear();

    return true;
}

bool ParallelAdjacencyListSchemaHashedTableManager::loadEdges(std::string &edgeFilePath, std::map<int, std::string> &propertiesPositions, std::string &edgeFileName, int threadId) {

    std::ifstream edgeFile(edgeFilePath);

    std::string headerLine;
    std::getline(edgeFile, headerLine);

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;


    std::vector< std::pair<std::string, std::unordered_map<std::string, char*> > > edgeSchemaHashedMap;
    std::map<std::string, std::set<std::string> > edges;

    std::vector< std::pair< std::string, std::unordered_map<std::string, char*> > > vertexSchemaHashedMap;
    std::unordered_map<std::string, char*> vertexProperties;

    std::unordered_map<std::string, char*> properties;

    std::istringstream iss(edgeFileName);
    std::string sourceVertex, edgeLabel, targetVertex;
    getline(iss, sourceVertex, '_');
    getline(iss, edgeLabel, '_');
    getline(iss, targetVertex, '_');

    std::string edgeLine;

    while (std::getline(edgeFile, edgeLine)) {
        rowCount++;

        if (rowCount % this->parallelismDegree != threadId) {
            continue;
        }

        std::istringstream iss(edgeLine);
        std::string property, sourceVertexId, targetVertexId;

        uint64_t propertyCounter = 0;
        while (getline(iss, property, '|')) {
            if (propertyCounter == 0) {
                sourceVertexId = property;
                propertyCounter++;
            } else if (propertyCounter == 1) {
                targetVertexId = property;
                propertyCounter++;
            } else if (this->propertiesLoad) {
                char * propertyVal = new char [property.length() + 1];
                strcpy(propertyVal, property.c_str());
                properties[propertiesPositions[propertyCounter++]] = propertyVal;
            } else {
                break;
            }
        }

        if (this->propertiesLoad) {
            vertexSchemaHashedMap.emplace_back(sourceVertex + "_" + sourceVertexId, vertexProperties);
            vertexSchemaHashedMap.emplace_back(targetVertex + "_" + targetVertexId, vertexProperties);
            edgeSchemaHashedMap.emplace_back(sourceVertex + "_" + sourceVertexId + "$" + edgeLabel + "$" + targetVertex + "_" + targetVertexId, properties);
        }

        if (this->topologyLoad) {
            edges[sourceVertex + "_" + sourceVertexId].emplace(targetVertex + "_" + targetVertexId);
        }

        if (++loadCounter % batchSize == 0) {

            if (this->propertiesLoad) {
                this->parallelSchemaHashedTable.upsertVertex(vertexSchemaHashedMap);
                vertexSchemaHashedMap.clear();

                this->parallelSchemaHashedTable.upsertEdge(edgeSchemaHashedMap);
                edgeSchemaHashedMap.clear();
            }

            if (this->topologyLoad) {
                this->parallelAdjacencyList.addNeighbourVertex(edgeLabel, edges);
                edges.clear();
            }

        }
    }

    if (this->propertiesLoad) {
        this->parallelSchemaHashedTable.upsertVertex(vertexSchemaHashedMap);
        vertexSchemaHashedMap.clear();

        this->parallelSchemaHashedTable.upsertEdge(edgeSchemaHashedMap);
        edgeSchemaHashedMap.clear();
    }

    if (this->topologyLoad) {
        this->parallelAdjacencyList.addNeighbourVertex(edgeLabel, edges);
        edges.clear();
    }

    return true;
}

std::map<int, std::string> ParallelAdjacencyListSchemaHashedTableManager::getVertexProperties(std::string vertexHeaderLine) {
    std::map<int, std::string> propertiesPositions;
    std::istringstream iss(vertexHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        propertiesPositions[propertiesPositions.size()] = propertyName;
    }
    return propertiesPositions;
}

std::map<int, std::string> ParallelAdjacencyListSchemaHashedTableManager::getEdgeProperties(std::string edgeHeaderLine) {
    std::map<int, std::string> propertiesPositions;
    std::istringstream iss(edgeHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        propertiesPositions[propertiesPositions.size()] = propertyName;
    }
    return propertiesPositions;
}
