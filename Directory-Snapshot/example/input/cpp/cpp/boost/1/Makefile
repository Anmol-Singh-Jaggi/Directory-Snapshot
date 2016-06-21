.PHONY: all run clean

source = main

all: $(source)

run: $(source)
	./$(source)

clean:
	rm $(source).o $(source)

$(source): $(source).cpp
	bash BUILD.sh
