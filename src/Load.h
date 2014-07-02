#ifndef LOAD_H
#define LOAD_H

class Load {
public:
  Load();
  Load(char *table);
  void split();
  void loadOrders();
  void loadCustomer();
private:
  // source file pointer
  FILE* fp;
  // reading buffer for a line
  char buffer[300];
  // for a row storage the column
  char *column[8]; 
  // temporary string for spliting
  char *s;
  // item counter in a page
  int count;
};

#endif