#include <cstdio>

#ifndef EXTERNALSORT_H
#define EXTERNALSORT_H

class externalSort{
public:
    externalSort(const char *unsort);
    void sort();
    ~externalSort();

private:
    char *_unsorted;
    bool flag;
    int fileNum; // the number of temp file

protected:
    void write(FILE *f, int a[][2], int n);
    char* fileName(int);
    static int cmp(const void *a, const void *b);
    void memSort();
    bool mergeSort();
    void createLoserTree(int*, const int*);
    void adjust(int*, int, const int*);
};
#endif
