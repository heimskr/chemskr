COMPILER     ?= clang++
OPTIMIZATION ?= -O0 -g
CPPFLAGS     := -std=c++20 -Wall -Wextra -Iinclude $(OPTIMIZATION)
OUTPUT       := chemskr_test

LEXFLAGS     := -Wno-sign-compare -Wno-register
LEXCPP       := src/chemskr/yylex.cpp
PARSECPP     := src/chemskr/yyparse.cpp
PARSEHDR     := include/chemskr/yyparse.h
PARSEHDR_SRC := src/chemskr/yyparse.h
FLEXSRC      := src/chemskr/formula.l
BISONSRC     := src/chemskr/formula.y

SOURCES      := $(shell find src/chemskr -name '*.cpp')
OBJECTS      := $(SOURCES:.cpp=.o) $(LEXCPP:.cpp=.o) $(PARSECPP:.cpp=.o)

.PHONY: all clean

all: $(OUTPUT)

test: $(OUTPUT)
	./$(OUTPUT)

%.o: %.cpp $(PARSEHDR)
	$(COMPILER) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OUTPUT) $(shell find src -name '*.o') $(PARSEHDR) $(PARSECPP) $(LEXCPP) $(PARSECPP:.c=.output) $(LEXCPP) $(PARSECPP)

$(OUTPUT): $(OBJECTS) src/main.o
	$(COMPILER) -o $@ $^

$(LEXCPP): $(FLEXSRC) $(PARSEHDR)
	flex --prefix=chemskr --outfile=$(LEXCPP) $(FLEXSRC)

$(PARSECPP) $(PARSEHDR): $(BISONSRC)
	bison --color=always --defines=$(PARSEHDR) --output=$(PARSECPP) $(BISONSRC)
	cp $(PARSEHDR) $(PARSEHDR_SRC)

$(LEXCPP:.cpp=.o): $(LEXCPP)
	$(COMPILER) $(CPPFLAGS) $(LEXFLAGS) -c $< -o $@

$(PARSECPP:.cpp=.o): $(PARSECPP) $(PARSEHDR)
	$(COMPILER) $(CPPFLAGS) $(LEXFLAGS) -c $< -o $@

DEPFILE  = .dep
DEPTOKEN = "\# MAKEDEPENDS"
DEPFLAGS = -f $(DEPFILE) -s $(DEPTOKEN)

depend:
	@ echo $(DEPTOKEN) > $(DEPFILE)
	makedepend $(DEPFLAGS) -- $(COMPILER) $(CFLAGS) -- $(SOURCES) 2>/dev/null
	@ rm $(DEPFILE).bak

sinclude $(DEPFILE)
