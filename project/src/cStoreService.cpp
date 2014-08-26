/****************************************
 * Copyright 2014.5.29
 * Owner: 12330318 weijiewei
 * File:  cStoreService.cpp
 * Use:   the implement of cStoreService
 ****************************************/
#include "cStoreService.h"

/* The default constructor. */
cStoreService::cStoreService() {
  pageManage_ = pageManage::getInstance();
}

/* The destructor. */
cStoreService::~cStoreService() {
  delete pageManage_;
}

/* Load orders.tbl. */
bool cStoreService::loadData_o() {
  pageManage_->loadData_o();
}

/* Load customer.tbl. */
bool cStoreService::loadData_c() {
  pageManage_->loadData_c();
}

/* Query by orderkey. */
bool cStoreService::queryItemByOrderkey(int orderkey, int& custkey,
  double& tolalprice, int& shippriority, int* queryTable, int queryTableSize) {
  return pageManage_->queryItemByOrderkey(orderkey, custkey,
    tolalprice, shippriority, queryTable, queryTableSize);
}

/* Compress the custkey in orders. */
bool cStoreService::compress() {
  return pageManage_->compressCustkey();
}

/* Join orders and customer. */
bool cStoreService::join() {
  return pageManage_->join();
}

/* Count the orders. */
bool cStoreService::count(int& num) {
  return pageManage_->count(num);
}
