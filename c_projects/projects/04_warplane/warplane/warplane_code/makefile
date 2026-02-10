CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm

# 将新文件加入 OBJS 列表
OBJS = main.o warplane.o music_manager.o boss_images.o

all: warplane

warplane: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c warplane.h music_manager.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o warplane
