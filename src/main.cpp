#include "Header.h"
using namespace std;

void load(char* table);
void retrieveOrderkey(int key);
void compress();
void join();
void count();

void test();
void test2();

// main function with parameters
int main(int argc, char *argv[]) {
    // valid instruction
    if (argc == 3) {
        // "db load orders" or "...customer"
        if (!strcmp(argv[1], "load")) {
            load(argv[2]);
        }
    } else if (argc == 4) {
        // "db retrieve orders"
        if (!strcmp(argv[1], "retrieve") && !strcmp(argv[2], "orders")) {
            retrieveOrderkey(atoi(argv[3]));
        } else if (!strcmp(argv[1], "compress") && !strcmp(argv[2], "orders")
                   && !strcmp(argv[3], "1")) {
            // "db compress orders 1"
            // compress the ORDERS ordered by CUNSTKEY
            compress();
        }
    } else if (argc == 2) {
        if (!strcmp(argv[1], "join")) {
            join();
        } else if (!strcmp(argv[1], "count")) {
            count();
        }
    }
    // test();
    return 0;
} // end main


// function to load table
void load(char* table) {
    printf("Load beginning workout.\n");
    clock_t start, end;
    double duration = 0;
    start = clock();
    
    Load* l;
    l = new Load(table);
    delete l;
    
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.2f s\n", duration);
    
}


// function to retrieve the orderkey
void retrieveOrderkey(int key) {
    Retrieve* retrieve;
    retrieve = new Retrieve(key);
    delete retrieve;
}

// to sort the orders with custkey
void compress() {
    const char *unsorted_file = "../orders.tbl";  // unsorted file name
    const char *infile = "sorted_cust.dat";
    const char *outfile = "compressed_cust.dat";

    clock_t start, end;
    double duration = 0;
    start = clock();
    
    printf("Sort beginning workout.\n");
    externalSort extSort(unsorted_file);
    extSort.sort();
    
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.2f s\n", duration);
    
    start = clock();
    
    Compress* cprs;
    printf("Compress beginning workout.\n");
    cprs = new Compress(infile, outfile);
    delete cprs;
    
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.2f s\n", duration);
}

// join the two table orders.tbl and customer.tbl
void join() {
    printf("Join beginning workout.\n");
    
    clock_t start, end;
    double duration = 0;
    start = clock();
    
    Join* j;
    j = new Join();
    delete j;
    
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.2f s\n", duration);
}

// function to count lines of joined orderkey
void count() {
    FILE *fp;
    if ((fp = fopen("compressed_cust.dat", "rb")) == NULL) {
        printf("File cannot be opened.\n");
        fclose(fp);
        exit(1);
    }
    int count;
    int total = 0;
    while(fread(&count, sizeof(int), 1,fp) != 0) {
        fread(&count, sizeof(int), 1, fp);
        total += count;
    }
    printf("The number of lines of orderkey is: %d\n", total);
    fclose(fp);
}


void test() {
    FILE *fp;
    if ((fp = fopen("sorted_cust.dat", "rb")) == NULL) {
        printf("File cannot be opened.\n");
        exit(1);
    }
    
    printf("Here is the test.\n");
    
    int data[1024];
    int count = 0;
    count = (int)fread(data, sizeof(int), 1024, fp);
    while (count != 0) {
        for (int i = 0; i < 100; i += 2) {
            cout << data[i] << "  " << data[i + 1] << endl;
        }
        memset(data, 0, sizeof(data));
        count = (int)fread(data, sizeof(int), 1024, fp);
    }
    fclose(fp);
}

void test2() {
    remove("test.dat");
    FILE* fp = fopen("test.dat", "wb");
    int data[4];
    data[0] = 4;
    data[1] = 11;
    data[2] = 149992;
    data[3] = 149999;
    fwrite(data, sizeof(int), 4, fp);
    fclose(fp);
}
