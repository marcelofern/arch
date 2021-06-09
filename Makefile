.PHONY: install

all: install

install:
	@echo "Installing arch binary..."
	@sudo gcc -std=c17 -Wall -Wextra -Wpedantic src/* -o /usr/local/bin/arch
	@echo "arch binary installed!"
