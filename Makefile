# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	"C:/Program Files/CMake/bin/cmake.exe" -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	"C:/Program Files/CMake/bin/cmake.exe" -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	"C:/Program Files/CMake/bin/cmake.exe" -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	"C:/Program Files/CMake/bin/cmake.exe" -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	"C:/Program Files/CMake/bin/cmake-gui.exe" -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	"C:/Program Files/CMake/bin/cmake.exe" -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(CMAKE_COMMAND) -E cmake_progress_start C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main/CMakeFiles C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main/cdnn_xm6_app/CMakeFiles/progress.marks
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f CMakeFiles/Makefile2 cdnn_xm6_app/all
	$(CMAKE_COMMAND) -E cmake_progress_start C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f CMakeFiles/Makefile2 cdnn_xm6_app/clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f CMakeFiles/Makefile2 cdnn_xm6_app/preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f CMakeFiles/Makefile2 cdnn_xm6_app/preinstall
.PHONY : preinstall/fast

# Convenience name for target.
cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/rule:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f CMakeFiles/Makefile2 cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/rule
.PHONY : cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/rule

# Convenience name for target.
cdnn_xm6_app: cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/rule

.PHONY : cdnn_xm6_app

# fast build rule for target.
cdnn_xm6_app/fast:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build
.PHONY : cdnn_xm6_app/fast

src/cdnn_qdata.o: src/cdnn_qdata.c.o

.PHONY : src/cdnn_qdata.o

# target to build an object file
src/cdnn_qdata.c.o:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/cdnn_qdata.c.o
.PHONY : src/cdnn_qdata.c.o

src/cdnn_qdata.i: src/cdnn_qdata.c.i

.PHONY : src/cdnn_qdata.i

# target to preprocess a source file
src/cdnn_qdata.c.i:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/cdnn_qdata.c.i
.PHONY : src/cdnn_qdata.c.i

src/cdnn_qdata.s: src/cdnn_qdata.c.s

.PHONY : src/cdnn_qdata.s

# target to generate assembly for a file
src/cdnn_qdata.c.s:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/cdnn_qdata.c.s
.PHONY : src/cdnn_qdata.c.s

src/crt0.o: src/crt0.c.o

.PHONY : src/crt0.o

# target to build an object file
src/crt0.c.o:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/crt0.c.o
.PHONY : src/crt0.c.o

src/crt0.i: src/crt0.c.i

.PHONY : src/crt0.i

# target to preprocess a source file
src/crt0.c.i:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/crt0.c.i
.PHONY : src/crt0.c.i

src/crt0.s: src/crt0.c.s

.PHONY : src/crt0.s

# target to generate assembly for a file
src/crt0.c.s:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/crt0.c.s
.PHONY : src/crt0.c.s

src/img.o: src/img.c.o

.PHONY : src/img.o

# target to build an object file
src/img.c.o:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/img.c.o
.PHONY : src/img.c.o

src/img.i: src/img.c.i

.PHONY : src/img.i

# target to preprocess a source file
src/img.c.i:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/img.c.i
.PHONY : src/img.c.i

src/img.s: src/img.c.s

.PHONY : src/img.s

# target to generate assembly for a file
src/img.c.s:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/img.c.s
.PHONY : src/img.c.s

src/inference.o: src/inference.cpp.o

.PHONY : src/inference.o

# target to build an object file
src/inference.cpp.o:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/inference.cpp.o
.PHONY : src/inference.cpp.o

src/inference.i: src/inference.cpp.i

.PHONY : src/inference.i

# target to preprocess a source file
src/inference.cpp.i:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/inference.cpp.i
.PHONY : src/inference.cpp.i

src/inference.s: src/inference.cpp.s

.PHONY : src/inference.s

# target to generate assembly for a file
src/inference.cpp.s:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/inference.cpp.s
.PHONY : src/inference.cpp.s

src/main.o: src/main.c.o

.PHONY : src/main.o

# target to build an object file
src/main.c.o:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/main.c.o
.PHONY : src/main.c.o

src/main.i: src/main.c.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.c.i:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/main.c.i
.PHONY : src/main.c.i

src/main.s: src/main.c.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.c.s:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) -f cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/build.make cdnn_xm6_app/CMakeFiles/cdnn_xm6_app.dir/src/main.c.s
.PHONY : src/main.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... list_install_components"
	@echo "... cdnn_xm6_app"
	@echo "... install/local"
	@echo "... install"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... src/cdnn_qdata.o"
	@echo "... src/cdnn_qdata.i"
	@echo "... src/cdnn_qdata.s"
	@echo "... src/crt0.o"
	@echo "... src/crt0.i"
	@echo "... src/crt0.s"
	@echo "... src/img.o"
	@echo "... src/img.i"
	@echo "... src/img.s"
	@echo "... src/inference.o"
	@echo "... src/inference.i"
	@echo "... src/inference.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

