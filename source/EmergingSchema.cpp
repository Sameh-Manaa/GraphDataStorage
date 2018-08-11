/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"EmergingSchema.hpp"

EmergingSchema::EmergingSchema() {
}

EmergingSchema::~EmergingSchema() {
    for (auto const& vertexTable : this->vertexEmergingSchema) {
        for (auto const& vertex : vertexTable.second) {
            for (char* vertexProperty : vertex.second) {
                delete[] vertexProperty;
            }
        }
    }


    for (auto const& edgeTable : this->edgeEmergingSchema) {
        for (auto const& edge : edgeTable.second) {
            for (char* edgeProperty : edge.second) {
                delete[] edgeProperty;
            }
        }
    }

    this->vertexEmergingSchema.clear();
    this->vertexPropertyEsIndex.clear();
    this->edgeEmergingSchema.clear();
    this->edgePropertyEsIndex.clear();
}

std::unordered_map<std::string, std::pair<uint16_t, uint16_t> > EmergingSchema::getVertexPropertyIndex() {
    return this->vertexPropertyEsIndex;
}

std::unordered_map<std::string, std::pair<uint16_t, uint16_t> > EmergingSchema::getEdgePropertyIndex() {
    return this->edgePropertyEsIndex;
}

uint64_t EmergingSchema::getVertexEmergingSchemaSize() {
    return this->vertexEmergingSchema.size();
}

uint64_t EmergingSchema::getEdgeEmergingSchemaSize() {
    return this->edgeEmergingSchema.size();
}

void EmergingSchema::generateVerticesEmergingSchema(UniversalTable &universalTable) {

    //generate ES vertices
    srand(0);

    int total_points, total_values, k, max_iterations, has_name;

    total_points = universalTable.vertexPropertyIndex.size();

    total_values = universalTable.vertexUniversalMap.size();

    k = 8;

    max_iterations = 20;

    has_name = 1;

    vector<Point> points;

    for (auto const& propertyOrderPair : universalTable.vertexPropertyIndex) {

        vector<double> values;

        for (auto const& vertex : universalTable.vertexUniversalMap) {
            double value = vertex.second.at(propertyOrderPair.second) ? 1 : 0;
            values.push_back(value);
        }

        Point p(propertyOrderPair.second, values, propertyOrderPair.first);
        points.push_back(p);
    }

    KMeans kmeans(k, total_points, total_values, max_iterations);
    kmeans.run(points);

    int maxClusterId = 0;

    for (auto const& propertyOrderPair : universalTable.vertexPropertyIndex) {
        int clusterId = kmeans.getPointCluster(propertyOrderPair.first);

        maxClusterId = (clusterId > maxClusterId) ? clusterId : maxClusterId;

        int tablePropertyCount = 0;
        for (auto const& propertyEsOrderPair : this->vertexPropertyEsIndex) {
            if (propertyEsOrderPair.second.first == clusterId) {
                tablePropertyCount = (propertyEsOrderPair.second.second >= tablePropertyCount) ? propertyEsOrderPair.second.second + 1 : tablePropertyCount;
            }
        }
        this->vertexPropertyEsIndex[propertyOrderPair.first] = std::make_pair(clusterId, tablePropertyCount);

        std::cout << "Property(" << propertyOrderPair.first << ") --> Cluster(" << clusterId << ")" << std::endl;
    }

    for (auto const& propertyEsOrderPair : this->vertexPropertyEsIndex) {

        std::map<std::string, std::vector<char*> > vertexEsTable;
        std::unordered_map<uint16_t, std::map<std::string, std::vector<char*> > >::iterator it =
                vertexEmergingSchema.insert(std::make_pair(propertyEsOrderPair.second.first, vertexEsTable)).first;

        for (auto const& vertex : universalTable.vertexUniversalMap) {
            if (vertex.second.at(universalTable.vertexPropertyIndex.at(propertyEsOrderPair.first))) {
                std::vector<char*> vertexProperties(propertyEsOrderPair.second.second + 1);
                std::pair < std::map<std::string, std::vector<char*> >::iterator, bool> itBoolPair =
                        it->second.insert(std::make_pair(vertex.first, vertexProperties));
                if (itBoolPair.first->second.size() <= propertyEsOrderPair.second.second) {
                    itBoolPair.first->second.resize(propertyEsOrderPair.second.second + 1);
                }
                itBoolPair.first->second.at(propertyEsOrderPair.second.second) = vertex.second.at(universalTable.vertexPropertyIndex.at(propertyEsOrderPair.first));
                //if (itBoolPair.second) {
                //    itBoolPair.first->second.at(0) = vertex.second.at(universalTable.vertexPropertyIndex.at("VertexType"));
                //}
            }
        }
    }


    for (auto const& table : this->vertexEmergingSchema) {
        std::cout << "Table(" << table.first << ") --> Size(" << table.second.size() << ") --> Properties(" << table.second.begin()->second.size() << ")" << std::endl;
    }
}

