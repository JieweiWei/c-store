/*******************************************
 * Copyright 2014.5.29
 * Owner: 12330318 weijiewei
 * File:  cStoreController.cpp
 * Use:   the implement of cStoreController
 *******************************************/
#include "cStoreController.h"

/* The default constructor. */
cStoreController::cStoreController() {}

/* User interaction. */
void cStoreController::executeOperation(int argc, char *argv[]) {
  if (argc == 3 && strcmp(argv[1], "load") == 0)  {
    if (strcmp(argv[2], "orders") == 0)
      loadData(orders);
    else if (strcmp(argv[2], "customer") == 0)
      loadData(customer);
    else
      printf("Command not found! please use <help> command.\n");
  } else if (argc == 3 && strcmp(argv[1], "retrieve") == 0
    && strcmp(argv[2], "orders") == 0) {
    queryItemByOrderkey();
  } else if (argc == 4 && strcmp(argv[1], "compress") == 0
    && strcmp(argv[2], "orders") == 0) {
    if (strcmp(argv[3], "1") == 0)
      compress();
    else
      printf("Sorry, you can only compress the first column!\n");
  } else if (argc == 2 && strcmp(argv[1], "join") == 0) {
    join();
  } else if (argc == 2 && strcmp(argv[1], "help") == 0) {
    help();
  } else if (argc == 2 && strcmp(argv[1], "count") == 0) {
    count();
  } else {
    printf("Command not found! please use <help> command.\n");
  }
}

/* Load data from .tbl files. */
void cStoreController::loadData(name_of_table nameOfTable) {
  printf("Loading data...\n");
  time_t t_start = time(NULL);
  if (nameOfTable == orders) {
    if (cStoreService_.loadData_o())
      printf("Success to load data.");
    else
      printf("Fail to load data.");
  } else {
    if (cStoreService_.loadData_c())
      printf("Success to load data.");
    else
      printf("Fail to load data.");
  }
  time_t t_end = time(NULL);
  printf(" (%.2lf sec)\n", difftime(t_end, t_start));
}

/* Query by orderkey. */
void cStoreController::queryItemByOrderkey() {
  printf("---------------------------- query ------------------------------\n");
  printf("Remind :\n");
  printf("Please enter a orderkey.\n");
  printf("Enter <ctrl+d> to stop.\n");
  printf("-----------------------------------------------------------------\n");
  int orderkey, custkey, shippriority;
  double totalprice;
  FILE *tableFile = fopen(PATH_OF_QUERY_TABLE, "rb");
  if (!tableFile) {
    printf("Fail to retrieve!\n");
    return;
  }
  int queryTable[MAX_SIZE_QUERY_TABLE];
  int queryTableSize =
    fread(queryTable, sizeof(int), MAX_SIZE_QUERY_TABLE, tableFile);
  while (scanf("%d", &orderkey) != EOF) {
    time_t t_start = time(NULL);
    if (cStoreService_.queryItemByOrderkey(orderkey,
      custkey, totalprice, shippriority, queryTable,
      queryTableSize)) {
      printf("%d %d %.2lf %d", orderkey, custkey,
        totalprice, shippriority);
    } else {
      printf("Empty.");
    }
    time_t t_end = time(NULL);
    printf(" (%.2lf sec)\n", difftime(t_end, t_start));
  }
  fclose(tableFile);
  printf("Finish.\n");
}

/* Compress the custkey in orders. */
void cStoreController::compress() {
  printf("Compressing data...\n");
  time_t t_start = time(NULL);
  if (cStoreService_.compress()) {
    printf("Success to compress.");
  } else {
    printf("Fail to compress.");
  }
  time_t t_end = time(NULL);
  printf(" (%.2lf sec)\n", difftime(t_end, t_start));
}

/* Join orders and customer. */
void cStoreController::join() {
  printf("Joining data...\n");
  time_t t_start = time(NULL);
  if (cStoreService_.join()) {
    printf("Success to join.");
  } else {
    printf("Fail to join.");
  }
  time_t t_end = time(NULL);
  printf(" (%.2lf sec)\n", difftime(t_end, t_start));
}

/* Count the orders. */
void cStoreController::count() {
  printf("counting data...\n");
  time_t t_start = time(NULL);
  int num = 0;
  if (cStoreService_.count(num)) {
    printf("%d", num);
  } else {
    printf("Fail to count.");
  }
  time_t t_end = time(NULL);
  printf(" (%.2lf sec)\n", difftime(t_end, t_start));
}

/* print all of commands. */
void cStoreController::help() {
  printf("---------------------------- cstore -----------------------------\n");
  printf("Action :\n");
  printf("load orders         - load the data from orders.tbl to cstore.\n");
  printf("load customer       - load the data from customer.tbl to cstore.\n");
  printf("retrieve orders     - query the item by orderkey.\n");
  printf("compress orders 1   - compress the first column.\n");
  printf("join                - join orders.tbl and customer.tbl\n");
  printf("count               - count the items of orders.tbl.\n");
  printf("-----------------------------------------------------------------\n");
}
