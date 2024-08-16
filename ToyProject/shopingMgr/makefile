CC = clang -Wall
# 查找src下的所有的后缀为.c的文件
C_SROUCE = $(wildcard src/*.c)  #主程序main.c不要放到src中了

# 将所有的.c源文件的后缀名修改成.o
C_OBJ = $(patsubst %.c,%.o,$(C_SROUCE))

# 当前路径
CUR_DIR = $(shell pwd)

# 用静态库编译 生成的程序
C_BIN_A = ./bin/shop_a
# 用动态库编译 生成的程序
C_BIN_SO = ./bin/shop_so

# 主程序文件
MAIN = main.c

# 配置头文件的路径
HFLAGS = -I ./inc

# 使用静态库 编译时, 指定静态库路径
LFLAGS_A = -L ./lib/static
# 使用动态库 编译时, 指定动态库路径
LFLAGS_SO = -L ./lib/dynamic

# 配置静态文件存放路径
OBJ_D = ./obj/.

# 拼接指令: -I ./inc  -L ./lib/*
# 静态库编译选项
C_FLAGS_A = $(HFLAGS) $(LFLAGS_A)  -lshop
# 动态库编译选项
C_FLAGS_SO = $(HFLAGS) $(LFLAGS_SO)  -lshop

# 静态库文件存放的路径
LIB_A_D = ./lib/static/
# 动态库文件存放的路径
LIB_SO_D = ./lib/dynamic/

# 静态库的名称
LIB_A = libshop.a 
# 动态库的名称
LIB_SO = libshop.so

all: $(LIB_A) $(LIB_SO) $(C_BIN_A) $(C_BIN_SO)

# 生成静态库
$(LIB_A): $(C_OBJ)
	# 将所有的静态文件打包成静态库文件
	ar cr $@ $^
	# 将静态库文件移动到./lib/static/. 下
	mv $(LIB_A) $(LIB_A_D)
	# 编译程序之后，需要将生成.o文件，移动到obj中
	mv ./src/*.o $(OBJ_D)

# 生成动态库
$(LIB_SO): $(C_SROUCE)
	# 生成动态库文件
	$(CC) -shared -fPIC $^ -o $@ $(HFLAGS)
	# 生成的动态库文件移动到项目的./lib/dynamic/. 下
	mv $(LIB_SO) $(LIB_SO_D)
	# 为动态库文件 建立软连接
	sudo ln -s $(CUR_DIR)/$(LIB_SO_D)$(LIB_SO)  /lib/$(LIB_SO)
	# 刷新动态库缓存
	sudo ldconfig

# 使用静态库 编译程序
$(C_BIN_A): $(MAIN)
	$(CC) $^ -o $@ $(C_FLAGS_A)

# 使用动态库 编译程序
$(C_BIN_SO): $(MAIN)
	$(CC) $^ -o $@ $(C_FLAGS_SO)

# 指定将.c文件转换为.o文件  
%.o:%.c
	$(CC) $^ -o $@  -c

.PHONY:
clean:
	$(RM) $(C_BIN_A) $(C_BIN_SO) $(LIB_A_D)$(LIB_A) $(LIB_SO_D)$(LIB_SO) $(OBJ_D)/*
	sudo unlink /lib/$(LIB_SO)
