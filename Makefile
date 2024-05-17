# Makefile

# 编译器
CXX = g++

# 编译器选项
CXXFLAGS = -Ilab1 -Ilab2

# 源文件
SRCS = whole.cpp lab1\lexi_anl.cpp lab1\lexi_class.cpp lab2\cfg_LR1.cpp lab2\predictTable.cpp lab2\syntax_anl.cpp

# 目标文件
OBJS = $(SRCS:.cpp=.o)

# 可执行文件
TARGET = whole

# 规则
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del $(OBJS) $(TARGET)

.PHONY: all clean
