# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.2.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.2.4\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\muhamed\IdeaProjects\ASP\pz10

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\muhamed\IdeaProjects\ASP\pz10\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/pz10.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pz10.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pz10.dir/flags.make

CMakeFiles/pz10.dir/main.cpp.obj: CMakeFiles/pz10.dir/flags.make
CMakeFiles/pz10.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\muhamed\IdeaProjects\ASP\pz10\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pz10.dir/main.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\pz10.dir\main.cpp.obj -c C:\Users\muhamed\IdeaProjects\ASP\pz10\main.cpp

CMakeFiles/pz10.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pz10.dir/main.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\muhamed\IdeaProjects\ASP\pz10\main.cpp > CMakeFiles\pz10.dir\main.cpp.i

CMakeFiles/pz10.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pz10.dir/main.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\muhamed\IdeaProjects\ASP\pz10\main.cpp -o CMakeFiles\pz10.dir\main.cpp.s

# Object files for target pz10
pz10_OBJECTS = \
"CMakeFiles/pz10.dir/main.cpp.obj"

# External object files for target pz10
pz10_EXTERNAL_OBJECTS =

pz10.exe: CMakeFiles/pz10.dir/main.cpp.obj
pz10.exe: CMakeFiles/pz10.dir/build.make
pz10.exe: CMakeFiles/pz10.dir/linklibs.rsp
pz10.exe: CMakeFiles/pz10.dir/objects1.rsp
pz10.exe: CMakeFiles/pz10.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\muhamed\IdeaProjects\ASP\pz10\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable pz10.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\pz10.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pz10.dir/build: pz10.exe

.PHONY : CMakeFiles/pz10.dir/build

CMakeFiles/pz10.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\pz10.dir\cmake_clean.cmake
.PHONY : CMakeFiles/pz10.dir/clean

CMakeFiles/pz10.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\muhamed\IdeaProjects\ASP\pz10 C:\Users\muhamed\IdeaProjects\ASP\pz10 C:\Users\muhamed\IdeaProjects\ASP\pz10\cmake-build-debug C:\Users\muhamed\IdeaProjects\ASP\pz10\cmake-build-debug C:\Users\muhamed\IdeaProjects\ASP\pz10\cmake-build-debug\CMakeFiles\pz10.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pz10.dir/depend
