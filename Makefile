# Wrapper makefile, internally mostly calls the platformio targets.

.PHONY: all
all:
	pipenv run pio run

.PHONY: upload
upload:
	pipenv run pio run -t upload

.PHONY: clean
clean:
	pipenv run pio run -t clean

.PHONY: doc
doc:
	cd tools && doxygen

.PHONY: check
check:
	cppcheck --enable=all src

.PHONY: tests
tests:
	(cd tests && ${MAKE})
