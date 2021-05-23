.PHONY: install

all: install

install:
	@echo "Installing arch binary..."
	@sudo gcc src/* -o /usr/local/bin/arch
	@echo "arch binary installed!"
