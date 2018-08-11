#ifndef AGGREGATION_HPP
#define AGGREGATION_HPP

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>

class Aggregation {
private:

    //compare two vectors of strings

    static int compareStringVectors(std::vector<std::string> left, std::vector<std::string> right) {
        //equal
        int result = 0;
        for (int i = 0; i < left.size(); i++) {
            if (left[i] < right[i]) {
                //less than
                result = -1;
                break;
            } else if (left[i] > right[i]) {
                //greater than
                result = 1;
                break;
            }
        }
        return result;
    }


public:

    static void computePercentile(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& keyValues, uint16_t PercentileColumnIndex) {
        double total = 0;
        for (uint64_t i = 0; i < keyValues.size(); i++) {
            total += keyValues[i].second[PercentileColumnIndex];
        }

        for (uint64_t i = 0; i < keyValues.size(); i++) {
            keyValues[i].second.emplace_back((keyValues[i].second[PercentileColumnIndex] / total) * 100);
        }
    }

    /* l is for left index and r is right index of the
       sub-array of keyValues to be sorted */
    static void GroupBy(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& keyValues, std::vector<uint8_t>& groupByOperation) {
        /*
         * group by operations
         * 0 count, 1 sum, 2 average
         */
        keyValues.begin()->second.resize(keyValues.begin()->second.size() + groupByOperation.size(), 0);

        //std::vector<std::pair<std::vector<std::string>, std::vector<std::string> > >::iterator it;
        uint64_t groupingRecord = 0;

        for (uint64_t i = 0; i < keyValues.size(); i++) {
            if (compareStringVectors(keyValues[i].first, keyValues[groupingRecord].first) != 0) {
                keyValues[groupingRecord].second.erase(keyValues[groupingRecord].second.begin());
                groupingRecord++;
                keyValues[groupingRecord].second.resize(keyValues[groupingRecord].second.size() + groupByOperation.size());
                keyValues[groupingRecord].first = keyValues[i].first;
                for (int g = 0; g < groupByOperation.size(); g++) {
                    if (g == 0) {
                        keyValues[groupingRecord].second[groupByOperation[g] + 1] = 1;
                    } else if (g == 1) {
                        keyValues[groupingRecord].second[groupByOperation[g] + 1] = keyValues[i].second[0];
                    } else if (g == 2) {
                        keyValues[groupingRecord].second[groupByOperation[g] + 1] = keyValues[i].second[0];
                    }
                }
            } else {
                for (int g = 0; g < groupByOperation.size() && groupByOperation[g] <= 2; g++) {
                    if (g == 0) {
                        keyValues[groupingRecord].second[groupByOperation[g] + 1]++;
                    } else if (g == 1) {
                        keyValues[groupingRecord].second[groupByOperation[g] + 1] += keyValues[i].second[0];
                    } else if (g == 2) {
                        keyValues[groupingRecord].second[groupByOperation[g] + 1] = keyValues[groupingRecord].second[groupByOperation[1] + 1] / keyValues[groupingRecord].second[groupByOperation[0] + 1];
                    }
                }
            }
        }
        keyValues[groupingRecord].second.erase(keyValues[groupingRecord].second.begin());
        keyValues.erase(keyValues.begin() + groupingRecord + 1, keyValues.end());
        keyValues.shrink_to_fit();
    }
};

#endif /* AGGREGATION_HPP */