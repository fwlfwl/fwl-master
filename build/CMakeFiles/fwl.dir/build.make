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
include CMakeFiles/fwl.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/fwl.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/fwl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fwl.dir/flags.make

CMakeFiles/fwl.dir/fwl/sock_stream.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/sock_stream.cc.o: ../fwl/sock_stream.cc
CMakeFiles/fwl.dir/fwl/sock_stream.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fwl.dir/fwl/sock_stream.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/sock_stream.cc.o -MF CMakeFiles/fwl.dir/fwl/sock_stream.cc.o.d -o CMakeFiles/fwl.dir/fwl/sock_stream.cc.o -c /home/fwl/program/fwl-master/fwl/sock_stream.cc

CMakeFiles/fwl.dir/fwl/sock_stream.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/sock_stream.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/sock_stream.cc > CMakeFiles/fwl.dir/fwl/sock_stream.cc.i

CMakeFiles/fwl.dir/fwl/sock_stream.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/sock_stream.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/sock_stream.cc -o CMakeFiles/fwl.dir/fwl/sock_stream.cc.s

CMakeFiles/fwl.dir/fwl/stream.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/stream.cc.o: ../fwl/stream.cc
CMakeFiles/fwl.dir/fwl/stream.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/fwl.dir/fwl/stream.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/stream.cc.o -MF CMakeFiles/fwl.dir/fwl/stream.cc.o.d -o CMakeFiles/fwl.dir/fwl/stream.cc.o -c /home/fwl/program/fwl-master/fwl/stream.cc

CMakeFiles/fwl.dir/fwl/stream.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/stream.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/stream.cc > CMakeFiles/fwl.dir/fwl/stream.cc.i

CMakeFiles/fwl.dir/fwl/stream.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/stream.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/stream.cc -o CMakeFiles/fwl.dir/fwl/stream.cc.s

CMakeFiles/fwl.dir/fwl/network_server.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/network_server.cc.o: ../fwl/network_server.cc
CMakeFiles/fwl.dir/fwl/network_server.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/fwl.dir/fwl/network_server.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/network_server.cc.o -MF CMakeFiles/fwl.dir/fwl/network_server.cc.o.d -o CMakeFiles/fwl.dir/fwl/network_server.cc.o -c /home/fwl/program/fwl-master/fwl/network_server.cc

CMakeFiles/fwl.dir/fwl/network_server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/network_server.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/network_server.cc > CMakeFiles/fwl.dir/fwl/network_server.cc.i

CMakeFiles/fwl.dir/fwl/network_server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/network_server.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/network_server.cc -o CMakeFiles/fwl.dir/fwl/network_server.cc.s

CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.o: ../fwl/http/http_servlet.cc
CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.o -MF CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.o.d -o CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.o -c /home/fwl/program/fwl-master/fwl/http/http_servlet.cc

CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/http/http_servlet.cc > CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.i

CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/http/http_servlet.cc -o CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.s

CMakeFiles/fwl.dir/fwl/http/http_server.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/http/http_server.cc.o: ../fwl/http/http_server.cc
CMakeFiles/fwl.dir/fwl/http/http_server.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/fwl.dir/fwl/http/http_server.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/http/http_server.cc.o -MF CMakeFiles/fwl.dir/fwl/http/http_server.cc.o.d -o CMakeFiles/fwl.dir/fwl/http/http_server.cc.o -c /home/fwl/program/fwl-master/fwl/http/http_server.cc

CMakeFiles/fwl.dir/fwl/http/http_server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/http/http_server.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/http/http_server.cc > CMakeFiles/fwl.dir/fwl/http/http_server.cc.i

CMakeFiles/fwl.dir/fwl/http/http_server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/http/http_server.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/http/http_server.cc -o CMakeFiles/fwl.dir/fwl/http/http_server.cc.s

CMakeFiles/fwl.dir/fwl/http/http_session.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/http/http_session.cc.o: ../fwl/http/http_session.cc
CMakeFiles/fwl.dir/fwl/http/http_session.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/fwl.dir/fwl/http/http_session.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/http/http_session.cc.o -MF CMakeFiles/fwl.dir/fwl/http/http_session.cc.o.d -o CMakeFiles/fwl.dir/fwl/http/http_session.cc.o -c /home/fwl/program/fwl-master/fwl/http/http_session.cc

