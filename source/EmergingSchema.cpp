/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"EmergingSchema.hpp"

EmergingSchema::EmergingSchema() {
    //this->edgeUniversalMap.max_load_factor(10);
}

uint64_t EmergingSchema::addVertexProperty(std::string propertyName) {
    bool inserted = this->vertexPropertyOrder.insert(std::make_pair(propertyName, this->vertexPropertyOrder.size())).second;

    if (inserted) {
        for (auto& vertex : this->vertexUniversalMap) {
            vertex.second.emplace_back("");
        }
    }
    return this->vertexPropertyOrder.at(propertyName);
}

uint64_t EmergingSchema::addEdgeProperty(std::string propertyName) {
    bool inserted = this->edgePropertyOrder.insert(std::make_pair(propertyName, this->edgePropertyOrder.size())).second;
    if (inserted) {
        for (auto& edge : this->edgeUniversalMap) {
            for (auto& labledEdge : edge.second) {
                labledEdge.second.push_back("");
            }
        }
    }
    return this->edgePropertyOrder.at(propertyName);
}

std::map<std::string, uint64_t> EmergingSchema::getVertexPropertyOrder() {
    return this->vertexPropertyOrder;
}

std::map<std::string, uint64_t> EmergingSchema::getEdgePropertyOrder() {
    return this->edgePropertyOrder;
}

void EmergingSchema::upsertVertex(std::string vertexId, std::vector<std::string> properties) {
    this->vertexUniversalMap[vertexId] = properties;
}

void EmergingSchema::upsertVertex(std::map<std::string, std::vector<std::string> > &vertexUniversalMap) {
    //this->vertexUniversalMap.reserve(this->vertexUniversalMap.size() + vertexUniversalMap.size());
    this->vertexUniversalMap.insert(vertexUniversalMap.begin(), vertexUniversalMap.end());
}

bool EmergingSchema::removeVertex(std::string vertexId) {
    if (this->vertexUniversalMap.erase(vertexId) == 1) {
        return true;
    } else {
        return false;
    }
}

void EmergingSchema::upsertEdge(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::vector<std::string> properties) {
    this->edgeUniversalMap[std::make_pair(sourceVertexId, targetVertexId)][edgeLabel] = properties;
}

void EmergingSchema::upsertEdge(std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > &edgeUniversalMap) {
    //this->edgeUniversalMap.reserve(this->edgeUniversalMap.size() + edgeUniversalMap.size());
    this->edgeUniversalMap.insert(edgeUniversalMap.begin(), edgeUniversalMap.end());
}

bool EmergingSchema::removeEdge(std::string sourceVertexId, std::string targetVertexId) {
    if (this->edgeUniversalMap.erase(std::make_pair(sourceVertexId, targetVertexId)) == 1) {
        return true;
    } else {
        return false;
    }
}

std::string EmergingSchema::getVertexProperty(std::string vertexId, std::string propertyName) {
    return this->vertexUniversalMap.at(vertexId).at(this->vertexPropertyOrder.at(propertyName));
}

std::vector<std::string> EmergingSchema::getVertexAllProperties(std::string vertexId) {
    return this->vertexUniversalMap.at(vertexId);
}

std::list<std::string> EmergingSchema::getQualifiedVertices(std::vector<std::string> &selectiveProperties) {
    std::list<std::string> qualifiedVertices(0);
    for (auto const& vertex : this->vertexUniversalMap) {
        for (uint64_t i = 0; i < this->vertexPropertyOrder.size(); i++) {
            if (selectiveProperties[i].empty()) {
                continue;
            } else if (selectiveProperties[i].compare(vertex.second[i]) == 0) {
                qualifiedVertices.push_back(vertex.first);
            } else {
                break;
            }
        }
    }
    return qualifiedVertices;
}

std::string EmergingSchema::getEdgeProperty(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel, std::string propertyName) {
    return this->edgeUniversalMap.at(std::make_pair(sourceVertexId, targetVertexId)).at(edgeLabel).at(this->edgePropertyOrder.at(propertyName));
}

std::vector<std::string> EmergingSchema::getEdgeAllProperties(std::string sourceVertexId, std::string targetVertexId, std::string edgeLabel) {
    return this->edgeUniversalMap.at(std::make_pair(sourceVertexId, targetVertexId)).at(edgeLabel);
}

