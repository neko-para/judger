CLS=compiler runner
OBJS=main.o $(patsubst %, %.o, $(CLS))

all: CONFIG main

CONFIG:
	./config linux


clean:
	-rm -f main
	-rm -f $(OBJS)

%.o: %.cpp %.h
	g++ -c $< -o $@ -g

main.o: main.cpp compiler.h runner.h
	g++ -c $< -o $@ -g

main: $(OBJS)
	g++ $(OBJS) -o main