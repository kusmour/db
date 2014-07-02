#include <stdlib.h>
#include <iostream>
#include "Retrieve.h"

using namespace std;

#define PAGE_SIZE 1024

#define ORDERKEY_PATH "orderkey.dat"
#define CUST_PATH "custkey.dat"
#define PRICE_PATH "totalprice.dat"
#define PRIORITY_PATH "priority.dat"
#define CATALOGUE_PATH "catalogue.dat"

Retrieve::Retrieve() {
}

Retrieve::Retrieve(int k) {
	key = k;
	// find the page that key might be in
	page = findPage();
	// get the position in the exact page that key might be in
	position = findPosition();
	getData();
}

// binary search in a page
int Retrieve::binary_search(const int a[], int low, int high) {
	if (low > high) {
		return -1;
	}
	int mid = low + (high - low) / 2;
	if (a[mid] == key) {
		return mid;
	} else if(a[mid] > key) {
		return binary_search(a, low, mid - 1);
	} else {
		return binary_search(a, mid + 1, high);
	}
}

// find the page that the key might be in
int Retrieve::findPage() {
	FILE *fp;
	int pageNum;
    int page = 0;
    int pages[PAGE_SIZE];

	fp = fopen(CATALOGUE_PATH, "rb");
	// the first element in catalogue is the number of page
	fread(&pageNum, sizeof(int), 1, fp);
	// get the first orderkey of each page
    while (fread(pages, sizeof(int), PAGE_SIZE, fp) != 0) {
        
        // to confirm the number of page is right
        /*
         int temp = fread(pages, sizeof(int), 10001, fp);
         printf("%d", temp);
         */

        // compare key with first orderkey in each pages
        // to find which page key might be in
        for (int i = 0; i < PAGE_SIZE - 1; i++) {
            if (pages[i] <= key && key < pages[i + 1]) {
                return page + i;
            }
        }
        page += PAGE_SIZE;
        
    }
    fclose(fp);
    return pageNum - 1;
}

// find the position of the key in a page
int Retrieve::findPosition() {
	FILE *fp;
	int dataNum = 0;
	int orderkey[1024];

	fp = fopen(ORDERKEY_PATH, "rb");
	fseek(fp, page * PAGE_SIZE * 4, SEEK_SET);
	// dataNum is the number if data in that page
	dataNum = (int)fread(orderkey, sizeof(int), PAGE_SIZE, fp);
	fclose(fp);
	// use binary search
	//to find the position in that page and return it
	return binary_search(orderkey, 0, dataNum - 1);
}

void Retrieve::getData() {
	int data[1024];
	double total[1024];

	int custkey = -1;
	double totalprice = -1;
	int priority = -1;

	// if position is -1 means that
	// the position of the orderkey cannot be found
	// which means that the orderkey does not exist
	if (position == -1) {
		printf("Data can not be found.\nPlease have another try.\n");
		exit(0);
	}

	FILE *fp;

	// get custkey
	fp = fopen(CUST_PATH, "rb");
	fseek(fp, page * 1024 * 4, SEEK_SET);
	fread(data, sizeof(int), 1024, fp);
	custkey = data[position];
	fclose(fp);

	// get totalprice
	// totalprice is a double
	fp = fopen(PRICE_PATH, "rb");
	fseek(fp, page * 1024 * 8, SEEK_SET);
	fread(total, sizeof(double), 1024, fp);
	totalprice = total[position];
	fclose(fp);

	// get shippriority
	fp = fopen(PRIORITY_PATH, "rb");
	fseek(fp, page * 1024 * 4, SEEK_SET);
	fread(data, sizeof(int), 1024, fp);
	priority = data[position];
	fclose(fp);

	printf("ORDERKEY | CUSTKEY | TOTALPRICE | SHIPPRIORITY\n");
	printf("%d  %d  %.2f  %d\n", key, custkey, totalprice, priority);
}
