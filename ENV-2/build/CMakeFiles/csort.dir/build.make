# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/miqo/homework/ENV-2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/miqo/homework/ENV-2/build

# Include any dependencies generated for this target.
include CMakeFiles/csort.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/csort.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/csort.dir/flags.make

CMakeFiles/csort.dir/main.cpp.o: CMakeFiles/csort.dir/flags.make
CMakeFiles/csort.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/miqo/homework/ENV-2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/csort.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/csort.dir/main.cpp.o -c /home/miqo/homework/ENV-2/main.cpp

CMakeFiles/csort.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/csort.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/miqo/homework/ENV-2/main.cpp > CMakeFiles/csort.dir/main.cpp.i

CMakeFiles/csort.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/csort.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/miqo/homework/ENV-2/main.cpp -o CMakeFiles/csort.dir/main.cpp.s

# Object files for target csort
csort_OBJECTS = \
"CMakeFiles/csort.dir/main.cpp.o"

# External object files for target csort
csort_EXTERNAL_OBJECTS =

csort: CMakeFiles/csort.dir/main.cpp.o
csort: CMakeFiles/csort.dir/build.make
csort: libComplex.a
csort: libSort.a
csort: CMakeFiles/csort.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/miqo/homework/ENV-2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable csort"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/csort.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/csort.dir/build: csort

.PHONY : CMakeFiles/csort.dir/build

CMakeFiles/csort.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/csort.dir/cmake_clean.cmake
.PHONY : CMakeFiles/csort.dir/clean

CMakeFiles/csort.dir/depend:
	cd /home/miqo/homework/ENV-2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/miqo/homework/ENV-2 /home/miqo/homework/ENV-2 /home/miqo/homework/ENV-2/build /home/miqo/homework/ENV-2/build /home/miqo/homework/ENV-2/build/CMakeFiles/csort.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/csort.dir/depend
