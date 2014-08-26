CUR_DIR = $(shell pwd)
SRC_DIR = $(CUR_DIR)/src
BIN_DIR = $(CUR_DIR)/bin
OBJ = pageManage.o \
			cStoreService.o \
			cStoreController.o \
			cStore.o
EXE = db
CC = g++

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ)
	mv -f $(EXE) $(OBJ) $(BIN_DIR)

pageManage.o:
	$(CC) -c $(SRC_DIR)/pageManage.cpp
cStoreService.o:
	$(CC) -c $(SRC_DIR)/cStoreService.cpp
cStoreController.o:
	$(CC) -c $(SRC_DIR)/cStoreController.cpp
cStore.o:
	$(CC) -c $(SRC_DIR)/cStore.cpp

clean:
	rm -f $(BIN_DIR)/$(EXE)
	rm -f $(BIN_DIR)/*.o
