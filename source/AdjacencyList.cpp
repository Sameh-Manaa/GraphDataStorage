/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"AdjacencyList.hpp"
#include<iostream>
#include <algorithm>

AdjacencyList::AdjacencyList() {

}

uint64_t AdjacencyList::insertNode() {
    std::map<std::string, std::vector<uint64_t> > labeledNeighbourMap;
    std::vector<uint64_t> neighbourIds(0);
    labeledNeighbourMap[""] = neighbourIds;
    nodeAdjacencyMap[nextNodeId] = labeledNeighbourMap;
    nextNodeId++;

    return nextNodeId - 1;
}


bool AdjacencyList::removeNode(uint64_t nodeId) {
    if (nodeAdjacencyMap.find(nodeId) != nodeAdjacencyMap.end()) {

        for (auto& labeledNeighbourMap : nodeAdjacencyMap) {
            for (auto& nodeLabeledNeighbours : labeledNeighbourMap.second) {
                std::vector<uint64_t>::iterator it = std::find(nodeLabeledNeighbours.second.begin(), nodeLabeledNeighbours.second.end(), nodeId);
                if (it != nodeLabeledNeighbours.second.end()) {
                    nodeLabeledNeighbours.second.erase(it);
                }
            }
        }

        nodeAdjacencyMap.erase(nodeId);

        return true;
    } else {
        return false;
    }
}


bool AdjacencyList::addNeighbour(uint64_t nodeId, uint64_t neighbourId, std::string edgeLabel) {
    if (nodeAdjacencyMap.find(nodeId) != nodeAdjacencyMap.end() &&
            nodeAdjacencyMap.find(neighbourId) != nodeAdjacencyMap.end()) {
        nodeAdjacencyMap.at(nodeId)[edgeLabel].push_back(neighbourId);
        return true;
    } else {
        return false;
    }
}


bool AdjacencyList::removeNeighbour(uint64_t nodeId, uint64_t neighbourId) {
    if (nodeAdjacencyMap.find(nodeId) != nodeAdjacencyMap.end() &&
            nodeAdjacencyMap.find(neighbourId) != nodeAdjacencyMap.end()) {
        for (auto& nodeLabeledNeighbours : nodeAdjacencyMap.at(nodeId)) {
            std::vector<uint64_t>::iterator it = std::find(nodeLabeledNeighbours.second.begin(), nodeLabeledNeighbours.second.end(), neighbourId);
            if (it != nodeLabeledNeighbours.second.end()) {
                nodeLabeledNeighbours.second.erase(it);
            }
        }
        return true;
    } else {
        return false;
    }
}


bool AdjacencyList::removeNeighbour(uint64_t nodeId, uint64_t neighbourId, std::string edgeLabel) {
    if (nodeAdjacencyMap.find(nodeId) != nodeAdjacencyMap.end() &&
            nodeAdjacencyMap.find(neighbourId) != nodeAdjacencyMap.end()) {
        std::vector<uint64_t>::iterator it = std::find(nodeAdjacencyMap.at(nodeId).at(edgeLabel).begin(), nodeAdjacencyMap.at(nodeId).at(edgeLabel).end(), neighbourId);
        if (it != nodeAdjacencyMap.at(nodeId).at(edgeLabel).end()) {
            nodeAdjacencyMap.at(nodeId).at(edgeLabel).erase(it);
        }
        return true;
    } else {
        return false;
    }
}


std::map<std::string, std::vector<uint64_t> > AdjacencyList::getNeighbours(uint64_t nodeId) {
    if (nodeAdjacencyMap.find(nodeId) != nodeAdjacencyMap.end()) {
        return nodeAdjacencyMap.at(nodeId);
    } else {
        return {};
    }
}


uint64_t AdjacencyList::getNodeIndex(uint64_t nodeId) {
    return std::distance(nodeAdjacencyMap.begin(), nodeAdjacencyMap.find(nodeId));
}
