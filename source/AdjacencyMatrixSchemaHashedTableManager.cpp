#include "AdjacencyMatrixSchemaHashedTableManager.hpp"

bool AdjacencyMatrixSchemaHashedTableManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory, uint8_t filesToLoad) {
    if (loadVertices(verticesDirectory, filesToLoad) && loadEdges(edgesDirectory, filesToLoad)) {
        this->adjacencyMatrix.shrinkToFit();
        std::cout << "==========================================================================" << std::endl;
        std::cout << "Adjacency Matrix Size: " << this->adjacencyMatrix.getAdjacencyMatrixSize() << std::endl;
        std::cout << "Vertex Schema Hashed Table Size: " << this->schemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
        std::cout << "Edge Schema Hashed Table Size: " << this->schemaHashedTable.getEdgeSchemaHashedTableSize() << std::endl;
        std::cout << "==========================================================================" << std::endl;
        return true;
    } else {
        return false;
    }

}

bool AdjacencyMatrixSchemaHashedTableManager::loadVertices(std::string verticesDirectory, uint8_t filesToLoad) {
    if (!this->propertiesLoad) {
        return true;
    }

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::vector< std::pair <std::string, std::unordered_map<std::string, char*> > > vertexSchemaHashedMap;

    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    pdir = opendir(verticesDirectory.data());

    while ((pent = readdir(pdir)) != NULL) {

        std::string fileName(pent->d_name);

        if (fileName.empty() || fileName.at(0) == '.') {
            continue;
        }

        rowCount = 0;

        std::cout << verticesDirectory + "/" + pent->d_name << std::endl;
        std::ifstream vertexFile(verticesDirectory + "/" + pent->d_name);

        std::string vertexLine, headerLine;
        std::getline(vertexFile, headerLine);

        std::map<int, std::string> propertiesPositions = this->getVertexProperties(headerLine);
        std::unordered_map<std::string, char*> properties;

        std::istringstream iss(pent->d_name);
        std::string vertexType;
        getline(iss, vertexType, '_');


        while (std::getline(vertexFile, vertexLine)) {
            rowCount++;
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

                this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
                vertexSchemaHashedMap.clear();

            }
        }

        this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
        vertexSchemaHashedMap.clear();
    }

    closedir(pdir);
    return true;
}

bool AdjacencyMatrixSchemaHashedTableManager::loadEdges(std::string edgesDirectory, uint8_t filesToLoad) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;


    std::vector< std::pair<std::string, std::unordered_map<std::string, char*> > > edgeSchemaHashedMap;
    std::map<std::string, std::set<std::string> > edges;

    std::vector< std::pair< std::string, std::unordered_map<std::string, char*> > > vertexSchemaHashedMap;
    std::unordered_map<std::string, char*> vertexProperties;

    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    pdir = opendir(edgesDirectory.data());

    while ((pent = readdir(pdir)) != NULL) {

        std::string fileName(pent->d_name);

        if (fileName.empty() || fileName.at(0) == '.') {
            continue;
        }

        rowCount = 0;

        std::cout << edgesDirectory + "/" + pent->d_name << std::endl;
        std::ifstream edgeFile(edgesDirectory + "/" + pent->d_name);

        std::string edgeLine, headerLine;
        std::getline(edgeFile, headerLine);

        std::map<int, std::string> propertiesPositions;
        std::unordered_map<std::string, char*> properties;

        if (this->propertiesLoad) {
            propertiesPositions = this->getEdgeProperties(headerLine);
        }

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
                    this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
                    vertexSchemaHashedMap.clear();

                    this->schemaHashedTable.upsertEdge(edgeSchemaHashedMap);
                    edgeSchemaHashedMap.clear();
                }

                if (this->topologyLoad) {
                    this->adjacencyMatrix.addNeighbourVertex(edgeLabel, edges);
                    edges.clear();
                }

            }
        }

        if (this->propertiesLoad) {
            this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
            vertexSchemaHashedMap.clear();

            this->schemaHashedTable.upsertEdge(edgeSchemaHashedMap);
            edgeSchemaHashedMap.clear();
        }

        if (this->topologyLoad) {
            this->adjacencyMatrix.addNeighbourVertex(edgeLabel, edges);
            edges.clear();
        }
    }

    closedir(pdir);
    return true;
}

