CC = g++
STD = -std=c++23
CFLAGS = -O1 -Iinclude -Wall -Wextra -pedantic $(STD) -MMD -MP -Wno-unused-parameter -g
LIBS = $(shell pkgconf --cflags --libs wayland-client)\
       $(shell pkgconf --cflags --libs xkbcommon)\
       $(shell pkgconf --cflags --libs wayland-egl)\
       $(shell pkgconf --cflags --libs gl)\
       $(shell pkgconf --cflags --libs egl)
NAME = llkit
TARGET = libllkit.so
SRC_DIR = src 
BUILD_DIR = build
INCLUDE_DIR = include
LIB_SRCS := $(shell find src -name "*.cpp")
PREFIX = /usr
LIB_OBJS := $(patsubst src/%.cpp, build/%.o, $(LIB_SRCS))
LIB_HEADERS := $(shell find include -name "*.h")
BUILD_DIRS = $(dir $(LIB_OBJS))
BUILD_DIRS += build/protocols
DEP := $(LIB_OBJS:.o=.d)

WL_PROTOCOLS_DIR = $(INCLUDE_DIR)/protocols
WL_DIR = $(shell pkgconf --variable=pkgdatadir wayland-protocols)
WL_FILES := stable/xdg-shell/xdg-shell.xml

WL_BUILD_DIR = build/protocols
WL_OBJS := $(patsubst %.xml, $(WL_BUILD_DIR)/%-client.o, $(notdir $(WL_FILES)))
WL_HEADERS := $(patsubst %.xml, $(WL_BUILD_DIR)/%-client.h, $(notdir $(WL_FILES)))

ifneq ($(findstring clang++, $(CC)),)
        CFLAGS += $(CLAGS) -D__cpp_concepts=202002L -Wno-builtin-macro-redefined -Wno-macro-redefined
endif

all: 
	$(MAKE) mkdir_build
	$(MAKE) gen_wayland_files
	$(MAKE) $(WL_OBJS) $(TARGET)

-include $(DEP)

gen_wayland_files:
	$(info :: generating wayland files)
	@for i in $(WL_FILES); do\
		j=$$(basename -s .xml $$i);\
		echo :::: $$i;\
		if [ ! -f "$(WL_PROTOCOLS_DIR)/$$j-client.h" ]; then\
			wayland-scanner client-header $(WL_DIR)/$$i $(WL_PROTOCOLS_DIR)/$$j-client.h;\
		fi;\
		if [ ! -f "$(WL_PROTOCOLS_DIR)/$$j-client.c" ]; then\
			wayland-scanner private-code $(WL_DIR)/$$i $(WL_PROTOCOLS_DIR)/$$j-client.c;\
		fi;\
	done

build/protocols/%.o : include/protocols/%.c
	$(info :: building wayland files)
	cc -g -MMD -MP -O1 -c $< -o $@


build/%.o : src/%.cpp
	$(info :: building library)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(TARGET) : $(LIB_OBJS)
	$(info :: building target)
	$(CC) $(CFLAGS) $(LIBS) -shared -o $@ $^ $(WL_OBJS)

install:
	$(info :: installing)
	install -Dm755 $(TARGET) $(PREFIX)/lib/$(TARGET)
	install -Dm755 -d $(PREFIX)/include/$(NAME)
	cp -r $(INCLUDE_DIR)/* $(PREFIX)/include/$(NAME)
	install -Dm755 $(NAME).pc $(PREFIX)/lib/pkgconfig/$(NAME).pc

uninstall:
	rm $(PREFIX)/lib/$(TARGET)
	rm $(PREFIX)/lib/pkgconfig/$(NAME).pc
	rm -r $(PREFIX)/include/$(NAME)

LIB = $(shell pkgconf --cflags --libs $(NAME))

test:
	$(info :: making client)
	$(CC) $(CFLAGS) $(LIB) tests/main.cpp -o build/test
	./build/test

.PHONY: help
.SILENT: help
help:
	$(info ~~ lunaland kit - a wayland gui toolkit)
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
		--suppress=unreadVariable --suppress=unusedFunction --suppress=constParameterCallback \
		--suppress=missingIncludeSystem --enable=all --suppress=cstyleCast \
		-Iinclude -i$(WL_PROTOCOLS_DIR) $(SRC_DIR)

format:
	clang-format -i $(LIB_SRCS) $(LIB_HEADERS)
