# This Makefile finds all unittest sources matching test_*.cpp, builds the test
# and executes it. The result is printed to console and also written to
# test_*_result.txt.

# set up source / header include paths
INCLUDE_DIRS=unity/src ../src
INCLUDE_FLAGS=$(addprefix -I,${INCLUDE_DIRS})

# find test modules
TESTSRC=$(shell find -type f -name test_\*.cpp)
RESULTS=$(TESTSRC:.cpp=_result.txt)

# Clean up and run all tests
all: clean ${RESULTS}

# Run tests. No clean-up, so tests with existing result files are not executed
# again.
run: ${RESULTS}

# Clean up
.PHONY: clean
clean:
	rm -f ${RESULTS}

%_testexe: %.cpp
	gcc ${INCLUDE_FLAGS} $< unity/src/unity.c -o $@

# Generate a test result by building the test source and executing the test
%_result.txt: %_testexe
	$< | tee $@