std::map<int, std::string> AdjacencyMatrixSchemaHashedTableManager::getVertexProperties(std::string vertexHeaderLine) {
    std::map<int, std::string> propertiesPositions;
    std::istringstream iss(vertexHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        propertiesPositions[propertiesPositions.size()] = propertyName;
    }
    return propertiesPositions;
}

std::map<int, std::string> AdjacencyMatrixSchemaHashedTableManager::getEdgeProperties(std::string edgeHeaderLine) {
    std::map<int, std::string> propertiesPositions;
    std::istringstream iss(edgeHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        propertiesPositions[propertiesPositions.size()] = propertyName;
    }
    return propertiesPositions;
}

void AdjacencyMatrixSchemaHashedTableManager::executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {

    // 1- get all vertices with type comment or post
    std::pair<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator, std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator> commentVertices;
    commentVertices = this->schemaHashedTable.getVertices("comment");
    std::cout << "Count of Comment Vertices Found: " << std::distance(commentVertices.first, commentVertices.second) << std::endl;

    std::pair<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator, std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator> postVertices;
    postVertices = this->schemaHashedTable.getVertices("post");
    std::cout << "Count of Post Vertices Found: " << std::distance(postVertices.first, postVertices.second) << std::endl;

    // 2- filter result on creation date before $date, length less than $messageLength, content is not empty and post language in $languages
    std::vector<std::pair<std::vector<std::string>, std::vector<double> > > tempResultSet;

    for (std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator it = commentVertices.first; it != commentVertices.second; ++it) {

        const char* content = it->second.at("content");

        std::string::size_type sz; // alias of size_t
        uint16_t length = std::stoi(it->second.at("length"), &sz);

        const char* creationDate = it->second.at("creationDate");
        tm creationDate_tm = UtilityFunctions::getDateTime(creationDate);

        if (
                (content && (content[0] != '\0')) &&
                (length < messageLength) &&
                (UtilityFunctions::compareDateTime(messageCreationDate, creationDate_tm) == 1)
                ) {

            std::pair<std::vector<std::string>, std::vector<double> > resultRecord;

            resultRecord.first.emplace_back(it->first);
            resultRecord.second.emplace_back(1);
            tempResultSet.emplace_back(resultRecord);
        }
    }

    this->adjacencyMatrix.getTargetVertex("replyOf", tempResultSet);

    std::vector<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator> replyOfPosts;
    replyOfPosts = this->schemaHashedTable.getVertices(tempResultSet);

    for (uint32_t i; i < replyOfPosts.size(); i++) {
        const char* language = replyOfPosts[i]->second.at("language");

        if (language && std::find(messageLanguages.begin(), messageLanguages.end(), std::string(language)) != messageLanguages.end()) {
            tempResultSet[i].first.erase(tempResultSet[i].first.end() - 1);
            resultSet.emplace_back(tempResultSet[i]);
        }
    }

    tempResultSet.clear();



    for (std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator it = postVertices.first; it != postVertices.second; ++it) {

        const char* language = it->second.at("language");

        const char* content = it->second.at("content");

        std::string::size_type sz; // alias of size_t
        uint16_t length = std::stoi(it->second.at("length"), &sz);

        const char* creationDate = it->second.at("creationDate");
        tm creationDate_tm = UtilityFunctions::getDateTime(creationDate);

        if (
                (content && (content[0] != '\0')) &&
                (length < messageLength) &&
                (UtilityFunctions::compareDateTime(messageCreationDate, creationDate_tm) == 1) &&
                (language && std::find(messageLanguages.begin(), messageLanguages.end(), std::string(language)) != messageLanguages.end())
                ) {

            std::pair<std::vector<std::string>, std::vector<double> > resultRecord;

            resultRecord.first.emplace_back(it->first);
            resultRecord.second.emplace_back(1);
            resultSet.emplace_back(resultRecord);
        }
    }


    this->adjacencyMatrix.getTargetVertexWithReplacement("hasCreator", resultSet);

    std::cout << "Count of Relevant Messages Found: " << resultSet.size() << std::endl;

}