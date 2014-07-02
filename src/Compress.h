#include <stdio.h>
#ifndef COMPRESS_H
#define COMPRESS_H

class Compress
{
public:
    Compress(){};
    Compress(const char * infile_name, const char * outfile_name);
	bool RLEcompress(const char * infile_name, const char * outfile_name);
};
#endif