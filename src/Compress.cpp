#include <string.h>
#include <istream>
#include "Compress.h"
using namespace std;

Compress::Compress(const char * infile_name, const char * outfile_name) {
    
    bool flag = false;
    
    if (RLEcompress(infile_name, outfile_name)) {
        flag = true;
    }
    
    if (flag) {
        printf("Compress succeed.\n");
    } else {
        printf("Compress failed\n");
    }
}

bool Compress::RLEcompress(const char * infile_name, const char * outfile_name)
{
	int seq_len;
	int cur_seq;
	int cnt = 0;
	int cur_int[1];
	int store[131072];
	//128 * 1024
	int flag;
    
    remove(outfile_name);
    
	FILE *infile, *outfile;
	infile = fopen(infile_name, "rb");
	outfile = fopen(outfile_name, "wb");
    
	if (infile == NULL)
		return false;
	//open infile fail
    
	if (feof(infile))
		return false;
	//infile is empty
    
	flag = (int)fread(cur_int, sizeof(int), 1, infile);
	cur_seq = cur_int[0];
	seq_len = 1;
	while (flag != 0)
	{
		//RLEcompress
		flag = (int)fread(cur_int, sizeof(int), 1, infile);
		if (cur_seq == cur_int[0])
			seq_len++;
        //if the same, then count
		else {
			store[cnt] = cur_seq;
			store[cnt + 1] = seq_len;
			cnt += 2;
            
			if (cnt == 131072) {
				fwrite(store, sizeof(int), cnt, outfile);
				memset(store, 0, sizeof(store));
				cnt = 0;
			}
			//paging
            
			cur_seq = cur_int[0];
			seq_len = 1;
			//store the outcome into an array
		}
	}
	if (seq_len >= 1) {
		store[cnt] = cur_int[0];
		store[cnt + 1] = seq_len - 1;
		cnt += 2;
	}
    
	fwrite(store, sizeof(int), cnt, outfile);
	//write to the outfile
    
	fclose(infile);
	fclose(outfile);
    
	return true;
}