/*************************************
 * Copyright 2014.5.29
 * Owner: 12330318 weijiewei
 * File:  pageManage.cpp
 * Use:   the implement of pageManage
 *************************************/
#include "pageManage.h"

/* Initialize static variables. */
bool pageManage::instanceFlag = false;
pageManage* pageManage::instance = NULL;

/* The default constructor. */
pageManage::pageManage() {
  instanceFlag = true;
}

/* The destructor. */
pageManage::~pageManage() {
  instanceFlag = false;
  instance = NULL;
}

/* Instantiation the object. */
pageManage* pageManage::getInstance() {
  /* Determine whether instantiated. */
  if (!instanceFlag)
    instance = new pageManage();
  return instance;
}

/* Load orders.tbl. */
bool pageManage::loadData_o() {
  /* Open the relevant files. */
  FILE* pfile = fopen(PATH_OF_ORDERS, "rb");
  if (!pfile)
    return false;
  FILE *ofile = fopen(PATH_OF_O_ORDERKEY, "wb"),
  *cfile = fopen(PATH_OF_O_CUSTKEY, "wb"),
  *tfile = fopen(PATH_OF_O_TOTALPRICE, "wb"),
  *sfile = fopen(PATH_OF_O_SHIPPRIORITY, "wb"),
  *tableFile = fopen(PATH_OF_QUERY_TABLE, "wb");
  if (!ofile || !cfile || !tfile || !sfile || !tableFile)
    return false;

  /* Create a query table to improve the query. */
  int queryTableSize = 0;
  int queryTable[MAX_SIZE_QUERY_TABLE];

  /* Traversing orders.tbl and load data. */
  int orderkeyData[MAX_ITEM],
  custkeyData[MAX_ITEM],
  shippriorityData[MAX_ITEM];
  double tolalpriceData[MAX_ITEM];
  int count = 0;
  int okey, ckey, spriority;
  char ostatus, odate[11], opriority[16], clerk[16], comment[80];
  double tprice;
  while (fscanf(pfile, "%d|%d|%c|%lf|%[^|]|%[^|]|%[^|]|%d|%[^|]|\n",
    &okey, &ckey, &ostatus, &tprice, odate, opriority, clerk,
    &spriority, comment) != EOF) {
    if (count == MAX_ITEM) {
      queryTable[queryTableSize++] = orderkeyData[0];
      fwrite(orderkeyData, sizeof(int), MAX_ITEM, ofile);
      fwrite(custkeyData, sizeof(int), MAX_ITEM, cfile);
      fwrite(tolalpriceData, sizeof(double), MAX_ITEM, tfile);
      fwrite(shippriorityData, sizeof(int), MAX_ITEM, sfile);
      count = 0;
    }
    orderkeyData[count] = okey;
    custkeyData[count] = ckey;
    tolalpriceData[count] = tprice;
    shippriorityData[count] = spriority;
    ++count;
  }
  if (count != 0) {
    queryTable[queryTableSize++] = orderkeyData[0];
    fwrite(orderkeyData, sizeof(int), count, ofile);
    fwrite(custkeyData, sizeof(int), count, cfile);
    fwrite(tolalpriceData, sizeof(double), count, tfile);
    fwrite(shippriorityData, sizeof(int), count, sfile);
  }
  fwrite(queryTable, sizeof(int), queryTableSize, tableFile);

  /* Close the relevant files. */
  fclose(pfile);
  fclose(ofile);
  fclose(cfile);
  fclose(tfile);
  fclose(sfile);
  fclose(tableFile);
  return true;
}

