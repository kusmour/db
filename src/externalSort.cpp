#include <cstdlib>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include "externalSort.h"

#define PAGE_SIZE 1024

externalSort::externalSort(const char *unsorted) {
  _unsorted = new char[strlen(unsorted) + 1];
  strcpy(_unsorted, unsorted);
  fileNum = 0;
  flag = false;
}

void externalSort::sort() {
  memSort();
  flag = mergeSort();
  if (flag) {
    printf("Sorting succeeded.\n");
  } else {
    printf("Sorting failed.");
  }
}

externalSort::~externalSort() {
  delete _unsorted;
}

void externalSort::write(FILE *f, int a[][2], int n) {
  fwrite(a, sizeof(int), n, f);
  return;
}

// creat a temp file name, using index
char* externalSort::fileName(int index) {
  char *temp = new char[100];
  sprintf(temp, "temp%d.txt", index);
  return temp;
}

// compare function for qsort, according to the first colum
int externalSort::cmp(const void *a, const void *b) {
  return ((int *)a)[0] - ((int *)b)[0];
}

// obtain sorted segment
void externalSort::memSort() {
  char buffer[PAGE_SIZE] = {0};
  int orderkey = 0, custkey = 0; // columns needed
  int data[PAGE_SIZE*64][2]; // stores custkey and orderkey£¬64 pages for each
  int i = 0;
  FILE* fin = fopen(_unsorted, "r");
  // extract orderkey and custkey
  while (!feof(fin)) {
    fgets(buffer, PAGE_SIZE, fin);
    sscanf(buffer,"%d|%d|%*c|%*lf|%*[^|]|%*[^|]|%*[^|]|%*d|%*[^|]|", &orderkey, &custkey);
    data[i][0] = custkey;
    data[i][1] = orderkey;
    ++i;
    // read every 128 pages, sort them in data array,
    // write the sorted data to temp file
    if (i >= PAGE_SIZE*64) {
      qsort(data,PAGE_SIZE*64,sizeof(int)*2,cmp);
      // write into temp file
      char *tempName = fileName(fileNum);
      fileNum++;
      FILE *tempFile = fopen(tempName, "ab+");
      free(tempName);
      write(tempFile, data, PAGE_SIZE*128);
      fclose(tempFile);
      i = 0;
    }
  }
  // handle data left which are less than 128 pages
  if (i != 0) {
    --i;
    qsort(data,PAGE_SIZE*64,sizeof(int)*2,cmp);
    char *tempName = fileName(fileNum);
    FILE *tempFile = fopen(tempName, "ab+");
    free(tempName);
    write(tempFile,data,i*2);
    fclose(tempFile);
  }
  fclose(fin);
  fileNum += 1;
  return;
}

// multiple merge sort
bool externalSort::mergeSort() {
  if (fileNum <= 0) {
    return false;
  }
    
  // make sure file won't be written twice
  remove("sorted_cust.dat");
  remove("sorted_order.dat");

 
  FILE *fout_cust = fopen("sorted_cust.dat", "ab+");
  FILE *fout_order = fopen("sorted_order.dat", "ab+");

  FILE* *farray = new FILE*[fileNum]; // point pointers to temp files
  // creat pointers to each temp file  
  for (int i = 0; i < fileNum; ++i) {
    char* tempFile = fileName(i);
    farray[i] = fopen(tempFile, "rb");
    free(tempFile);
  }

  int *dataCust = new int[fileNum + 1]; // leaves node
  int *dataOrder = new int[fileNum];
  int *ls = new int[fileNum]; // loser tree, stores the position of parents
  int MINKEY = 0;
  int MAXKEY = 999999999;


  memset(dataCust, 0, sizeof(int) * (fileNum + 1));
  memset(dataOrder, 0, sizeof(int) * fileNum);
  memset(ls, 0, sizeof(int) * fileNum);

  //read data
  for (int i = 0; i < fileNum; i++) {
    fread(&dataCust[i], sizeof(int), 1, farray[i]);
    fread(&dataOrder[i], sizeof(int), 1, farray[i]);

  }
  dataCust[fileNum] = MINKEY; // use for create loser tree

  createLoserTree(ls, dataCust);

  // sort
  while(dataCust[ls[0]] != MAXKEY) {
    int index = ls[0]; // position stores the file index of minimum custkey

    // writes the custkey and orderkey of winner
    fwrite(&dataCust[index], sizeof(int), 1, fout_cust);
    fwrite(&dataOrder[index], sizeof(int), 1, fout_order);

    // read new data
    fread(&dataCust[index], sizeof(int), 1, farray[index]);
    if (feof(farray[index]) != 0) {
      dataCust[index] = MAXKEY; // set end mark
    } else {
      fread(&dataOrder[index], sizeof(int), 1, farray[index]);
    }
    adjust(ls,index,dataCust);
  }
  // close file
  delete [] dataCust;
  delete [] dataOrder;
  for (int i = 0; i < fileNum; ++i) {
    fclose(farray[i]);
  }

  // delete the temporary file    
  for (int i = 0; i < fileNum; ++i) {
    remove(fileName(i));
  }
    
  delete [] farray;
  fclose(fout_cust);
  fclose(fout_order);
  return true;
}

void externalSort::createLoserTree(int *ls, const int *b) {
  
  for (int i = 0; i < fileNum; ++i)
    ls[i] = fileNum; // initialize loser tree
  for (int i = fileNum - 1; i >= 0; --i)
    adjust(ls, i, b); // adjust loser tree
}

void externalSort::adjust(int *ls, int winner, const int *b) {
  int parent = (winner + fileNum) / 2; // parent of winner stored in ls
  int temp;
  while (parent > 0) {
    if (b[winner] > b[ls[parent]]) {
      temp = winner; // change positions of winner and loser
      winner = ls[parent];
      ls[parent] = temp;
    }
    parent/=2; // higher level parent
  }
  ls[0] = winner; // ls[0] stores the winner of the tree
}
