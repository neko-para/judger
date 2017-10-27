ifeq ($(shell uname -o), GNU/Linux)
OS=linux
else
OS=windows
endif

ifeq ($(OS), windows)
LDFLAGS+=-lpsapi -static-libgcc -static-libstdc++
EXESUF=.exe
endif

all: CONFIG libtest.a main$(EXESUF)

CONFIG:
	./config $(OS)

clean:
	-rm -f main$(EXESYUF) libtest.a
	-rm -f test.o main.o

test.o: test.cpp test.h

libtest.a: test.o
	ar r libtest.a test.o

main.o: main.cpp test.h

main$(EXESUF): main.o libtest.a
	$(CXX) main.o libtest.a -o main$(EXESUF) $(CXXFLAGS) $(LDFLAGS)