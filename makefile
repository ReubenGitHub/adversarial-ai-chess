CC=g++
CFLAGS=-Wall -Wextra

SOURCES=$(shell find . -name "*.cpp")
EXECUTABLE=play-chess
COMPILED_DIR=./compiled
TARGET_FILE=$(COMPILED_DIR)/$(EXECUTABLE)

$(EXECUTABLE):
	$(CC) $(CFLAGS) -o $(TARGET_FILE) $(SOURCES)

run:
	$(TARGET_FILE)

clean:
	rm $(TARGET_FILE)
