CC = gcc
CFLAGS = -Wall -O2
# Default MySQL flags; on Linux you may use `mysql_config --cflags --libs`
MYSQL_CFLAGS = -I/usr/include/mysql
MYSQL_LIBS = -lmysqlclient

SRCS = main.c address_book.c
OBJS = $(SRCS:.c=.o)
TARGET = address_book

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(MYSQL_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(MYSQL_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
