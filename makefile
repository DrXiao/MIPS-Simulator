TARGET=main
SRC_DIR=src
INCLUDE_DIR=include
FORWARDING=forwarding
STALL=stall

CC=g++
CFLAGS= -g -o

forwarding:
	$(CC) $(CFLAGS) $(TARGET) $(SRC_DIR)/$(FORWARDING)/*.cpp -I$(INCLUDE_DIR)

win_forwarding:
	$(CC) $(CFLAGS) $(TARGET).exe $(SRC_DIR)/$(FORWARDING)/*.cpp -I$(INCLUDE_DIR)

stall:
	$(CC) $(CFLAGS) $(TARGET) $(SRC_DIR)/$(STALL)/*.cpp -I$(INCLUDE_DIR)

win_stall:
	$(CC) $(CFLAGS) $(TARGET).exe $(SRC_DIR)/$(STALL)/*.cpp -I$(INCLUDE_DIR)