/* Load customer.tbl. */
bool pageManage::loadData_c() {
  /* Open the relevant files. */
  FILE* pfile = fopen(PATH_OF_CUSTOMER, "rb");
  if (!pfile)
    return false;
  FILE *cfile = fopen(PATH_OF_C_CUSTKEY, "wb");
  if (!cfile)
    return false;

  /* Traversing orders.tbl and load data. */
  int custkeyData[MAX_ITEM], nationkey, ckey;
  char name[26], address[41], phone[16], mktsegment[11], comment[118];
  double acctbal;
  int count = 0;
  while (fscanf(pfile, "%d|%[^|]|%[^|]|%d|%[^|]|%lf|%[^|]|%[^|]|\n",
    &ckey, name, address, &nationkey, phone, &acctbal, mktsegment,
    comment) != EOF) {
    if (count == MAX_ITEM) {
      fwrite(custkeyData, sizeof(int), MAX_ITEM, cfile);
      count = 0;
    }
    custkeyData[count++] = ckey;
  }
  fwrite(custkeyData, sizeof(int), count, cfile);

  /* Close the relevant files. */
  fclose(pfile);
  fclose(cfile);
  return true;
}

/* Binary search for query. */
int pageManage::binarySearch(const int* array, int start, int end, int key) {
  int left, right, mid;
  left = start;
  right = end;
  while (left <= right) {
    mid = (left + right) / 2;
    if (key == array[mid]) {
      return mid;
    } else if (key < array[mid]) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }
  return -1;
}

/* Query by orderkey. */
bool pageManage::queryItemByOrderkey(
  /* Open the relevant files. */
  int orderkey, int& custkey, double& tolalprice, int& shippriority,
  int* queryTable, int queryTableSize ) {
  FILE* ofile = fopen(PATH_OF_O_ORDERKEY, "rb"),
      * cfile = fopen(PATH_OF_O_CUSTKEY, "rb"),
      * tfile = fopen(PATH_OF_O_TOTALPRICE, "rb"),
      * sfile = fopen(PATH_OF_O_SHIPPRIORITY, "rb");
  if (!ofile || !cfile || !tfile || !sfile)
    return false;

  /* Find the page of the orderkey. */
  int position = 0;
  for (int pageId = 0; pageId < queryTableSize; ++pageId) {
    if (orderkey == queryTable[pageId]) {
      fseek(cfile , position * sizeof(int), SEEK_SET);
      fseek(tfile , position * sizeof(double), SEEK_SET);
      fseek(sfile , position * sizeof(int), SEEK_SET);
      fread(&custkey, sizeof(int), 1, cfile);
      fread(&tolalprice, sizeof(double), 1, tfile);
      fread(&shippriority, sizeof(int), 1, sfile);
      return true;
    } else if (orderkey < queryTable[pageId]) {
      break;
    }
    position += MAX_ITEM;
  }
  position -= MAX_ITEM;
  int okeyPage[MAX_ITEM];

  /* Find the index of the orderkey in the page. */
  fseek(ofile, position * sizeof(int), SEEK_SET);
  int num = fread(okeyPage, sizeof(int), MAX_ITEM, ofile);
  int index = binarySearch(okeyPage, 0, num, orderkey);
  if (index == -1)
    return false;
  fseek(cfile , (position+index) * sizeof(int), SEEK_SET);
  fseek(tfile , (position+index) * sizeof(double), SEEK_SET);
  fseek(sfile , (position+index) * sizeof(int), SEEK_SET);
  fread(&custkey, sizeof(int), 1, cfile);
  fread(&tolalprice, sizeof(double), 1, tfile);
  fread(&shippriority, sizeof(int), 1, sfile);

  /* Close the relevant files. */
  fclose(ofile);
  fclose(cfile);
  fclose(tfile);
  fclose(sfile);
  return true;
}

/* Quick sort for external sort. */
void pageManage::quickSort(int key[], int otherField[], int size) {
  int i = 0, j = size - 1;
  int val = key[0], record = otherField[0];
  if (size <= 1)
    return;
  while (i < j) {
    for (; j > i; --j) {
      if (key[j] < val) {
        key[i] = key[j];
        otherField[i] = otherField[j];
        break;
      }
    }
    for (; i < j; ++i) {
      if (key[i] > val) {
        key[j] = key[i];
        otherField[j] = otherField[i];
        break;
      }
    }
  }
  key[i] = val;
  otherField[i] = record;
  quickSort(key, otherField, i);
  quickSort(key+i+1, otherField+i+1, size-1-i);
}

/* Get the name of temporary files */
char* pageManage::getTempName(int index) {
  char *tempfile = new char[MAX_SIZE_TEMPNAME];
  snprintf(tempfile, MAX_SIZE_TEMPNAME, "bin/temp%d.bin", index);
  return tempfile;
}