CMakeFiles/fwl.dir/fwl/http/http_session.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/http/http_session.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/http/http_session.cc > CMakeFiles/fwl.dir/fwl/http/http_session.cc.i

CMakeFiles/fwl.dir/fwl/http/http_session.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/http/http_session.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/http/http_session.cc -o CMakeFiles/fwl.dir/fwl/http/http_session.cc.s

CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.o: ../fwl/http/ragel/httpclient_parser.cc
CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.o -MF CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.o.d -o CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.o -c /home/fwl/program/fwl-master/fwl/http/ragel/httpclient_parser.cc

CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/http/ragel/httpclient_parser.cc > CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.i

CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/http/ragel/httpclient_parser.cc -o CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.s

CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.o: ../fwl/http/ragel/http11_parser.cc
CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.o -MF CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.o.d -o CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.o -c /home/fwl/program/fwl-master/fwl/http/ragel/http11_parser.cc

CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/http/ragel/http11_parser.cc > CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.i

CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/http/ragel/http11_parser.cc -o CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.s

CMakeFiles/fwl.dir/fwl/http/http_parser.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/http/http_parser.cc.o: ../fwl/http/http_parser.cc
CMakeFiles/fwl.dir/fwl/http/http_parser.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/fwl.dir/fwl/http/http_parser.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/http/http_parser.cc.o -MF CMakeFiles/fwl.dir/fwl/http/http_parser.cc.o.d -o CMakeFiles/fwl.dir/fwl/http/http_parser.cc.o -c /home/fwl/program/fwl-master/fwl/http/http_parser.cc

CMakeFiles/fwl.dir/fwl/http/http_parser.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/http/http_parser.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/http/http_parser.cc > CMakeFiles/fwl.dir/fwl/http/http_parser.cc.i

CMakeFiles/fwl.dir/fwl/http/http_parser.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/http/http_parser.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/http/http_parser.cc -o CMakeFiles/fwl.dir/fwl/http/http_parser.cc.s

CMakeFiles/fwl.dir/fwl/http/http.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/http/http.cc.o: ../fwl/http/http.cc
CMakeFiles/fwl.dir/fwl/http/http.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/fwl.dir/fwl/http/http.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/http/http.cc.o -MF CMakeFiles/fwl.dir/fwl/http/http.cc.o.d -o CMakeFiles/fwl.dir/fwl/http/http.cc.o -c /home/fwl/program/fwl-master/fwl/http/http.cc

CMakeFiles/fwl.dir/fwl/http/http.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/http/http.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/http/http.cc > CMakeFiles/fwl.dir/fwl/http/http.cc.i

CMakeFiles/fwl.dir/fwl/http/http.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/http/http.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/http/http.cc -o CMakeFiles/fwl.dir/fwl/http/http.cc.s

CMakeFiles/fwl.dir/fwl/socket.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/socket.cc.o: ../fwl/socket.cc
CMakeFiles/fwl.dir/fwl/socket.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/fwl.dir/fwl/socket.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/socket.cc.o -MF CMakeFiles/fwl.dir/fwl/socket.cc.o.d -o CMakeFiles/fwl.dir/fwl/socket.cc.o -c /home/fwl/program/fwl-master/fwl/socket.cc

CMakeFiles/fwl.dir/fwl/socket.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/socket.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/socket.cc > CMakeFiles/fwl.dir/fwl/socket.cc.i

CMakeFiles/fwl.dir/fwl/socket.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/socket.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/socket.cc -o CMakeFiles/fwl.dir/fwl/socket.cc.s

CMakeFiles/fwl.dir/fwl/address.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/address.cc.o: ../fwl/address.cc
CMakeFiles/fwl.dir/fwl/address.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/fwl.dir/fwl/address.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/address.cc.o -MF CMakeFiles/fwl.dir/fwl/address.cc.o.d -o CMakeFiles/fwl.dir/fwl/address.cc.o -c /home/fwl/program/fwl-master/fwl/address.cc

CMakeFiles/fwl.dir/fwl/address.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/address.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/address.cc > CMakeFiles/fwl.dir/fwl/address.cc.i

CMakeFiles/fwl.dir/fwl/address.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/address.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/address.cc -o CMakeFiles/fwl.dir/fwl/address.cc.s

