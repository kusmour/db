#include <stdio.h>

#ifndef RETRIEVE_H
#define RETRIEVE_H

class Retrieve {
	public:
		Retrieve();
		Retrieve(int k);
		int binary_search(const int [], int, int);
		int findPage();
		int findPosition();
		void getData();
	private:
		int key;
		int page;
		int position;
};
#endif