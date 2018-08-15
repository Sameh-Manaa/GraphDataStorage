#ifndef UTILITYFUNCTIONS_HPP
#define UTILITYFUNCTIONS_HPP

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <boost/functional/hash.hpp>

class UtilityFunctions {
private:

    template <typename Container> // we can make this generic for any container [1]
    struct container_hash {

        std::size_t operator()(Container const& c) const {
            return boost::hash_range(c.begin(), c.end());
        }
    };
public:

    //compare DateTime

    static int compareDateTime(tm &leftDateTime, tm &rightDateTime) {
        //result: 1 greater than, 0 equal, -1 less than
        int result = -1;
        if (leftDateTime.tm_year > rightDateTime.tm_year ||
                (leftDateTime.tm_year == rightDateTime.tm_year && leftDateTime.tm_mon > rightDateTime.tm_mon) ||
                (leftDateTime.tm_year == rightDateTime.tm_year && leftDateTime.tm_mon == rightDateTime.tm_mon && leftDateTime.tm_mday > rightDateTime.tm_mday) ||
                (leftDateTime.tm_year == rightDateTime.tm_year && leftDateTime.tm_mon == rightDateTime.tm_mon && leftDateTime.tm_mday == rightDateTime.tm_mday && leftDateTime.tm_hour > rightDateTime.tm_hour) ||
                (leftDateTime.tm_year == rightDateTime.tm_year && leftDateTime.tm_mon == rightDateTime.tm_mon && leftDateTime.tm_mday == rightDateTime.tm_mday && leftDateTime.tm_hour == rightDateTime.tm_hour && leftDateTime.tm_min > rightDateTime.tm_min) ||
                (leftDateTime.tm_year == rightDateTime.tm_year && leftDateTime.tm_mon == rightDateTime.tm_mon && leftDateTime.tm_mday == rightDateTime.tm_mday && leftDateTime.tm_hour == rightDateTime.tm_hour && leftDateTime.tm_min == rightDateTime.tm_min && leftDateTime.tm_sec > rightDateTime.tm_sec)) {
            result = 1;
        } else if (leftDateTime.tm_year == rightDateTime.tm_year &&
                leftDateTime.tm_mon == rightDateTime.tm_mon &&
                leftDateTime.tm_mday == rightDateTime.tm_mday &&
                leftDateTime.tm_hour == rightDateTime.tm_hour &&
                leftDateTime.tm_min == rightDateTime.tm_min &&
                leftDateTime.tm_sec == rightDateTime.tm_sec) {
            result = 0;
        }
        return result;
    }

    static tm getDateTime(const char* dateTimeString) {
        tm tm1;
        sscanf(dateTimeString, "%4d-%2d-%2dT%2d:%2d:%2d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday, &tm1.tm_hour, &tm1.tm_min, &tm1.tm_sec);
        return tm1;
    }

    //compare two vectors of strings

    static int CompareVectors(std::vector<std::string>& left, std::vector<std::string>& right) {
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

    static int CompareVectors(std::vector<double>& left, std::vector<double>& right) {
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
    static void GroupByValue(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& keyValues, std::vector<uint8_t>& groupByOperation) {
        /*
         * group by operations
         * 0 count, 1 sum, 2 average
         */
        keyValues.begin()->second.resize(keyValues.begin()->second.size() + groupByOperation.size(), 0);

        //std::vector<std::pair<std::vector<std::string>, std::vector<std::string> > >::iterator it;
        uint64_t groupingRecord = 0;

        std::vector<double> groupingVector = keyValues.begin()->second;

        for (uint64_t i = 0; i < keyValues.size(); i++) {
            if (CompareVectors(keyValues[i].second, groupingVector) != 0) {
                keyValues[groupingRecord].first.clear();
                groupingRecord++;
                groupingVector = keyValues[i].second;
                keyValues[groupingRecord].second = keyValues[i].second;
                keyValues[groupingRecord].second.resize(keyValues[groupingRecord].second.size() + groupByOperation.size(), 0);
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
        keyValues[groupingRecord].first.clear();
        keyValues.erase(keyValues.begin() + groupingRecord + 1, keyValues.end());
    }

    static void GroupBy(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& keyValues, std::vector<uint8_t>& groupByOperation) {
        /*
         * group by operations
         * 0 count, 1 sum, 2 average
         */
        keyValues.begin()->second.resize(keyValues.begin()->second.size() + groupByOperation.size(), 0);

        //std::vector<std::pair<std::vector<std::string>, std::vector<std::string> > >::iterator it;
        uint64_t groupingRecord = 0;

        for (uint64_t i = 0; i < keyValues.size(); i++) {
            if (CompareVectors(keyValues[i].first, keyValues[groupingRecord].first) != 0) {
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
    }

    static void GroupByDcQuery(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& keyValues, std::vector<uint8_t>& groupByOperation) {

        std::unordered_map<std::vector<std::string>, uint32_t, container_hash<std::vector<std::string> > > tempGrouppingMap;
        tempGrouppingMap.reserve(keyValues.size());

        for (uint64_t i = 0; i < keyValues.size(); i++) {
            tempGrouppingMap.emplace(keyValues[i].first, 0).first->second++;
        }
        
        keyValues.clear();
        
        for(auto const &kv : tempGrouppingMap){
            std::vector<std::string> tempKey = kv.first;
            tempKey[2] = std::to_string(kv.second);
            keyValues.emplace_back(tempKey,std::vector<double>());
        }
    }
};

#endif /* UTILITYFUNCTIONS_HPP */