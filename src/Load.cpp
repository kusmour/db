#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Load.h"

using namespace std;

#define PAGE_SIZE 1024

// the file paths for loadOrders
#define ORDER_PATH "../orders.tbl"
#define ORDERKEY_PATH "orderkey.dat"
#define CUST_PATH "custkey.dat"
#define PRICE_PATH "totalprice.dat"
#define PRIORITY_PATH "priority.dat"
#define CATALOGUE_PATH "catalogue.dat"

// the file paths for loadCustomer
#define CUSTOMER_PATH "../customer.tbl"
#define CUST_PATH2 "custkey2.dat"

Load::Load() {
}

Load::Load(char* table) {
  if (!strcmp(table, "orders")) {
    loadOrders();
  } else if (!strcmp(table, "customer")) {
    loadCustomer();
  } else {
    printf("Table not found.\n");
  }
}

// function to split the line from source file
void Load::split() {
  s = strtok(buffer, "|");
  for (int i = 0; i < 8; i++) {
    column[i] = s;
    s = strtok(NULL, "|");
  }
}

void Load::loadOrders() {
  // wether open is successful
  if ((fp = fopen(ORDER_PATH, "rb")) == NULL) {
    printf("orders.tbl cannot be opened.\n");
    fclose(fp);
    exit(1);
  }

  // arrays for one page
  int orderkey[PAGE_SIZE];
  int custkey[PAGE_SIZE];
  double totalprice[PAGE_SIZE];
  int priority[PAGE_SIZE];

  // number of page
  int pageNum = 0;

  // file pointers
  FILE *orderkey_fp = NULL;
  FILE *custkey_fp = NULL;
  FILE *priority_fp = NULL;
  FILE *totalprice_fp = NULL;
  FILE *Catalogue_fp = NULL;

  // make sure it won't be written twice
  remove(CUST_PATH);
  remove(PRICE_PATH);
  remove(PRIORITY_PATH);
  remove(CATALOGUE_PATH);

  orderkey_fp = fopen(ORDERKEY_PATH, "wb");
  custkey_fp = fopen(CUST_PATH,"wb");
  totalprice_fp = fopen(PRICE_PATH,"wb");
  priority_fp = fopen(PRIORITY_PATH,"wb");
  // the catalogue file
  Catalogue_fp = fopen(CATALOGUE_PATH,"wb");

  // write down the number of page
  fwrite(&pageNum, sizeof(int), 1, Catalogue_fp);

  // counter in a page
  count = 0;

  // while is not EOF
  while (fgets(buffer, 300, fp)) {
    // get a line
    split();
    // we need column 0 1 3 7
    // with the type int int double int
    orderkey[count] = atoi(column[0]);
    custkey[count] = atoi(column[1]);
    totalprice[count] = atof(column[3]);
    priority[count] = atoi(column[7]);

    count++;

    // if a page is full
    // write then
    if (count == PAGE_SIZE) {
      fwrite(orderkey, sizeof(int), count, orderkey_fp);
      fwrite(custkey, sizeof(int), count, custkey_fp);
      // notice that totalprice is double
      fwrite(totalprice, sizeof(double), count, totalprice_fp);
      fwrite(priority, sizeof(int), count, priority_fp);

      fwrite(orderkey, sizeof(int), 1, Catalogue_fp);

	   // clear the counter
	   count = 0;
     // increment of number of page
     pageNum++;
	   // clear all arrays
	   memset(orderkey, 0, sizeof(orderkey));
	   memset(custkey, 0, sizeof(custkey));
	   memset(totalprice, 0, sizeof(totalprice));
	   memset(priority, 0, sizeof(priority));
    }
  }

  // if is not full of a page
  // which means it is the last page
  // write then
  fwrite(orderkey, sizeof(int), count, orderkey_fp);
  fwrite(custkey, sizeof(int), count, custkey_fp);
  // notice that totalprice is double
  fwrite(totalprice, sizeof(double), count, totalprice_fp);
  fwrite(priority, sizeof(int), count, priority_fp);
  // write down the first orderkey of the page
  fwrite(orderkey, sizeof(int), 1, Catalogue_fp);

  // increment of pageNum
  pageNum++;

  // save the number of page in the catalogue file
  // first find the first position
  fseek(Catalogue_fp, 0, SEEK_SET);
  fwrite(&pageNum, sizeof(int), 1, Catalogue_fp);

  fclose(orderkey_fp);
  fclose(custkey_fp);
  fclose(priority_fp);
  fclose(totalprice_fp);
  fclose(Catalogue_fp);
  fclose(fp);
  printf("Load Orders finished.\n");
}

void Load::loadCustomer() {
  // wether open is successful
  if ((fp = fopen(CUSTOMER_PATH, "rb")) == NULL) {
    printf("customer.tbl cannot be opened.\n");
    fclose(fp);
    exit(1);
  }
  // make sure file won't be written twice
  remove(CUST_PATH2);

  FILE *custkey_fp = fopen(CUST_PATH2, "wb");
  // arrays for one page
  int custkey[PAGE_SIZE];

  count = 0;
  while(fgets(buffer, 300, fp)) {
    // get a line from source file
    split();
    // we need the 1st column
    custkey[count] = atoi(column[0]);
    
    count++;

    // if a page is full
    if (count == PAGE_SIZE) {
      fwrite(custkey, sizeof(int), count, custkey_fp);
      //clear the array
      memset(custkey, 0, sizeof(custkey));
      // clear the counter
      count = 0;
    }
  }
  // the last time of write
  fwrite(custkey, sizeof(int), count, custkey_fp);

  fclose(custkey_fp);
  fclose(fp);
  printf("Load Customer finished.\n");
}
