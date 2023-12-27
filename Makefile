PROJECT = server
TEST = server_tests

CXX = g++
CXXFLAGS = -Wall -Ofast
LDFLAGS = -lboost_program_options -lcryptopp

COMLINE_OPTIONS = -b vcalc.conf -e vcalc.log -p 33333

TESTS = tests.cpp

SOURCES = $(filter-out $(TESTS),$(wildcard *.cpp))
HEADERS = $(wildcard *.h)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

TEST_OBJECTS = $(filter-out main.o,$(OBJECTS))


.PHONY: all clean test run

all: $(PROJECT)

test: $(TEST)
	./$(TEST)

run: $(PROJECT)
	./$^ $(COMLINE_OPTIONS)


$(PROJECT): $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(TEST): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -DUNIT_TEST_TEST $(TESTS) $^ $(LDFLAGS) -lUnitTest++ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJECTS): $(HEADERS)


clean:
	rm -f $(PROJECT) $(TEST) *.o
