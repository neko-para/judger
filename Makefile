CLS=compiler runner
OBJS=main.o $(patsubst %, %.o, $(CLS)) $(patsubst %, %.general.o, $(CLS))
ifeq ($(shell uname -o), GNU/Linux)
OS=linux
EXESUF=
else
OS=windows
EXESUF=.exe
endif

ifeq ($(OS), windows)
LDFLAGS+=-lpsapi -static-libgcc -static-libstdc++
EXESUF=.exe
endif

all: CONFIG main$(EXESUF)

CONFIG:
	./config $(OS)

clean:
	-rm -f main
	-rm -f $(OBJS)

%.o: %.cpp %.h

%.general.o: %.general.cpp %.h

main.o: main.cpp compiler.h runner.h

main$(EXESUF): $(OBJS)
	$(CXX) $(OBJS) -o main$(EXESUF) $(CXXFLAGS) $(LDFLAGS)