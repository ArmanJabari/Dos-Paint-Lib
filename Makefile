CC = gcc
CFLAGS = -Wall -O2
TOOLS_DIR = tools
BUILD_DIR = build

ifeq ($(OS),Windows_NT)
    EXE = .exe
    MKDIR = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    RMDIR = if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
else
    EXE = 
    MKDIR = mkdir -p $(BUILD_DIR)
    RMDIR = rm -rf $(BUILD_DIR)
endif

TARGETS = $(BUILD_DIR)/colorconv$(EXE) $(BUILD_DIR)/palconv$(EXE) $(BUILD_DIR)/spriteconv$(EXE)

.PHONY: all clean

all: | $(BUILD_DIR) $(TARGETS)

$(BUILD_DIR):
	@$(MKDIR)

$(BUILD_DIR)/%$(EXE): $(TOOLS_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	@$(RMDIR)
