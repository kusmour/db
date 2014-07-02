#ifndef JOIN_H
#define JOIN_H

class Join {
public:
	Join();
	~Join();
	void Orders_Customer();
private:
	FILE* orderkey_fp;
	FILE* custkey_fp;
	FILE* compressed_fp;
	FILE* outfile_fp;
};
#endif