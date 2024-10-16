CC = g++
STD = -std=c++23
CFLAGS = -O1 -Iinclude -Wall -Wextra -pedantic $(STD) -MMD -MP -g
LIBS = $(shell pkg-config --cflags --libs wayland-client)
NAME = llkit
TARGET = libllkit.so
SRC_DIR = src 
BUILD_DIR = build
INCLUDE_DIR = include
LIB_SRCS := $(shell find src -name "*.cpp")
LIB_OBJS := $(patsubst src/%.cpp, build/%.o, $(LIB_SRCS))
LIB_HEADERS := $(shell find include -name "*.h")
BUILD_DIRS = $(dir $(LIB_OBJS))
BUILD_DIRS += build/protocols
USR_LIB := $(shell pkgconf --variable=libdir wayland-client)
USR_INCLUDE := $(shell pkgconf --variable=includedir wayland-client)
DEP := $(LIB_OBJS:.o=.d)

WL_PROTOCOLS_DIR = $(INCLUDE_DIR)/protocols
WL_DIR = $(shell pkgconf --variable=pkgdatadir wayland-protocols)
WL_FILES := stable/xdg-shell/xdg-shell.xml

WL_BUILD_DIR = build/protocols
WL_OBJS := $(patsubst %.xml, $(WL_BUILD_DIR)/%-client.o, $(notdir $(WL_FILES)))
WL_HEADERS := $(patsubst %.xml, $(WL_BUILD_DIR)/%-client.h, $(notdir $(WL_FILES)))

all: 
	$(MAKE) mkdir_build
	$(MAKE) gen_wayland_files
	$(MAKE) $(WL_OBJS) $(TARGET)

-include $(DEP)

gen_wayland_files: mkdir_build
	$(info :: generating wayland files)
	@for i in $(WL_FILES); do\
		j=$$(basename -s .xml $$i);\
		echo :::: $$i;\
		wayland-scanner client-header $(WL_DIR)/$$i $(WL_PROTOCOLS_DIR)/$$j-client.h;\
		wayland-scanner private-code $(WL_DIR)/$$i $(WL_PROTOCOLS_DIR)/$$j-client.c;\
	done

build/protocols/%.o : include/protocols/%.c
	$(info :: building wayland files)
	cc -MMD -MP -O1 -c $< -o $@


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
		--suppress=cstyleCast -Iinclude -i$(WL_PROTOCOLS_DIR) $(SRC_DIR)

format:
	clang-format -i $(LIB_SRCS) $(LIB_HEADERS)
