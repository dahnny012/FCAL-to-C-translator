CXX_DIR = ../lib/cxxtest
CXXTEST = $(CXX_DIR)/bin/cxxtestgen
CXXFLAGS = --error-printer --abort-on-fail --have-eh

FLAGS = -Wall -g

all: run-tests
# Program files.
AST.o: AST.h AST.cpp
	g++ $(FLAGS) -c AST.cpp

readInput.o:	readInput.cpp readInput.h
	g++ $(FLAGS) -c readInput.cpp 

regex.o:	regex.cpp regex.h
	g++ $(FLAGS) -c regex.cpp 

scanner.o:	scanner.cpp scanner.h regex.h
	g++ $(FLAGS) -c scanner.cpp 

parser.o: parser.h parser.cpp
	g++ $(FLAGS) -c parser.cpp
	
parseResult.o: parseResult.h parseResult.cpp
	g++ $(FLAGS) -c parseResult.cpp
	
extToken.o: extToken.h extToken.cpp
	g++ $(FLAGS) -c extToken.cpp

# Testing files and targets.
run-tests: regex_tests scanner_tests parser_tests ast_tests codegeneration_tests
	./regex_tests
	./scanner_tests
	./parser_tests
	./ast_tests
	./codegeneration_tests

regex_tests: regex_tests.cpp regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -o regex_tests regex.o regex_tests.cpp

regex_tests.cpp: regex.h regex_tests.h
	$(CXXTEST) $(CXXFLAGS) -o regex_tests.cpp regex_tests.h

scanner_tests: scanner_tests.cpp scanner.o regex.o readInput.o
	g++ $(FLAGS) -I$(CXX_DIR)  -o scanner_tests \
	scanner.o regex.o readInput.o scanner_tests.cpp
		
scanner_tests.cpp:	scanner.o scanner_tests.h readInput.h
	$(CXXTEST) $(CXXFLAGS) -o scanner_tests.cpp scanner_tests.h

parser_tests: parser.o scanner.o regex.o parser_tests.cpp readInput.o parseResult.o extToken.o AST.o
	g++ $(FLAGS) -I$(CXX_DIR)  -o parser_tests \
	regex.o readInput.o scanner.o parser.o parser_tests.cpp parseResult.o extToken.o AST.o

parser_tests.cpp: parser.o regex.o parser_tests.h readInput.o parseResult.o extToken.o
	$(CXXTEST) $(CXXFLAGS) -o parser_tests.cpp parser_tests.h
 
ast_tests: ast_tests.cpp readInput.o parser.o parseResult.o extToken.o scanner.o regex.o AST.o
	g++ $(FLAGS) -I$(CXX_DIR) -o ast_tests \
	readInput.o parser.o parseResult.o extToken.o scanner.o regex.o ast_tests.cpp AST.o
    
ast_tests.cpp: AST.h ast_tests.h
	$(CXXTEST) $(CXXFLAGS) -o ast_tests.cpp ast_tests.h

codegeneration_tests.cpp: codegeneration_tests.h AST.h AST.cpp
	$(CXXTEST) $(CXXFLAGS) -o codegeneration_tests.cpp codegeneration_tests.h

codegeneration_tests: readInput.o parser.o parseResult.o extToken.o scanner.o regex.o AST.o codegeneration_tests.cpp
	g++ $(FLAGS) -I$(CXX_DIR) -o codegeneration_tests \
	readInput.o parser.o parseResult.o extToken.o scanner.o regex.o AST.o codegeneration_tests.cpp;

exp: scanner.o regex.o
	g++ -g -o exp scanner.o regex.o
	


clean:
	rm -Rf *.o \
	regex_tests regex_tests.cpp \
	scanner_tests scanner_tests.cpp \
	parser_tests parser_tests.cpp \
	ast_tests ast_tests.cpp \
	codegeneration_tests codegeneration_tests.cpp

