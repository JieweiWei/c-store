/***********************************************
 * Copyright 2014.5.29
 * Owner: 12330318 weijiewei
 * File:  cStoreController.h
 * Use:   the controller of c-store, the class
 *        is designed for user interaction
 ***********************************************/
#ifndef _CSTORECONTROLLER_H_
#define _CSTORECONTROLLER_H_

#include <cstring>
#include <ctime>
#include "cStoreService.h"

/******************** cStoreController **********************
 * +executeOperation     - execute operation.
 * -loadData             - load data from .tbl files.
 * -queryItemByOrderkey  - query by orderkey.
 * -compress             - compress the custkey in orders.
 * -join                 - join orders and customer.
 * -count                - count the orders.
 * -help                 - print all of commands.
 ************************************************************/
class cStoreController {
 public:
  cStoreController();
  void executeOperation(int argc, char *argv[]);
 private:
  enum name_of_table {orders, customer};
  void loadData(name_of_table nameOfTable);
  void queryItemByOrderkey();
  void compress();
  void join();
  void count();
  void help();
  cStoreService cStoreService_;
};

#endif  // _CSTORECONTROLLER_H_