std::list<std::pair<std::string, std::string> > EmergingSchema::getQualifiedEdges(std::vector<std::string> &selectiveProperties) {
    std::list<std::pair<std::string, std::string> > qualifiedEdges(0);
    for (auto const& edge : this->edgeUniversalMap) {
        for (auto const& labledEdge : edge.second) {
            for (uint64_t i = 0; i < this->edgePropertyOrder.size(); i++) {
                if (selectiveProperties[i].empty()) {
                    continue;
                } else if (selectiveProperties[i].compare(labledEdge.second[i]) == 0) {
                    qualifiedEdges.push_back(edge.first);
                } else {
                    break;
                }
            }
        }
    }
    return qualifiedEdges;
}

uint64_t EmergingSchema::getVertexEmergingSchemaSize() {
    return this->vertexUniversalMap.size();
}

uint64_t EmergingSchema::getEdgeEmergingSchemaSize() {
    return this->edgeUniversalMap.size();
}

void EmergingSchema::generateVerticesEmergingSchema() {

    //generate ES vertices
    srand(0);

    int total_points, total_values, k, max_iterations, has_name;

    total_points = this->vertexPropertyOrder.size() - 1;

    total_values = this->vertexUniversalMap.size();

    k = 6;

    max_iterations = 20;

    has_name = 1;

    vector<Point> points;

    for (auto const& propertyOrderPair : this->vertexPropertyOrder) {
        if (propertyOrderPair.first == "VertexType") {
            continue;
        }

        vector<double> values;

        for (auto const& vertex : this->vertexUniversalMap) {
            double value = vertex.second.at(propertyOrderPair.second).empty() ? 0 : 1;
            values.push_back(value);
        }

        Point p(propertyOrderPair.second, values, propertyOrderPair.first);
        points.push_back(p);
    }

    KMeans kmeans(k, total_points, total_values, max_iterations);
    kmeans.run(points);

    int maxClusterId = 0;

    for (auto const& propertyOrderPair : this->vertexPropertyOrder) {
        int clusterId = kmeans.getPointCluster(propertyOrderPair.first);

        maxClusterId = (clusterId > maxClusterId) ? clusterId : maxClusterId;

        if (propertyOrderPair.first != "VertexType") {
            int tablePropertyCount = 1;
            for (auto const& propertyEsOrderPair : this->vertexPropertyEsOrder) {
                if (propertyEsOrderPair.second.first == clusterId) {
                    tablePropertyCount = (propertyEsOrderPair.second.second >= tablePropertyCount) ? propertyEsOrderPair.second.second + 1 : tablePropertyCount;
                }
            }
            this->vertexPropertyEsOrder[propertyOrderPair.first] = std::make_pair(clusterId, tablePropertyCount);
        }

        std::cout << "Property(" << propertyOrderPair.first << ") --> Cluster(" << clusterId << ")" << std::endl;
    }

    for (auto const& propertyEsOrderPair : this->vertexPropertyEsOrder) {

        std::map<std::string, std::vector<std::string> > vertexEsTable;
        std::map<uint64_t, std::map<std::string, std::vector<std::string> > >::iterator it =
                vertexEmergingSchema.insert(std::make_pair(propertyEsOrderPair.second.first, vertexEsTable)).first;

        for (auto const& vertex : this->vertexUniversalMap) {
            if (!vertex.second.at(this->vertexPropertyOrder.at(propertyEsOrderPair.first)).empty()) {
                std::vector<std::string> vertexProperties(propertyEsOrderPair.second.second + 1);
                std::pair < std::map<std::string, std::vector<std::string> >::iterator, bool> itBoolPair =
                        it->second.insert(std::make_pair(vertex.first, vertexProperties));
                if (itBoolPair.first->second.size() <= propertyEsOrderPair.second.second) {
                    itBoolPair.first->second.resize(propertyEsOrderPair.second.second + 1);
                }
                itBoolPair.first->second.at(propertyEsOrderPair.second.second) = vertex.second.at(this->vertexPropertyOrder.at(propertyEsOrderPair.first));
                if (itBoolPair.second) {
                    itBoolPair.first->second.at(0) = vertex.second.at(this->vertexPropertyOrder.at("VertexType"));
                }
            }
        }
    }


    for (auto const& table : this->vertexEmergingSchema) {
        std::cout << "Table(" << table.first << ") --> Size(" << table.second.size() << ") --> Properties(" << table.second.begin()->second.size() << ")" << std::endl;
    }
}