void EmergingSchema::generateEdgesEmergingSchema(UniversalTable &universalTable) {

    //generate ES Edges
    srand(0);

    int total_points, total_values, k, max_iterations, has_name;

    total_points = universalTable.edgePropertyIndex.size();

    total_values = universalTable.edgeUniversalMap.size();

    k = 2;

    max_iterations = 20;

    has_name = 1;

    vector<Point> points;

    for (auto const& propertyOrderPair : universalTable.edgePropertyIndex) {
        vector<double> values;

        for (auto const& edge : universalTable.edgeUniversalMap) {
            double value = edge.second.at(propertyOrderPair.second) ? 1 : 0;
            values.push_back(value);
        }

        Point p(propertyOrderPair.second, values, propertyOrderPair.first);
        points.push_back(p);
    }

    KMeans kmeans(k, total_points, total_values, max_iterations);
    kmeans.run(points);

    int maxClusterId = 0;

    for (auto const& propertyOrderPair : universalTable.edgePropertyIndex) {
        int clusterId = kmeans.getPointCluster(propertyOrderPair.first);

        maxClusterId = (clusterId > maxClusterId) ? clusterId : maxClusterId;

        int tablePropertyCount = 0;
        for (auto const& propertyEsOrderPair : this->edgePropertyEsIndex) {
            if (propertyEsOrderPair.second.first == clusterId + 1) {
                tablePropertyCount = (propertyEsOrderPair.second.second >= tablePropertyCount) ? propertyEsOrderPair.second.second + 1 : tablePropertyCount;
            }
        }
        this->edgePropertyEsIndex[propertyOrderPair.first] = std::make_pair(clusterId + 1, tablePropertyCount);

        std::cout << "Property(" << propertyOrderPair.first << ") --> Cluster(" << clusterId + 1 << ")" << std::endl;
    }

    for (auto const& propertyEsOrderPair : this->edgePropertyEsIndex) {

        std::map<std::string, std::vector<char*> > edgeEsTable;
        std::unordered_map<uint16_t, std::map<std::string, std::vector<char*> > >::iterator it1 =
                edgeEmergingSchema.insert(std::make_pair(propertyEsOrderPair.second.first, edgeEsTable)).first;

        for (auto const& edge : universalTable.edgeUniversalMap) {

            if (edge.second.at(universalTable.edgePropertyIndex.at(propertyEsOrderPair.first))) {

                std::vector<char*> edgeProperties(propertyEsOrderPair.second.second + 1);
                std::pair < std::map<std::string, std::vector<char*> >::iterator, bool> it3 =
                        it1->second.insert(std::make_pair(edge.first, edgeProperties));

                if (it3.first->second.size() <= propertyEsOrderPair.second.second) {
                    it3.first->second.resize(propertyEsOrderPair.second.second + 1);
                }
                it3.first->second.at(propertyEsOrderPair.second.second) = edge.second.at(universalTable.edgePropertyIndex.at(propertyEsOrderPair.first));
            } else {
                bool edgeWithoutProperties = true;
                for (uint16_t i = 0; i < edge.second.size(); i++) {
                    if (edge.second.at(i)) {
                        edgeWithoutProperties = false;
                        break;
                    }
                }
                if (edgeWithoutProperties) {

                    std::map<std::string, std::vector<char*> > edgeEsTable;
                    std::unordered_map<uint16_t, std::map<std::string, std::vector<char*> > >::iterator it1 =
                            edgeEmergingSchema.insert(std::make_pair(0, edgeEsTable)).first;

                    std::vector<char*> edgeProperties;
                    std::pair < std::map<std::string, std::vector<char*> >::iterator, bool> it3 =
                            it1->second.insert(std::make_pair(edge.first, edgeProperties));
                }
            }
        }
    }


    for (auto const& table : this->edgeEmergingSchema) {
        std::cout << "Table(" << table.first << ") --> Size(" << table.second.size() << ") --> Properties(" << table.second.begin()->second.size() << ")" << std::endl;
    }
}

std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator>
EmergingSchema::getVertices(std::string vertexType, std::string propertyName) {
    std::pair<uint16_t, uint16_t> tablePropertyIndexPair = this->vertexPropertyEsIndex.at(propertyName);
    std::string vertexTypeInc = vertexType;
    char lastVertexChar = vertexType.back();
    lastVertexChar++;
    vertexTypeInc[vertexTypeInc.size() - 1] = lastVertexChar;

    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> result;
    result.first = this->vertexEmergingSchema.at(tablePropertyIndexPair.first).lower_bound(vertexType);
    result.second = this->vertexEmergingSchema.at(tablePropertyIndexPair.first).upper_bound(vertexTypeInc);
    return result;
}
