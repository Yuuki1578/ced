.PHONY: clean

.PHONY: gcc-check

.PHONY: coreutils-check

.PHONY: bash-check

.PHONY: ar-check

.PHONY: ld-check

.PHONY: strip-check

clean: $(BUILD)
	@echo [INFO] Removing $< directory
	@rm -rf $<

check: gcc-check coreutils-check bash-check ar-check ld-check strip-check

gcc-check:
	@echo [INFO] Checking GCC availability
	@gcc --version

coreutils-check:
	@echo [INFO] Checking Coreutils availability
	@coreutils --version

bash-check:
	@echo [INFO] Checking Bash availability
	@bash --version

ar-check:
	@echo [INFO] Checking Archiver availability
	@ar --version

ld-check:
	@echo [INFO] Checking Linker availability
	@ld --version

strip-check:
	@echo [INFO] Checking strip availability
	@strip --version