CMakeFiles/fwl.dir/fwl/fd_manager.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/fd_manager.cc.o: ../fwl/fd_manager.cc
CMakeFiles/fwl.dir/fwl/fd_manager.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/fwl.dir/fwl/fd_manager.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/fd_manager.cc.o -MF CMakeFiles/fwl.dir/fwl/fd_manager.cc.o.d -o CMakeFiles/fwl.dir/fwl/fd_manager.cc.o -c /home/fwl/program/fwl-master/fwl/fd_manager.cc

CMakeFiles/fwl.dir/fwl/fd_manager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/fd_manager.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/fd_manager.cc > CMakeFiles/fwl.dir/fwl/fd_manager.cc.i

CMakeFiles/fwl.dir/fwl/fd_manager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/fd_manager.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/fd_manager.cc -o CMakeFiles/fwl.dir/fwl/fd_manager.cc.s

CMakeFiles/fwl.dir/fwl/bytearray.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/bytearray.cc.o: ../fwl/bytearray.cc
CMakeFiles/fwl.dir/fwl/bytearray.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/fwl.dir/fwl/bytearray.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/bytearray.cc.o -MF CMakeFiles/fwl.dir/fwl/bytearray.cc.o.d -o CMakeFiles/fwl.dir/fwl/bytearray.cc.o -c /home/fwl/program/fwl-master/fwl/bytearray.cc

CMakeFiles/fwl.dir/fwl/bytearray.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/bytearray.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/bytearray.cc > CMakeFiles/fwl.dir/fwl/bytearray.cc.i

CMakeFiles/fwl.dir/fwl/bytearray.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/bytearray.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/bytearray.cc -o CMakeFiles/fwl.dir/fwl/bytearray.cc.s

CMakeFiles/fwl.dir/fwl/hook.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/hook.cc.o: ../fwl/hook.cc
CMakeFiles/fwl.dir/fwl/hook.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/fwl.dir/fwl/hook.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/hook.cc.o -MF CMakeFiles/fwl.dir/fwl/hook.cc.o.d -o CMakeFiles/fwl.dir/fwl/hook.cc.o -c /home/fwl/program/fwl-master/fwl/hook.cc

CMakeFiles/fwl.dir/fwl/hook.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/hook.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/hook.cc > CMakeFiles/fwl.dir/fwl/hook.cc.i

CMakeFiles/fwl.dir/fwl/hook.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/hook.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/hook.cc -o CMakeFiles/fwl.dir/fwl/hook.cc.s

CMakeFiles/fwl.dir/fwl/timer.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/timer.cc.o: ../fwl/timer.cc
CMakeFiles/fwl.dir/fwl/timer.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/fwl.dir/fwl/timer.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/timer.cc.o -MF CMakeFiles/fwl.dir/fwl/timer.cc.o.d -o CMakeFiles/fwl.dir/fwl/timer.cc.o -c /home/fwl/program/fwl-master/fwl/timer.cc

CMakeFiles/fwl.dir/fwl/timer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/timer.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/timer.cc > CMakeFiles/fwl.dir/fwl/timer.cc.i

CMakeFiles/fwl.dir/fwl/timer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/timer.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/timer.cc -o CMakeFiles/fwl.dir/fwl/timer.cc.s

CMakeFiles/fwl.dir/fwl/iomanager.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/iomanager.cc.o: ../fwl/iomanager.cc
CMakeFiles/fwl.dir/fwl/iomanager.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/fwl.dir/fwl/iomanager.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/iomanager.cc.o -MF CMakeFiles/fwl.dir/fwl/iomanager.cc.o.d -o CMakeFiles/fwl.dir/fwl/iomanager.cc.o -c /home/fwl/program/fwl-master/fwl/iomanager.cc

CMakeFiles/fwl.dir/fwl/iomanager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/iomanager.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/iomanager.cc > CMakeFiles/fwl.dir/fwl/iomanager.cc.i

CMakeFiles/fwl.dir/fwl/iomanager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/iomanager.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/iomanager.cc -o CMakeFiles/fwl.dir/fwl/iomanager.cc.s

CMakeFiles/fwl.dir/fwl/scheduler.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/scheduler.cc.o: ../fwl/scheduler.cc
CMakeFiles/fwl.dir/fwl/scheduler.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object CMakeFiles/fwl.dir/fwl/scheduler.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/scheduler.cc.o -MF CMakeFiles/fwl.dir/fwl/scheduler.cc.o.d -o CMakeFiles/fwl.dir/fwl/scheduler.cc.o -c /home/fwl/program/fwl-master/fwl/scheduler.cc

