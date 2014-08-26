/*******************************************************
 * Copyright 2014.5.29
 * Owner: 12330318 weijiewei
 * File:  cStore.cpp
 * Use:   Interface to the operating system and c-store
 *******************************************************/
#include "cStoreController.h"

int main(int argc, char *argv[]) {
  cStoreController cStoreController_;
  cStoreController_.executeOperation(argc, argv);
  return 0;
}

