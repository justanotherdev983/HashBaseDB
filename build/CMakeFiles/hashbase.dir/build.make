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
CMAKE_SOURCE_DIR = /home/boudewijn/code/c/HashBaseDB

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/boudewijn/code/c/HashBaseDB/build

# Include any dependencies generated for this target.
include CMakeFiles/hashbase.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hashbase.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hashbase.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hashbase.dir/flags.make

CMakeFiles/hashbase.dir/src/core/main.cpp.o: CMakeFiles/hashbase.dir/flags.make
CMakeFiles/hashbase.dir/src/core/main.cpp.o: ../src/core/main.cpp
CMakeFiles/hashbase.dir/src/core/main.cpp.o: CMakeFiles/hashbase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/boudewijn/code/c/HashBaseDB/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hashbase.dir/src/core/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hashbase.dir/src/core/main.cpp.o -MF CMakeFiles/hashbase.dir/src/core/main.cpp.o.d -o CMakeFiles/hashbase.dir/src/core/main.cpp.o -c /home/boudewijn/code/c/HashBaseDB/src/core/main.cpp

CMakeFiles/hashbase.dir/src/core/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hashbase.dir/src/core/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/boudewijn/code/c/HashBaseDB/src/core/main.cpp > CMakeFiles/hashbase.dir/src/core/main.cpp.i

CMakeFiles/hashbase.dir/src/core/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hashbase.dir/src/core/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/boudewijn/code/c/HashBaseDB/src/core/main.cpp -o CMakeFiles/hashbase.dir/src/core/main.cpp.s

CMakeFiles/hashbase.dir/src/core/database.cpp.o: CMakeFiles/hashbase.dir/flags.make
CMakeFiles/hashbase.dir/src/core/database.cpp.o: ../src/core/database.cpp
CMakeFiles/hashbase.dir/src/core/database.cpp.o: CMakeFiles/hashbase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/boudewijn/code/c/HashBaseDB/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/hashbase.dir/src/core/database.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hashbase.dir/src/core/database.cpp.o -MF CMakeFiles/hashbase.dir/src/core/database.cpp.o.d -o CMakeFiles/hashbase.dir/src/core/database.cpp.o -c /home/boudewijn/code/c/HashBaseDB/src/core/database.cpp

CMakeFiles/hashbase.dir/src/core/database.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hashbase.dir/src/core/database.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/boudewijn/code/c/HashBaseDB/src/core/database.cpp > CMakeFiles/hashbase.dir/src/core/database.cpp.i

CMakeFiles/hashbase.dir/src/core/database.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hashbase.dir/src/core/database.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/boudewijn/code/c/HashBaseDB/src/core/database.cpp -o CMakeFiles/hashbase.dir/src/core/database.cpp.s

CMakeFiles/hashbase.dir/src/core/hash.cpp.o: CMakeFiles/hashbase.dir/flags.make
CMakeFiles/hashbase.dir/src/core/hash.cpp.o: ../src/core/hash.cpp
CMakeFiles/hashbase.dir/src/core/hash.cpp.o: CMakeFiles/hashbase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/boudewijn/code/c/HashBaseDB/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/hashbase.dir/src/core/hash.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hashbase.dir/src/core/hash.cpp.o -MF CMakeFiles/hashbase.dir/src/core/hash.cpp.o.d -o CMakeFiles/hashbase.dir/src/core/hash.cpp.o -c /home/boudewijn/code/c/HashBaseDB/src/core/hash.cpp

CMakeFiles/hashbase.dir/src/core/hash.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hashbase.dir/src/core/hash.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/boudewijn/code/c/HashBaseDB/src/core/hash.cpp > CMakeFiles/hashbase.dir/src/core/hash.cpp.i

CMakeFiles/hashbase.dir/src/core/hash.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hashbase.dir/src/core/hash.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/boudewijn/code/c/HashBaseDB/src/core/hash.cpp -o CMakeFiles/hashbase.dir/src/core/hash.cpp.s

# Object files for target hashbase
hashbase_OBJECTS = \
"CMakeFiles/hashbase.dir/src/core/main.cpp.o" \
"CMakeFiles/hashbase.dir/src/core/database.cpp.o" \
"CMakeFiles/hashbase.dir/src/core/hash.cpp.o"

# External object files for target hashbase
hashbase_EXTERNAL_OBJECTS =

hashbase: CMakeFiles/hashbase.dir/src/core/main.cpp.o
hashbase: CMakeFiles/hashbase.dir/src/core/database.cpp.o
hashbase: CMakeFiles/hashbase.dir/src/core/hash.cpp.o
hashbase: CMakeFiles/hashbase.dir/build.make
hashbase: CMakeFiles/hashbase.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/boudewijn/code/c/HashBaseDB/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable hashbase"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hashbase.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hashbase.dir/build: hashbase
.PHONY : CMakeFiles/hashbase.dir/build

CMakeFiles/hashbase.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hashbase.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hashbase.dir/clean

CMakeFiles/hashbase.dir/depend:
	cd /home/boudewijn/code/c/HashBaseDB/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/boudewijn/code/c/HashBaseDB /home/boudewijn/code/c/HashBaseDB /home/boudewijn/code/c/HashBaseDB/build /home/boudewijn/code/c/HashBaseDB/build /home/boudewijn/code/c/HashBaseDB/build/CMakeFiles/hashbase.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hashbase.dir/depend

