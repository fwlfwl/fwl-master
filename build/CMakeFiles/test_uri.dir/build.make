# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/fwl/program/fwl-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fwl/program/fwl-master/build

# Include any dependencies generated for this target.
include CMakeFiles/test_uri.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_uri.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_uri.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_uri.dir/flags.make

CMakeFiles/test_uri.dir/tests/test_uri.cc.o: CMakeFiles/test_uri.dir/flags.make
CMakeFiles/test_uri.dir/tests/test_uri.cc.o: ../tests/test_uri.cc
CMakeFiles/test_uri.dir/tests/test_uri.cc.o: CMakeFiles/test_uri.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_uri.dir/tests/test_uri.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_uri.dir/tests/test_uri.cc.o -MF CMakeFiles/test_uri.dir/tests/test_uri.cc.o.d -o CMakeFiles/test_uri.dir/tests/test_uri.cc.o -c /home/fwl/program/fwl-master/tests/test_uri.cc

CMakeFiles/test_uri.dir/tests/test_uri.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_uri.dir/tests/test_uri.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/tests/test_uri.cc > CMakeFiles/test_uri.dir/tests/test_uri.cc.i

CMakeFiles/test_uri.dir/tests/test_uri.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_uri.dir/tests/test_uri.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/tests/test_uri.cc -o CMakeFiles/test_uri.dir/tests/test_uri.cc.s

# Object files for target test_uri
test_uri_OBJECTS = \
"CMakeFiles/test_uri.dir/tests/test_uri.cc.o"

# External object files for target test_uri
test_uri_EXTERNAL_OBJECTS =

../bin/test_uri: CMakeFiles/test_uri.dir/tests/test_uri.cc.o
../bin/test_uri: CMakeFiles/test_uri.dir/build.make
../bin/test_uri: ../lib/libfwl.so
../bin/test_uri: /usr/lib/x86_64-linux-gnu/libssl.so
../bin/test_uri: /usr/lib/x86_64-linux-gnu/libcrypto.so
../bin/test_uri: CMakeFiles/test_uri.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_uri"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_uri.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_uri.dir/build: ../bin/test_uri
.PHONY : CMakeFiles/test_uri.dir/build

CMakeFiles/test_uri.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_uri.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_uri.dir/clean

CMakeFiles/test_uri.dir/depend:
	cd /home/fwl/program/fwl-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fwl/program/fwl-master /home/fwl/program/fwl-master /home/fwl/program/fwl-master/build /home/fwl/program/fwl-master/build /home/fwl/program/fwl-master/build/CMakeFiles/test_uri.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_uri.dir/depend

