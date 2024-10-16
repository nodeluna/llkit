CC = g++
STD = -std=c++23
CFLAGS = -O1 -Iinclude -Wall -Wextra -pedantic $(STD) -MMD -MP
LIBS = $(shell pkg-config --cflags --libs wayland-client)
NAME = llkit
TARGET = libllkit.so
SRC_DIR = src 
BUILD_DIR = build
INCLUDE_DIR = include
LIB_SRCS := $(shell find src -name "*.cpp")
LIB_OBJS := $(patsubst src/%.cpp, build/%.o, $(LIB_SRCS))
BUILD_DIRS = $(dir $(LIB_OBJS))
BUILD_DIRS += build/protocols
USR_LIB := $(shell pkgconf --variable=libdir wayland-client)
USR_INCLUDE := $(shell pkgconf --variable=includedir wayland-client)
DEP := $(LIB_OBJS:.o=.d)

WL_PROTOCOLS_DIR = protocols
WL_DIR = $(shell pkgconf --variable=pkgdatadir wayland-protocols)
WL_FILES := 

WL_BUILD_DIR = build/protocols
WL_OBJS := $(patsubst %.xml, $(WL_BUILD_DIR)/%.o, $(notdir $(WL_FILES)))

all: mkdir_build gen_wayland_files $(WL_OBJS) $(TARGET)

-include $(DEP)

gen_wayland_files:
	$(info :: generating wayland files)
	@for i in $(WL_FILES); do\
		j=$$(basename -s .xml $$i);\
		echo :::: $$i;\
		wayland-scanner client-header $(WL_DIR)/$$i $(WL_PROTOCOLS_DIR)/$$j.h;\
		wayland-scanner private-code $(WL_DIR)/$$i $(WL_PROTOCOLS_DIR)/$$j.c;\
	done

build/protocols/%.o : protocols/%.c | mkdir_build
	$(info :: building wayland files)
	gcc -MMD -MP -O1 -c $< -o $@


build/%.o : src/%.cpp | mkdir_build
	$(info :: building library)
	$(CC) $(CFLAGS) $(LIBS) -fPIC -c $< -o $@

$(TARGET) : $(LIB_OBJS)
	$(info :: building target)
	$(CC) $(CFLAGS) $(LIBS) -shared -o $@ $^

install:
	$(info :: installing)
	install -Dm755 $(TARGET) $(USR_LIB)/$(TARGET)
	cp -r $(INCLUDE_DIR)/ $(USR_INCLUDE)/$(NAME)
	install -Dm755 $(NAME).pc $(USR_LIB)/pkgconfig/$(NAME).pc

uninstall:
	rm $(USR_LIB)/$(TARGET)
	rm $(USR_LIB)/pkgconfig/$(NAME).pc
	rm -r $(USR_INCLUDE)/$(NAME)

.PHONY: help
.SILENT: help
help:
	$(info :: all)
	$(info :: client)
	$(info :: server)
	$(info :: clean)
	$(info :: cppcheck)
	$(info :: mkdir_build)
	$(info :: gen_wayland_files)
	@for i in $(LIB_OBJS) $(WL_OBJS); do\
		echo :: $$i;\
	done

mkdir_build:
	@for i in $(BUILD_DIRS) $(WL_PROTOCOLS_DIR); do\
		mkdir -p $$i;\
	done

clean:
	$(info :: cleaning build)
	@for i in $(TARAGET) $(BUILD_DIR) $(WL_PROTOCOLS_DIR); do\
		rm -r $$i;\
	done


cppcheck:
	$(info :: running static code analysis)
	$(info  )
	cppcheck --cppcheck-build-dir=build --std=c++23 --check-level=exhaustive\
		--suppress=unreadVariable --suppress=unusedFunction --suppress=missingIncludeSystem --enable=all\
		-Iinclude $(SRC_DIR)

format:
	clang-format -i src/*/* include/*/*
