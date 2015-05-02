CC := g++
PREFIX = .
INCS :=  $(PREFIX)
CFLAGS := -Wall -Wunreachable-code -Werror -Wsign-compare -g -fPIC -std=c++11
SOURCE := $(wildcard  *.cpp ../6A/csv_handler.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SOURCE))
TARGETS=chatroom

all:$(TARGETS)
$(TARGETS) : $(OBJS)
	$(CC) -o $@ $^ $(LIBS) \
		-Wl,-rpath,/usr/lib/gcc/x86_64-linux-gnu/4.7.3/,-rpath,$(PREFIX)/lib
%.o:%.cpp	
	$(CC) $(CFLAGS) $(addprefix -I,$(INCS)) -c $< -o $@
clean:
	rm -rf $(TARGETS) $(OBJS)
test:
	./$(TARGETS) s
	gnome-terminal -x bash -c "./$(TARGETS) c 127.0.0.1 ghk"
	gnome-terminal -x bash -c "./$(TARGETS) c 127.0.0.1"
	gnome-terminal -x bash -c "./$(TARGETS) c 127.0.0.1"
