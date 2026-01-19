TARGET	:= main # 最终生成的可执行文件名
OBJS	:= main.o swap.o add.o max.o # 编译过程需要的目标文件
COMPILE	:= gcc # 使用的编译器

$(TARGET) : $(OBJS)
	$(COMPILE) -o $(TARGET) $(OBJS)
clean :
	rm -rf $(TARGET) $(OBJS)