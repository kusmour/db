#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Join.h"

#define SORTED_ORDER_PATH "sorted_order.dat"
#define CUST_PATH2 "custkey2.dat"
#define COMPRESSED_PATH "compressed_cust.dat"
#define OUT "stdout.dat"

Join::Join() {
	orderkey_fp = NULL;
	custkey_fp = NULL;
	compressed_fp = NULL;
	outfile_fp = NULL;

	orderkey_fp = fopen(SORTED_ORDER_PATH, "rb");
	custkey_fp = fopen(CUST_PATH2, "rb");
	compressed_fp = fopen(COMPRESSED_PATH, "rb");

	outfile_fp = fopen(OUT, "wb");

	if (orderkey_fp == NULL || custkey_fp == NULL || compressed_fp == NULL) {
		printf("Files open failed.\n");
		exit(1);
	}
    Orders_Customer();
}
Join::~Join() {
	// close all file pointers
	fclose(orderkey_fp);
	fclose(custkey_fp);
	fclose(compressed_fp);
	fclose(outfile_fp);
}

void Join::Orders_Customer() {
	int custkey;
	int orderkey[100001];
	int compressed;

	// counter for orderkey
	int count = 0;
    // wether the end of compressed custkey
    bool flag = true;

	fread(&compressed, sizeof(int), 1, compressed_fp);
	// read the custkey from table COSTOMER
	while (fread(&custkey, sizeof(int), 1, custkey_fp) != 0 && flag) {
		// if found
		if (custkey > compressed) {
            // if current custkey from customer is bigger than current compressed custkey
            // then move on to next compressed custkey
            while (custkey > compressed) {
                fread(&count, sizeof(int), 1, compressed_fp);
                // move the file pointer of orderkey
                fseek(orderkey_fp, sizeof(int) * count, SEEK_CUR);
                // read next compressed custkey
                // and judge wether at the end of the compressed custkey table
                // then finish traversal
                if (fread(&compressed, sizeof(int), 1, compressed_fp) == 0) {
                    flag = false;
                }
            }
		}
        if (custkey == compressed) {
			// get the number of custkey
			fread(&count, sizeof(int), 1, compressed_fp);
			// get the corresponding orderkeys
			fread(orderkey, sizeof(int), count, orderkey_fp);
			// write the joined result
			for (int i = 0; i < count; i++) {
				fwrite(&compressed, sizeof(int), 1, outfile_fp);
				fwrite(&orderkey[i], sizeof(int), 1, outfile_fp);
                printf("%d, %d\n", compressed, orderkey[i]);
			}
            // read next compressed custkey
            // and judge wether at the end of the compressed custkey table
            // then finish traversal
            if (fread(&compressed, sizeof(int), 1, compressed_fp) == 0) {
                //printf("Compressed: %d\n", compressed);
                flag = false;
            }
		}
        memset(orderkey, 0, sizeof(orderkey));
	}  // end the whole loop
    printf("Join finished.\n");
}