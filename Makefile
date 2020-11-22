.PHONY: all clean debug init prepare release

all:
	@qbs profile:am335x config:release

init:
	@qbs config --unset profiles.am335x-gcc
	@qbs config --unset profiles.am335x

prepare:
	@./configure

release:
	@qbs profile:am335x config:release

clean:
	@rm -rvf default debug release
