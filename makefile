TARGET=main
SRC_DIR=src
INCLUDE_DIR=include
FORWARDING=forwarding
STALL=stall

CC=g++
CFLAGS= -g -o

forwarding:
	$(CC) $(CFLAGS) $(TARGET) $(SRC_DIR)/$(FORWARDING)/*.cpp -I$(INCLUDE_DIR)/$(FORWARDING)

stall:
	$(CC) $(CFLAGS) $(TARGET) $(SRC_DIR)/$(STALL)/*.cpp -I$(INCLUDE_DIR)/$(STALL)