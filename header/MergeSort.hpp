//Source: https://www.geeksforgeeks.org/merge-sort/
//License: https://creativecommons.org/licenses/by-sa/4.0/

/*
 changes has made on original code to enable sorting string values
 */

/* C program for Merge Sort */

#ifndef MERGESORT_HPP
#define MERGESORT_HPP

#include <stdlib.h>
#include <stdio.h>
#include <vector>

class MergeSort {
private:

    //compare two vectors of strings

    static int compareStringVectors(std::vector<std::string> left, std::vector<std::string> right, std::vector<int>& sortingDirection) {
        //equal
        int result = 0;
        for (int i = 0; i < left.size(); i++) {
            if (left[i] < right[i]) {
                //less than
                result = sortingDirection[i];
                break;
            } else if (left[i] > right[i]) {
                //greater than
                result = sortingDirection[i] * -1;
                break;
            }
        }
        return result;
    }

    // Merges two subarrays of keyValues[].
    // First subarray is keyValues[l..m]
    // Second subarray is keyValues[m+1..r]

    static void merge(std::vector<std::pair<std::vector<std::string>, std::vector<std::string> > >& keyValues, int left, int middle, int right, std::vector<int>& sortingDirection) {
        int i, j, k;
        int n1 = middle - left + 1;
        int n2 = right - middle;

        /* create temp arrays */
        std::vector<std::pair<std::vector<std::string>, std::vector<std::string> > > L(n1), R(n2);

        /* Copy data to temp arrays L[] and R[] */
        for (i = 0; i < n1; i++)
            L[i] = keyValues[left + i];
        for (j = 0; j < n2; j++)
            R[j] = keyValues[middle + 1 + j];

        /* Merge the temp arrays back into keyValues[l..r]*/
        i = 0; // Initial index of first subarray
        j = 0; // Initial index of second subarray
        k = left; // Initial index of merged subarray
        while (i < n1 && j < n2) {
            if (compareStringVectors(L[i].first, R[j].first, sortingDirection) <= 0) {
                keyValues[k] = L[i];
                i++;
            } else {
                keyValues[k] = R[j];
                j++;
            }
            k++;
        }

        /* Copy the remaining elements of L[], if there
           are any */
        while (i < n1) {
            keyValues[k] = L[i];
            i++;
            k++;
        }

        /* Copy the remaining elements of R[], if there
           are any */
        while (j < n2) {
            keyValues[k] = R[j];
            j++;
            k++;
        }
    }


public:

    /* l is for left index and r is right index of the
       sub-array of keyValues to be sorted */
    static void mergeSort(std::vector<std::pair<std::vector<std::string>, std::vector<std::string> > >& keyValues, int left, int right, std::vector<int>& sortingDirection) {
        if (left < right) {
            // Same as (left+right)/2, but avoids overflow for
            // large left and right
            int middle = left + (right - left) / 2;

            // Sort first and second halves
            mergeSort(keyValues, left, middle, sortingDirection);
            mergeSort(keyValues, middle + 1, right, sortingDirection);

            merge(keyValues, left, middle, right, sortingDirection);
        }
    }
};

#endif /* MERGESORT_HPP */