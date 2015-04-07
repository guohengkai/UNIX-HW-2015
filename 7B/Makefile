CC := g++
PREFIX = .
INCS :=  $(PREFIX)
CFLAGS := -Wall -Wunreachable-code -Werror -Wsign-compare -g -fPIC -std=c++11
SOURCE := $(wildcard  *.cpp ../6A/csv_handler.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SOURCE))
TARGETS=csv_map_reduce

all:$(TARGETS)
$(TARGETS) : $(OBJS)
	$(CC) -o $@ $^ $(LIBS) \
		-Wl,-rpath,/usr/lib/gcc/x86_64-linux-gnu/4.7.3/,-rpath,$(PREFIX)/lib
%.o:%.cpp	
	$(CC) $(CFLAGS) $(addprefix -I,$(INCS)) -c $< -o $@
clean:
	rm -rf $(TARGETS) $(OBJS)
test:
	cd ../6A
	make
	cd ../7B
	./$(TARGETS) ../6A/test.csv test.bin
	../6A/csv_convert_print test_0.bin
	../6A/csv_convert_print test_1.bin
	../6A/csv_convert_print test_2.bin
	../6A/csv_convert_print test.bin

