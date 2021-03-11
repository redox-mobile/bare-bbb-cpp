.PHONY: all clean debug init prepare release

all:
	@qbs profile:am335x config:release

init:
	@qbs config --unset profiles.am335x-gcc
	@qbs config --unset profiles.am335x

prepare:
	@qbs config --unset profiles.am335x
	@qbs config profiles.am335x.qbs.toolchain "am335x-gcc"

release:
	@qbs profile:am335x config:release

clean:
	@rm -rvf default debug release
