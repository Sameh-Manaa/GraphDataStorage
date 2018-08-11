/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AdjacencyMatrixUniversalTableManager.hpp"

bool AdjacencyMatrixUniversalTableManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory) {
    if (loadVertices(verticesDirectory) && loadEdges(edgesDirectory)) {
        this->adjacencyMatrix.shrinkToFit();
        //        std::cout << "==========================================================================" << std::endl;
        //        std::cout << "Vertex Universal Table Size In Bytes: " << this->universalTable.getVertexUniversalTableSizeInBytes() << std::endl;
        //        std::cout << "Edge Universal Table Size In Bytes: " << this->universalTable.getEdgeUniversalTableSizeInBytes() << std::endl;
        //        std::cout << "==========================================================================" << std::endl;
        return true;
    } else {
        return false;
    }
}

bool AdjacencyMatrixUniversalTableManager::loadVertices(std::string verticesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::vector< std::pair < std::string, std::vector<char*> > > vertexUniversalMap;

    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    pdir = opendir(verticesDirectory.data());

    while ((pent = readdir(pdir)) != NULL) {
        rowCount = 0;

        std::cout << verticesDirectory + "/" + pent->d_name << std::endl;
        std::ifstream vertexFile(verticesDirectory + "/" + pent->d_name);

        std::string vertexLine, headerLine;
        std::getline(vertexFile, headerLine);

        std::vector<int16_t> propertiesPositions = this->addVertexProperties(headerLine);
        std::vector<char*> properties(universalTable.getVertexPropertyIndex().size());

        std::istringstream iss(pent->d_name);
        std::string vertexType;
        getline(iss, vertexType, '_');


        while (std::getline(vertexFile, vertexLine)) {
            rowCount++;

            std::istringstream iss(vertexLine);
            std::string property, vertexOriginalId;

            uint64_t propertyCounter = 0;

            while (getline(iss, property, '|')) {
                if (propertiesPositions[propertyCounter] == -1) {
                    vertexOriginalId = property;
                    propertyCounter++;
                    continue;
                }
                char * propertyVal = new char [property.length() + 1];
                strcpy(propertyVal, property.c_str());
                properties[propertiesPositions[propertyCounter++]] = propertyVal;
            }

            vertexUniversalMap.emplace_back(vertexType + "_" + vertexOriginalId, properties);

            if (++loadCounter % batchSize == 0) {

                this->universalTable.upsertVertex(vertexUniversalMap);
                vertexUniversalMap.clear();

            }
        }

        this->universalTable.upsertVertex(vertexUniversalMap);
        vertexUniversalMap.clear();

        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency Matrix Size: " << this->adjacencyMatrix.getAdjacencyMatrixSize() << std::endl;
        std::cout << "Vertex Universal Table Size: " << this->universalTable.getVertexUniversalTableSize() << std::endl;
        std::cout << "Edge Universal Table Size: " << this->universalTable.getEdgeUniversalTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    closedir(pdir);
    return true;
}

bool AdjacencyMatrixUniversalTableManager::loadEdges(std::string edgesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::vector< std::pair < std::string, std::vector<char*> > > vertexUniversalMap;
    std::vector<char*> vertexProperties(universalTable.getVertexPropertyIndex().size());

    std::vector< std::pair <std::string, std::vector<char*> > > edgeUniversalMap;
    std::vector<std::tuple<std::string, std::string, std::string> > edges;

    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    pdir = opendir(edgesDirectory.data());

    while ((pent = readdir(pdir)) != NULL) {
        rowCount = 0;

        std::cout << edgesDirectory + "/" + pent->d_name << std::endl;
        std::ifstream edgeFile(edgesDirectory + "/" + pent->d_name);

        std::string edgeLine, headerLine;
        std::getline(edgeFile, headerLine);

        std::vector<int16_t> propertiesPositions = this->addEdgeProperties(headerLine);
        std::vector<char*> properties(universalTable.getEdgePropertyIndex().size());

        std::istringstream iss(pent->d_name);
        std::string sourceVertex, edgeLabel, targetVertex;
        getline(iss, sourceVertex, '_');
        getline(iss, edgeLabel, '_');
        getline(iss, targetVertex, '_');


        while (std::getline(edgeFile, edgeLine)) {
            rowCount++;

            std::istringstream iss(edgeLine);
            std::string property, sourceVertexId, targetVertexId;

            uint64_t propertyCounter = 0;
            while (getline(iss, property, '|')) {
                if (propertiesPositions[propertyCounter] == -1 && propertyCounter == 0) {
                    sourceVertexId = property;
                    propertyCounter++;
                } else if (propertiesPositions[propertyCounter] == -1 && propertyCounter == 1) {
                    targetVertexId = property;
                    propertyCounter++;
                } else {
                    char * propertyVal = new char [property.length() + 1];
                    strcpy(propertyVal, property.c_str());
                    properties[propertiesPositions[propertyCounter++]] = propertyVal;
                }
            }

            vertexUniversalMap.emplace_back(sourceVertex + "_" + sourceVertexId, vertexProperties);
            vertexUniversalMap.emplace_back(targetVertex + "_" + targetVertexId, vertexProperties);

            edges.emplace_back(std::make_tuple(sourceVertex + "_" + sourceVertexId, edgeLabel, targetVertex + "_" + targetVertexId));
            edgeUniversalMap.emplace_back(sourceVertex + "_" + sourceVertexId + "$" + edgeLabel + "$" + targetVertex + "_" + targetVertexId, properties);

            if (++loadCounter % batchSize == 0) {

                this->universalTable.upsertVertex(vertexUniversalMap);
                vertexUniversalMap.clear();

                this->universalTable.upsertEdge(edgeUniversalMap);
                edgeUniversalMap.clear();

                this->adjacencyMatrix.addNeighbourVertex(edges);
                edges.clear();

            }
        }

        this->universalTable.upsertVertex(vertexUniversalMap);
        vertexUniversalMap.clear();

        this->universalTable.upsertEdge(edgeUniversalMap);
        edgeUniversalMap.clear();

        this->adjacencyMatrix.addNeighbourVertex(edges);
        edges.clear();


        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency Matrix Size: " << this->adjacencyMatrix.getAdjacencyMatrixSize() << std::endl;
        std::cout << "Vertex Universal Table Size: " << this->universalTable.getVertexUniversalTableSize() << std::endl;
        std::cout << "Edge Universal Table Size: " << this->universalTable.getEdgeUniversalTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    closedir(pdir);
    return true;
}

std::vector<int16_t> AdjacencyMatrixUniversalTableManager::addVertexProperties(std::string vertexHeaderLine) {
    std::vector<int16_t> propertiesPositions(0);
    std::istringstream iss(vertexHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        if (propertyName == "id") {
            propertiesPositions.push_back(-1);
            continue;
        }
        propertiesPositions.push_back(this->universalTable.addVertexProperty(propertyName));
    }
    return propertiesPositions;
}

std::vector<int16_t> AdjacencyMatrixUniversalTableManager::addEdgeProperties(std::string edgeHeaderLine) {
    std::vector<int16_t> propertiesPositions(0);
    std::istringstream iss(edgeHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        if (propertyName.substr(propertyName.size() - 3, 3) == ".id") {
            propertiesPositions.push_back(-1);
            continue;
        }
        propertiesPositions.push_back(this->universalTable.addEdgeProperty(propertyName));
    }
    return propertiesPositions;
}