/* External sort for compress. */
bool pageManage::externalSort() {
  FILE* ofile = fopen(PATH_OF_O_ORDERKEY, "rb"),
  *cfile = fopen(PATH_OF_O_CUSTKEY, "rb");
  if (!ofile || !cfile)
    return false;

  /* Traversing the file and load 128 pages to quick sort. */
  int oDataP[MAX_ITEM*MAX_PAGE], cDataP[MAX_ITEM*MAX_PAGE];
  tuple tDataP[MAX_ITEM*MAX_PAGE];
  int tmpFileId = 0;
  while (!feof(ofile)) {
    FILE* tmpFile = fopen(getTempName(tmpFileId++), "wb");
    if (!tmpFile)
      return false;

    int num = fread(oDataP, sizeof(int), MAX_ITEM*MAX_PAGE, ofile);
    fread(cDataP, sizeof(int), MAX_ITEM*MAX_PAGE, cfile);
    quickSort(cDataP, oDataP, num);
    for (int i = 0; i < num; ++i) {
      tDataP[i].orderkey = oDataP[i];
      tDataP[i].custkey = cDataP[i];
    }
    fwrite(tDataP, sizeof(tuple), num, tmpFile);
    fclose(tmpFile);
  }
  fclose(ofile);
  fclose(cfile);

  /* Merge sort. */
  FILE** tmpFiles = new FILE*[tmpFileId],
  *ofileAfterSort = fopen(PATH_OF_ORDERKEY_AFTER_SORT, "wb"),
  *cfileAfterSort = fopen(PATH_OF_CUSTKEY_AFTER_SORT, "wb");
  if (!ofileAfterSort || !cfileAfterSort)
    return false;
  for (int i = 0; i < tmpFileId; ++i) {
    tmpFiles[i] = fopen(getTempName(i), "rb");
    if (!tmpFiles[i])
      return false;
  }
  int numOfEOF = 0;
  int* len = new int[tmpFileId];
  int* cur = new int[tmpFileId];
  tuple** head = new tuple*[tmpFileId];
  int okeyAfterSort[MAX_ITEM], ckeyAfterSort[MAX_ITEM], count = 0;
  for (int i = 0; i < tmpFileId; ++i) {
    head[i] = new tuple[MAX_ITEM];
    len[i] = fread(head[i], sizeof(tuple), MAX_ITEM, tmpFiles[i]);
    cur[i] = 0;
  }
  while (numOfEOF != tmpFileId) {
    tuple minMax(0, MINMAX);
    int min_index = 0;
    for (int i = 0; i < tmpFileId; ++i) {
      if (feof(tmpFiles[i]) && len[i] == cur[i])
        continue;
      if (head[i][cur[i]].custkey < minMax.custkey) {
        minMax = head[i][cur[i]];
        min_index = i;
      }
    }
    if (count == MAX_ITEM) {
      fwrite(okeyAfterSort, sizeof(int), count, ofileAfterSort);
      fwrite(ckeyAfterSort, sizeof(int), count, cfileAfterSort);
      count = 0;
    }
    okeyAfterSort[count] = minMax.orderkey;
    ckeyAfterSort[count] = minMax.custkey;
    ++count;
    ++cur[min_index];
    if (cur[min_index] == len[min_index] && len[min_index] != 0) {
      len[min_index] = fread(head[min_index], sizeof(tuple), MAX_ITEM, tmpFiles[min_index]);
      cur[min_index] = 0;
    }
    if (feof(tmpFiles[min_index]) && len[min_index] == cur[min_index])
      ++numOfEOF;
  }
  if (count != 0) {
    fwrite(okeyAfterSort, sizeof(int), count, ofileAfterSort);
    fwrite(ckeyAfterSort, sizeof(int), count, cfileAfterSort);
  }
  for (int i = 0; i < tmpFileId; ++i) {
    fclose(tmpFiles[i]);
    remove(getTempName(i));
  }
  fclose(ofileAfterSort);
  fclose(cfileAfterSort);
  delete []tmpFiles;
  for (int i = 0; i < tmpFileId; ++i)
    delete [](head[i]);
  delete []head;
  delete []len;
  delete []cur;
  return true;
}

