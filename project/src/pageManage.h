/************************************
 * Copyright 2014.5.29
 * Owner: 12330318 weijiewei
 * File:  pageManage.h
 * Use:   the kernel of c-store
 ************************************/
#ifndef _PAGEMANAGE_H_
#define _PAGEMANAGE_H_

#include <cstdio>
#include "path.h"
#include "size.h"

/* Disallow the type to copy and assign. */
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)

/*********************** pageManage *************************
 * +loadData_o           - load orders.tbl.
 * +loadData_c           - load customer.tbl.
 * +queryItemByOrderkey  - query by orderkey.
 * +compressCustkey      - compress the custkey in orders.
 * +join                 - join orders and customer.
 * +count                - count the orders.
 * -binarySearch         - binary search for query.
 * -quickSort            - quick sort for external sort.
 * -externalSort         - external sort for compress.
 * note: 1.each method return true with Successful operation.
 *       2.the pageManage use singleton pattern.
 ************************************************************/
class pageManage {
 public:
  bool loadData_o();
  bool loadData_c();
  bool queryItemByOrderkey(int orderkey, int& custkey,
    double& tolalprice, int& shippriority, int* queryTable, int queryTableSize);
  bool compressCustkey();
  bool join();
  bool count(int& num);

  /* For singleton pattern. */
  static pageManage* getInstance();
  ~pageManage();

 private:
  /* For sort. */
  struct tuple {
    int orderkey;
    int custkey;
    tuple(int orderkey_ = 0, int custkey_ = 0) :
    orderkey(orderkey_), custkey(custkey_) {}
  };
  /* For compress. */
  struct item {
    int value;
    int length;
    item(int value_ = 0, int length_ = 1) :
    value(value_), length(length_) {}
  };

  int binarySearch(const int* array, int start, int end, int key);
  void quickSort(int arr1[], int arr2[], int size);
  bool externalSort();
  inline char* getTempName(int number);

  /* For singleton pattern. */
  pageManage();
  static bool instanceFlag;
  static pageManage* instance;
  DISALLOW_COPY_AND_ASSIGN(pageManage);
};

#endif  // _PAGEMANAGE_H_