CMakeFiles/fwl.dir/fwl/scheduler.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/scheduler.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/scheduler.cc > CMakeFiles/fwl.dir/fwl/scheduler.cc.i

CMakeFiles/fwl.dir/fwl/scheduler.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/scheduler.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/scheduler.cc -o CMakeFiles/fwl.dir/fwl/scheduler.cc.s

CMakeFiles/fwl.dir/fwl/fiber.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/fiber.cc.o: ../fwl/fiber.cc
CMakeFiles/fwl.dir/fwl/fiber.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Building CXX object CMakeFiles/fwl.dir/fwl/fiber.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/fiber.cc.o -MF CMakeFiles/fwl.dir/fwl/fiber.cc.o.d -o CMakeFiles/fwl.dir/fwl/fiber.cc.o -c /home/fwl/program/fwl-master/fwl/fiber.cc

CMakeFiles/fwl.dir/fwl/fiber.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/fiber.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/fiber.cc > CMakeFiles/fwl.dir/fwl/fiber.cc.i

CMakeFiles/fwl.dir/fwl/fiber.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/fiber.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/fiber.cc -o CMakeFiles/fwl.dir/fwl/fiber.cc.s

CMakeFiles/fwl.dir/fwl/log.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/log.cc.o: ../fwl/log.cc
CMakeFiles/fwl.dir/fwl/log.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_20) "Building CXX object CMakeFiles/fwl.dir/fwl/log.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/log.cc.o -MF CMakeFiles/fwl.dir/fwl/log.cc.o.d -o CMakeFiles/fwl.dir/fwl/log.cc.o -c /home/fwl/program/fwl-master/fwl/log.cc

CMakeFiles/fwl.dir/fwl/log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/log.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/log.cc > CMakeFiles/fwl.dir/fwl/log.cc.i

CMakeFiles/fwl.dir/fwl/log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/log.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/log.cc -o CMakeFiles/fwl.dir/fwl/log.cc.s

CMakeFiles/fwl.dir/fwl/unit.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/unit.cc.o: ../fwl/unit.cc
CMakeFiles/fwl.dir/fwl/unit.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_21) "Building CXX object CMakeFiles/fwl.dir/fwl/unit.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/unit.cc.o -MF CMakeFiles/fwl.dir/fwl/unit.cc.o.d -o CMakeFiles/fwl.dir/fwl/unit.cc.o -c /home/fwl/program/fwl-master/fwl/unit.cc

CMakeFiles/fwl.dir/fwl/unit.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/unit.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/unit.cc > CMakeFiles/fwl.dir/fwl/unit.cc.i

CMakeFiles/fwl.dir/fwl/unit.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/unit.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/unit.cc -o CMakeFiles/fwl.dir/fwl/unit.cc.s

CMakeFiles/fwl.dir/fwl/config.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/config.cc.o: ../fwl/config.cc
CMakeFiles/fwl.dir/fwl/config.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_22) "Building CXX object CMakeFiles/fwl.dir/fwl/config.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/config.cc.o -MF CMakeFiles/fwl.dir/fwl/config.cc.o.d -o CMakeFiles/fwl.dir/fwl/config.cc.o -c /home/fwl/program/fwl-master/fwl/config.cc

CMakeFiles/fwl.dir/fwl/config.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/config.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/config.cc > CMakeFiles/fwl.dir/fwl/config.cc.i

CMakeFiles/fwl.dir/fwl/config.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/config.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/config.cc -o CMakeFiles/fwl.dir/fwl/config.cc.s

CMakeFiles/fwl.dir/fwl/thread.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/thread.cc.o: ../fwl/thread.cc
CMakeFiles/fwl.dir/fwl/thread.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_23) "Building CXX object CMakeFiles/fwl.dir/fwl/thread.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/thread.cc.o -MF CMakeFiles/fwl.dir/fwl/thread.cc.o.d -o CMakeFiles/fwl.dir/fwl/thread.cc.o -c /home/fwl/program/fwl-master/fwl/thread.cc

CMakeFiles/fwl.dir/fwl/thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/thread.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/thread.cc > CMakeFiles/fwl.dir/fwl/thread.cc.i

