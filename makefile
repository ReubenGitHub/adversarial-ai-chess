CC=g++
CFLAGS=-Wall -Wextra

SOURCES=$(shell find . -name "*.cpp")
EXECUTABLE=play-chess
BIN_DIR=./bin
TARGET_FILE=$(BIN_DIR)/$(EXECUTABLE)

$(EXECUTABLE):
	$(CC) $(CFLAGS) -o $(TARGET_FILE) $(SOURCES)

run:
	$(TARGET_FILE)

clean:
	rm $(TARGET_FILE)
