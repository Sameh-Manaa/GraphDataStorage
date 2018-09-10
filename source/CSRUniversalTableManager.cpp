/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CSRUniversalTableManager.hpp"

bool CSRUniversalTableManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory, uint8_t filesToLoad) {
    if (loadVertices(verticesDirectory, filesToLoad) && loadEdges(edgesDirectory, filesToLoad)) {
        std::cout << "==========================================================================" << std::endl;
        std::cout << "CSR Size: " << this->csr.getCSRSize() << std::endl;
        std::cout << "Vertex Universal Table Size: " << this->universalTable.getVertexUniversalTableSize() << std::endl;
        std::cout << "Edge Universal Table Size: " << this->universalTable.getEdgeUniversalTableSize() << std::endl;
        std::cout << "==========================================================================" << std::endl;
        return true;
    } else {
        return false;
    }
}

bool CSRUniversalTableManager::loadVertices(std::string verticesDirectory, uint8_t filesToLoad) {
    if (!this->propertiesLoad) {
        return true;
    }

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::vector< std::pair < std::string, std::vector<char*> > > vertexUniversalMap;

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
    }

    closedir(pdir);
    return true;
}

bool CSRUniversalTableManager::loadEdges(std::string edgesDirectory, uint8_t filesToLoad) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::vector < std::pair <std::string, std::vector<char*> > > edgeUniversalMap;

    std::map<std::string, std::set<std::string> > edges;

    std::vector < std::pair < std::string, std::vector<char*> > > vertexUniversalMap;
    std::vector<char*> vertexProperties;

    if (this->propertiesLoad) {
        vertexProperties.resize(universalTable.getVertexPropertyIndex().size());
    }

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

        std::vector<int16_t> propertiesPositions;
        std::vector<char*> properties;

        if (this->propertiesLoad) {
            propertiesPositions = this->addEdgeProperties(headerLine);
            properties.resize(universalTable.getEdgePropertyIndex().size());
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
                vertexUniversalMap.emplace_back(sourceVertex + "_" + sourceVertexId, vertexProperties);
                vertexUniversalMap.emplace_back(targetVertex + "_" + targetVertexId, vertexProperties);
                edgeUniversalMap.emplace_back(sourceVertex + "_" + sourceVertexId + "$" + edgeLabel + "$" + targetVertex + "_" + targetVertexId, properties);
            }

            if (this->topologyLoad) {
                edges[sourceVertex + "_" + sourceVertexId].emplace(targetVertex + "_" + targetVertexId);
            }

            if (++loadCounter % batchSize == 0) {

                if (this->propertiesLoad) {
                    this->universalTable.upsertVertex(vertexUniversalMap);
                    vertexUniversalMap.clear();

                    this->universalTable.upsertEdge(edgeUniversalMap);
                    edgeUniversalMap.clear();
                }

                if (this->topologyLoad) {
                    this->csr.addNeighbourVertex(edgeLabel, edges);
                    edges.clear();
                }
            }
        }

        if (this->propertiesLoad) {
            this->universalTable.upsertVertex(vertexUniversalMap);
            vertexUniversalMap.clear();

            this->universalTable.upsertEdge(edgeUniversalMap);
            edgeUniversalMap.clear();
        }

        if (this->topologyLoad) {
            this->csr.addNeighbourVertex(edgeLabel, edges);
            edges.clear();
        }
    }

    closedir(pdir);
    return true;
}

std::vector<int16_t> CSRUniversalTableManager::addVertexProperties(std::string vertexHeaderLine) {
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

std::vector<int16_t> CSRUniversalTableManager::addEdgeProperties(std::string edgeHeaderLine) {
    std::vector<int16_t> propertiesPositions(0);
    std::istringstream iss(edgeHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        if (propertiesPositions.size() < 2) {
            propertiesPositions.push_back(-1);
            continue;
        }
        propertiesPositions.push_back(this->universalTable.addEdgeProperty(propertyName));
    }
    return propertiesPositions;
}

void CSRUniversalTableManager::executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {

    // 1- get all vertices with type comment or post
    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> commentVertices;
    commentVertices = this->universalTable.getVertices("comment");
    std::cout << "Count of Comment Vertices Found: " << std::distance(commentVertices.first, commentVertices.second) << std::endl;

    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> postVertices;
    postVertices = this->universalTable.getVertices("post");
    std::cout << "Count of Post Vertices Found: " << std::distance(postVertices.first, postVertices.second) << std::endl;

    // 2- filter result on creation date before $date, length less than $messageLength, content is not empty and post language in $languages
    std::unordered_map<std::string, uint16_t> vertexPropertyOrder = this->universalTable.getVertexPropertyIndex();

    uint64_t creationDatePropertyIndex = vertexPropertyOrder.at("creationDate");
    uint64_t lengthPropertyIndex = vertexPropertyOrder.at("length");
    uint64_t contentPropertyIndex = vertexPropertyOrder.at("content");
    uint64_t languagePropertyIndex = vertexPropertyOrder.at("language");

    std::vector<std::pair<std::vector<std::string>, std::vector<double> > > tempResultSet;

    for (std::map<std::string, std::vector<char*> >::const_iterator it = commentVertices.first; it != commentVertices.second; ++it) {

        const char* content = it->second.at(contentPropertyIndex);

        std::string::size_type sz; // alias of size_t
        uint16_t length = std::stoi(it->second.at(lengthPropertyIndex), &sz);

        const char* creationDate = it->second.at(creationDatePropertyIndex);
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

    this->csr.getTargetVertex("replyOf", tempResultSet);

    std::vector<std::map<std::string, std::vector<char*> >::const_iterator> replyOfPosts;
    replyOfPosts = this->universalTable.getVertices(tempResultSet);

    for (uint32_t i; i < replyOfPosts.size(); i++) {
        const char* language = replyOfPosts[i]->second.at(languagePropertyIndex);

        if (language && std::find(messageLanguages.begin(), messageLanguages.end(), std::string(language)) != messageLanguages.end()) {
            tempResultSet[i].first.erase(tempResultSet[i].first.end() - 1);
            resultSet.emplace_back(tempResultSet[i]);
        }
    }

    tempResultSet.clear();



    for (std::map<std::string, std::vector<char*> >::const_iterator it = postVertices.first; it != postVertices.second; ++it) {

        const char* language = it->second.at(languagePropertyIndex);

        const char* content = it->second.at(contentPropertyIndex);

        std::string::size_type sz; // alias of size_t
        uint16_t length = std::stoi(it->second.at(lengthPropertyIndex), &sz);

        const char* creationDate = it->second.at(creationDatePropertyIndex);
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


    this->csr.getTargetVertexWithReplacement("hasCreator", resultSet);

    std::cout << "Count of Relevant Messages Found: " << resultSet.size() << std::endl;

}

void CSRUniversalTableManager::executeQueryDC(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    this->csr.getAllEdges(resultSet);
    std::cout << "Count of edges Found: " << resultSet.size() << std::endl;
}