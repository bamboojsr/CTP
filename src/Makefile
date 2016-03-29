CC = g++
CFLAGS = -g -std=gnu++11
TARGET = all
all : SubscriptMart
SubscriptMart : SubscriptMart.cpp
	$(CC) $(CFLAGS) -o $@ $^ -lpthread -L . -lthostmduserapi -lthosttraderapi 

.PHONY : clean
clean :
	rm -f *.o SubscriptMart