CMakeFiles/fwl.dir/fwl/thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/thread.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/thread.cc -o CMakeFiles/fwl.dir/fwl/thread.cc.s

CMakeFiles/fwl.dir/fwl/mutex.cc.o: CMakeFiles/fwl.dir/flags.make
CMakeFiles/fwl.dir/fwl/mutex.cc.o: ../fwl/mutex.cc
CMakeFiles/fwl.dir/fwl/mutex.cc.o: CMakeFiles/fwl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_24) "Building CXX object CMakeFiles/fwl.dir/fwl/mutex.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fwl.dir/fwl/mutex.cc.o -MF CMakeFiles/fwl.dir/fwl/mutex.cc.o.d -o CMakeFiles/fwl.dir/fwl/mutex.cc.o -c /home/fwl/program/fwl-master/fwl/mutex.cc

CMakeFiles/fwl.dir/fwl/mutex.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fwl.dir/fwl/mutex.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fwl/program/fwl-master/fwl/mutex.cc > CMakeFiles/fwl.dir/fwl/mutex.cc.i

CMakeFiles/fwl.dir/fwl/mutex.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fwl.dir/fwl/mutex.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fwl/program/fwl-master/fwl/mutex.cc -o CMakeFiles/fwl.dir/fwl/mutex.cc.s

# Object files for target fwl
fwl_OBJECTS = \
"CMakeFiles/fwl.dir/fwl/sock_stream.cc.o" \
"CMakeFiles/fwl.dir/fwl/stream.cc.o" \
"CMakeFiles/fwl.dir/fwl/network_server.cc.o" \
"CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.o" \
"CMakeFiles/fwl.dir/fwl/http/http_server.cc.o" \
"CMakeFiles/fwl.dir/fwl/http/http_session.cc.o" \
"CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.o" \
"CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.o" \
"CMakeFiles/fwl.dir/fwl/http/http_parser.cc.o" \
"CMakeFiles/fwl.dir/fwl/http/http.cc.o" \
"CMakeFiles/fwl.dir/fwl/socket.cc.o" \
"CMakeFiles/fwl.dir/fwl/address.cc.o" \
"CMakeFiles/fwl.dir/fwl/fd_manager.cc.o" \
"CMakeFiles/fwl.dir/fwl/bytearray.cc.o" \
"CMakeFiles/fwl.dir/fwl/hook.cc.o" \
"CMakeFiles/fwl.dir/fwl/timer.cc.o" \
"CMakeFiles/fwl.dir/fwl/iomanager.cc.o" \
"CMakeFiles/fwl.dir/fwl/scheduler.cc.o" \
"CMakeFiles/fwl.dir/fwl/fiber.cc.o" \
"CMakeFiles/fwl.dir/fwl/log.cc.o" \
"CMakeFiles/fwl.dir/fwl/unit.cc.o" \
"CMakeFiles/fwl.dir/fwl/config.cc.o" \
"CMakeFiles/fwl.dir/fwl/thread.cc.o" \
"CMakeFiles/fwl.dir/fwl/mutex.cc.o"

# External object files for target fwl
fwl_EXTERNAL_OBJECTS =

../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/sock_stream.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/stream.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/network_server.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/http/http_servlet.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/http/http_server.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/http/http_session.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/http/ragel/httpclient_parser.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/http/ragel/http11_parser.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/http/http_parser.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/http/http.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/socket.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/address.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/fd_manager.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/bytearray.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/hook.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/timer.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/iomanager.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/scheduler.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/fiber.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/log.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/unit.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/config.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/thread.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/fwl/mutex.cc.o
../lib/libfwl.so: CMakeFiles/fwl.dir/build.make
../lib/libfwl.so: CMakeFiles/fwl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fwl/program/fwl-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_25) "Linking CXX shared library ../lib/libfwl.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fwl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fwl.dir/build: ../lib/libfwl.so
.PHONY : CMakeFiles/fwl.dir/build

CMakeFiles/fwl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fwl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fwl.dir/clean

CMakeFiles/fwl.dir/depend:
	cd /home/fwl/program/fwl-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fwl/program/fwl-master /home/fwl/program/fwl-master /home/fwl/program/fwl-master/build /home/fwl/program/fwl-master/build /home/fwl/program/fwl-master/build/CMakeFiles/fwl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fwl.dir/depend

