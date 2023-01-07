CC = gcc
CFLAGS = -DUNICODE -w -lgdi32
TARGET = Paint
SRCS = main.c
SRCS += IComponent.cpp
SRCS += Canvas.cpp

INCDIR = 
LIBDIR =
LIBS =

$(TARGET): $(SRCS)
	$(CC) -o $(TARGET)  $(SRCS) $(CFLAGS) 