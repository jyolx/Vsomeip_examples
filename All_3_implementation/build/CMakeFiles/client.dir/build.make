# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation/build

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/src/client.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/client.cpp.o: /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation/src/client.cpp
CMakeFiles/client.dir/src/client.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client.dir/src/client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/client.cpp.o -MF CMakeFiles/client.dir/src/client.cpp.o.d -o CMakeFiles/client.dir/src/client.cpp.o -c /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation/src/client.cpp

CMakeFiles/client.dir/src/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation/src/client.cpp > CMakeFiles/client.dir/src/client.cpp.i

CMakeFiles/client.dir/src/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation/src/client.cpp -o CMakeFiles/client.dir/src/client.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/src/client.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client: CMakeFiles/client.dir/src/client.cpp.o
client: CMakeFiles/client.dir/build.make
client: /usr/local/lib/libvsomeip3.so.3.4.10
client: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.83.0
client: /usr/lib/x86_64-linux-gnu/libboost_log.so.1.83.0
client: /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.83.0
client: /usr/lib/x86_64-linux-gnu/libboost_chrono.so.1.83.0
client: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.83.0
client: /usr/lib/x86_64-linux-gnu/libboost_atomic.so.1.83.0
client: /usr/lib/x86_64-linux-gnu/libboost_regex.so.1.83.0
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client
.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation/build /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation/build /home/jyolsna/Documents/vsomeip-3.4.10/Jyolsna_example/All_3_implementation/build/CMakeFiles/client.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/client.dir/depend

