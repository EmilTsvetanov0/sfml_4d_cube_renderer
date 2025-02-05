BUILD_DIR ?= build
EXECUTABLE ?= cube_rotation.exe

# all clean rebuild
# configure build run

ifeq ($(OS),Windows_NT)
    RM = rmdir /S /Q
    MKDIR = mkdir
    RUN = .\\
else
    RM = rm -rf
    MKDIR = mkdir -p
    RUN = ./
endif

.PHONY: all
all: build run

.PHONY: configure
configure:
	$(MKDIR) $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..

.PHONY: build
build: configure
	cmake --build $(BUILD_DIR)

.PHONY: run
run:
	$(RUN)$(BUILD_DIR)/$(EXECUTABLE)

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)

.PHONY: rebuild
rebuild: clean all