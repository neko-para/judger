ifeq ($(shell uname -o), GNU/Linux)
OS=linux
else
OS=windows
endif

ifeq ($(OS), windows)
LDFLAGS+=-lpsapi -static-libgcc -static-libstdc++
EXESUF=.exe
endif
CHECKER=$(patsubst checker/%.cpp, checker/%$(EXESUF), $(wildcard checker/*.cpp))
all: CONFIG libtest.a main$(EXESUF) $(CHECKER)

CONFIG:
	./config $(OS)

clean:
	-rm -f main$(EXESYUF) libtest.a
	-rm -f test.o procres.o main.o

%.o: %.cpp %.h

libtest.a: test.o procres.o
	$(AR) r libtest.a test.o procres.o

main.o: main.cpp test.h

main$(EXESUF): main.o libtest.a
	$(CXX) main.o libtest.a -o main$(EXESUF) $(CXXFLAGS) $(LDFLAGS)

checker/%$(EXESUF): checker/%.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)