/* Compress the custkey in orders. */
bool pageManage::compressCustkey() {
  /* External sort. */
  if (!externalSort())
    return false;

  /* Open the relevant files. */
  FILE* cfileAfterSort = fopen(PATH_OF_CUSTKEY_AFTER_SORT, "rb"),
  *cfileAfterCompress = fopen(PATH_OF_CUSTKEY_AFTER_COMPRESS, "wb");
  if (!cfileAfterSort || !cfileAfterCompress)
    return false;
  int ckeyAfterSort[MAX_ITEM], count = 0, length = 1, pre_ckey = -1;
  item ckeyAfterCompress[MAX_ITEM];

  /* Compress custkey sort. */
  while(!feof(cfileAfterSort)) {
    int num = fread(ckeyAfterSort, sizeof(int), MAX_ITEM, cfileAfterSort);
    for (int i = 0; i < num; ++i) {
      if (ckeyAfterSort[i] == pre_ckey) {
        ++length;
      } else if (pre_ckey != -1) {
        if (count == MAX_ITEM) {
          fwrite(ckeyAfterCompress, sizeof(item), count, cfileAfterCompress);
          count = 0;
        }
        ckeyAfterCompress[count++] = item(pre_ckey, length);
        length = 1;
      }
      pre_ckey = ckeyAfterSort[i];
    }
  }
  if (count != 0)
    fwrite(ckeyAfterCompress, sizeof(item), count, cfileAfterCompress);
  item last_item(pre_ckey, length);
  fwrite(&last_item, sizeof(item), 1, cfileAfterCompress);

  /* Close the relevant files and remove the temporary files. */
  fclose(cfileAfterSort);
  fclose(cfileAfterCompress);
  remove(PATH_OF_CUSTKEY_AFTER_SORT);
  return true;
}

/* Join orders and customer. */
bool pageManage::join() {
  /* Open the relevant files. */
  FILE* o_okeyfile = fopen(PATH_OF_ORDERKEY_AFTER_SORT, "rb"),
      * o_ckeyfile = fopen(PATH_OF_CUSTKEY_AFTER_COMPRESS, "rb"),
      * c_ckeyfile = fopen(PATH_OF_C_CUSTKEY, "rb");
  if (!o_okeyfile || !o_ckeyfile || !c_ckeyfile)
    return false;

  /* Join orders and customer. */
  item o_ckey;
  int c_ckey, o_okey;
  bool next_o_ckey = true, next_c_ckey = true;
  while (!feof(o_ckeyfile) && !feof(c_ckeyfile)) {
    if (next_o_ckey)
      fread(&o_ckey, sizeof(item), 1, o_ckeyfile);
    if (next_c_ckey)
      fread(&c_ckey, sizeof(int), 1, c_ckeyfile);
    if (o_ckey.value == c_ckey) {
      for (int i = 0; i < o_ckey.length; ++i) {
        fread(&o_okey, sizeof(int), 1, o_okeyfile);
        fprintf(stdout, "%d %d\n", o_ckey.value, o_okey);
      }
      next_c_ckey = true;
      next_o_ckey = true;
    } else if (o_ckey.value < c_ckey) {
      next_o_ckey = true;
      next_c_ckey = false;
    } else {
      next_o_ckey = false;
      next_c_ckey = true;
    }
  }

  /* Close the relevant files. */
  fclose(o_okeyfile);
  fclose(o_ckeyfile);
  fclose(c_ckeyfile);
  return true;
}

/* Count the orders. */
bool pageManage::count(int& num) {
  FILE* rfile = fopen(PATH_OF_CUSTKEY_AFTER_COMPRESS, "rb");
  if (!rfile)
    return false;
  item item_[MAX_ITEM];
  num = 0;
  while (!feof(rfile)) {
    int size = fread(item_, sizeof(item), MAX_ITEM, rfile);
    for (int i = 0; i < size; ++i)
      num += item_[i].length;
  }
  fclose(rfile);
  return true;
}

