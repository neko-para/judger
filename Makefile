ifeq ($(shell uname), Linux)
OS=posix
else ifeq ($(shell uname), Darwin)
OS=posix
else
OS=win32
endif

ifeq ($(OS), win32)
LDFLAGS+=-lpsapi
EXESUF=.exe
endif
CHECKER=$(patsubst checker/%.cpp, out/%$(EXESUF), $(wildcard checker/*.cpp))
OBJS=$(patsubst src/%.cpp, tmp/%.o, $(wildcard src/*.cpp))

all: dirs tmp/libtest.a out/judger$(EXESUF) out/$(CHECKER)

clean:
	-rm -rf tmp
	-rm -rf out

dirs: tmp out

.PHONY: all clean dirs

out:
	mkdir -p out

tmp:
	mkdir -p tmp

tmp/%.o: libtest.$(OS)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

tmp/libtest.a: tmp/test.o tmp/procres.o
	$(AR) r tmp/libtest.a tmp/test.o tmp/procres.o

out/%$(EXESUF): checker/%.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

tmp/%.o: src/%.cpp $(wildcard *.h)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

out/judger$(EXESUF): tmp/libtest.a $(OBJS)
	$(CXX) $(OBJS) -o $@ $(CXXFLAGS) $(LDFLAGS) -lncurses -ltinfo tmp/libtest.a
