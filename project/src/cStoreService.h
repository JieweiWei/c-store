/************************************************
 * Copyright 2014.5.29
 * Owner: 12330318 weijiewei
 * File:  cStoreService.h
 * Use:   the service of c-store, the class is
 *        designed for expansion of the c-store
 ************************************************/
#ifndef _CSTORESERVICE_H_
#define _CSTORESERVICE_H_

#include "pageManage.h"

/********************** cStoreService ***********************
 * +loadData_o           - load orders.tbl.
 * +loadData_c           - load customer.tbl.
 * +queryItemByOrderkey  - query by orderkey.
 * +compress             - compress the custkey in orders.
 * +join                 - join orders and customer.
 * +count                - count the orders.
 * note: 1.each method return true with Successful operation.
 ************************************************************/
class cStoreService {
 public:
  cStoreService();
  ~cStoreService();
  bool loadData_o();
  bool loadData_c();
  bool queryItemByOrderkey(int orderkey, int& custkey,
    double& tolalprice, int& shippriority, int* queryTable,
    int queryTableSize);
  bool compress();
  bool join();
  bool count(int& num);
 private:
  pageManage* pageManage_;
};

#endif  // _CSTORESERVICE_H_
