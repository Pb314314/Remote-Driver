# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/server_process_try

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/server_process_try/build

# Include any dependencies generated for this target.
include CMakeFiles/create_P.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/create_P.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/create_P.dir/flags.make

CMakeFiles/create_P.dir/server_create_process.cpp.o: CMakeFiles/create_P.dir/flags.make
CMakeFiles/create_P.dir/server_create_process.cpp.o: ../server_create_process.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/server_process_try/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/create_P.dir/server_create_process.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/create_P.dir/server_create_process.cpp.o -c /home/pi/server_process_try/server_create_process.cpp

CMakeFiles/create_P.dir/server_create_process.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/create_P.dir/server_create_process.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/server_process_try/server_create_process.cpp > CMakeFiles/create_P.dir/server_create_process.cpp.i

CMakeFiles/create_P.dir/server_create_process.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/create_P.dir/server_create_process.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/server_process_try/server_create_process.cpp -o CMakeFiles/create_P.dir/server_create_process.cpp.s

CMakeFiles/create_P.dir/server_create_process.cpp.o.requires:

.PHONY : CMakeFiles/create_P.dir/server_create_process.cpp.o.requires

CMakeFiles/create_P.dir/server_create_process.cpp.o.provides: CMakeFiles/create_P.dir/server_create_process.cpp.o.requires
	$(MAKE) -f CMakeFiles/create_P.dir/build.make CMakeFiles/create_P.dir/server_create_process.cpp.o.provides.build
.PHONY : CMakeFiles/create_P.dir/server_create_process.cpp.o.provides

CMakeFiles/create_P.dir/server_create_process.cpp.o.provides.build: CMakeFiles/create_P.dir/server_create_process.cpp.o


# Object files for target create_P
create_P_OBJECTS = \
"CMakeFiles/create_P.dir/server_create_process.cpp.o"

# External object files for target create_P
create_P_EXTERNAL_OBJECTS =

create_P: CMakeFiles/create_P.dir/server_create_process.cpp.o
create_P: CMakeFiles/create_P.dir/build.make
create_P: CMakeFiles/create_P.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/server_process_try/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable create_P"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/create_P.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/create_P.dir/build: create_P

.PHONY : CMakeFiles/create_P.dir/build

CMakeFiles/create_P.dir/requires: CMakeFiles/create_P.dir/server_create_process.cpp.o.requires

.PHONY : CMakeFiles/create_P.dir/requires

CMakeFiles/create_P.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/create_P.dir/cmake_clean.cmake
.PHONY : CMakeFiles/create_P.dir/clean

CMakeFiles/create_P.dir/depend:
	cd /home/pi/server_process_try/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/server_process_try /home/pi/server_process_try /home/pi/server_process_try/build /home/pi/server_process_try/build /home/pi/server_process_try/build/CMakeFiles/create_P.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/create_P.dir/depend