void EmergingSchema::generateEdgesEmergingSchema() {

    //generate ES Edges
    srand(0);

    int total_points, total_values, k, max_iterations, has_name;

    total_points = this->edgePropertyOrder.size();

    total_values = this->edgeUniversalMap.size();

    k = 2;

    max_iterations = 20;

    has_name = 1;

    vector<Point> points;

    for (auto const& propertyOrderPair : this->edgePropertyOrder) {
        vector<double> values;

        for (auto const& edge : this->edgeUniversalMap) {
            for (auto const& edgeLabel : edge.second) {
                double value = edgeLabel.second.at(propertyOrderPair.second).empty() ? 0 : 1;
                values.push_back(value);
            }
        }

        Point p(propertyOrderPair.second, values, propertyOrderPair.first);
        points.push_back(p);
    }

    KMeans kmeans(k, total_points, total_values, max_iterations);
    kmeans.run(points);

    int maxClusterId = 0;

    for (auto const& propertyOrderPair : this->edgePropertyOrder) {
        int clusterId = kmeans.getPointCluster(propertyOrderPair.first);

        maxClusterId = (clusterId > maxClusterId) ? clusterId : maxClusterId;

        int tablePropertyCount = 0;
        for (auto const& propertyEsOrderPair : this->edgePropertyEsOrder) {
            if (propertyEsOrderPair.second.first == clusterId + 1) {
                tablePropertyCount = (propertyEsOrderPair.second.second >= tablePropertyCount) ? propertyEsOrderPair.second.second + 1 : tablePropertyCount;
            }
        }
        this->edgePropertyEsOrder[propertyOrderPair.first] = std::make_pair(clusterId + 1, tablePropertyCount);

        std::cout << "Property(" << propertyOrderPair.first << ") --> Cluster(" << clusterId + 1 << ")" << std::endl;
    }

    for (auto const& propertyEsOrderPair : this->edgePropertyEsOrder) {

        std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > edgeEsTable;
        std::map<uint64_t, std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > >::iterator it1 =
                edgeEmergingSchema.insert(std::make_pair(propertyEsOrderPair.second.first, edgeEsTable)).first;

        for (auto const& edge : this->edgeUniversalMap) {

            for (auto const& edgeLabeled : edge.second) {

                if (!edgeLabeled.second.at(this->edgePropertyOrder.at(propertyEsOrderPair.first)).empty()) {

                    std::map<std::string, std::vector<std::string> > edgeLabeledEsTable;
                    std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > >::iterator it2 =
                            it1->second.insert(std::make_pair(edge.first, edgeLabeledEsTable)).first;

                    std::vector<std::string> edgeProperties(propertyEsOrderPair.second.second + 1);
                    std::pair < std::map<std::string, std::vector<std::string> >::iterator, bool> it3 =
                            it2->second.insert(std::make_pair(edgeLabeled.first, edgeProperties));

                    if (it3.first->second.size() <= propertyEsOrderPair.second.second) {
                        it3.first->second.resize(propertyEsOrderPair.second.second + 1);
                    }
                    it3.first->second.at(propertyEsOrderPair.second.second) = edgeLabeled.second.at(this->edgePropertyOrder.at(propertyEsOrderPair.first));
                } else {
                    bool edgeWithoutProperties = true;
                    for (uint64_t i=0; i < edgeLabeled.second.size(); i++) {
                        if (!edgeLabeled.second.at(i).empty()) {
                            edgeWithoutProperties = false;
                            break;
                        }
                    }
                    if (edgeWithoutProperties) {

                        std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > edgeEsTable;
                        std::map<uint64_t, std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > >::iterator it1 =
                                edgeEmergingSchema.insert(std::make_pair(0, edgeEsTable)).first;

                        std::map<std::string, std::vector<std::string> > edgeLabeledEsTable;
                        std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > >::iterator it2 =
                                it1->second.insert(std::make_pair(edge.first, edgeLabeledEsTable)).first;

                        std::vector<std::string> edgeProperties;
                        std::pair < std::map<std::string, std::vector<std::string> >::iterator, bool> it3 =
                                it2->second.insert(std::make_pair(edgeLabeled.first, edgeProperties));
                    }
                }
            }
        }
    }


    for (auto const& table : this->edgeEmergingSchema) {
        std::cout << "Table(" << table.first << ") --> Size(" << table.second.size() << ") --> Labels(" << table.second.begin()->second.size() << ") --> Properties(" << table.second.begin()->second.begin()->second.size() << ")" << std::endl;
    }
}

void EmergingSchema::clearUniversalTable(){
    this->vertexUniversalMap.clear();
    this->edgeUniversalMap.clear();
}