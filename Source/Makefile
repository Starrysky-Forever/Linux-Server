
.PHONY:clean install distclean output

INCLUDE_PATH=../include #存储头文件路径
CFLAGS= -I$(INCLUDE_PATH) -g -c -Wall#存储编译时选项
CPPFLAGES=-E -D -O1 -O2 -O3#存储预处理选项
LIBRARY_PATH=../library
LDFLAGS=-lpthread#存储库选项 -L库位置 -l库名（掐头去尾）
TARGET=thread_pool_server#存储目标名
CC=gcc #存储编译器
INSTALL_PATH= /usr/bin/#程序安装路径

SRCFILE=$(wildcard *.c)
DESFILE=$(patsubst %.c,%.o,$(SRCFILE))

CP=sudo cp
RM=sudo rm -rf

$(TARGET):$(DESFILE)
	$(CC) $^ $(LDFLAGS) -o $@

%.o:%.c
	$(CC) $(CFLAGS) $<
					 
clean:
	$(RM) $(DESFILE) $(TARGET)

install:
	$(CP) $(TARGET) $(INSTALL_PATH)

distclean:
	$(RM)$(INSTALL_PATH)$(TARGET) 

output:
	-@echo "test message.."
