# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /home/chris/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/231.8770.66/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /home/chris/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/231.8770.66/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chris/Projects/C-Projects/Tanks2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chris/Projects/C-Projects/Tanks2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Tanks2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Tanks2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Tanks2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Tanks2.dir/flags.make

CMakeFiles/Tanks2.dir/ItemList.cpp.o: CMakeFiles/Tanks2.dir/flags.make
CMakeFiles/Tanks2.dir/ItemList.cpp.o: /home/chris/Projects/C-Projects/Tanks2/ItemList.cpp
CMakeFiles/Tanks2.dir/ItemList.cpp.o: CMakeFiles/Tanks2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chris/Projects/C-Projects/Tanks2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Tanks2.dir/ItemList.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Tanks2.dir/ItemList.cpp.o -MF CMakeFiles/Tanks2.dir/ItemList.cpp.o.d -o CMakeFiles/Tanks2.dir/ItemList.cpp.o -c /home/chris/Projects/C-Projects/Tanks2/ItemList.cpp

CMakeFiles/Tanks2.dir/ItemList.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tanks2.dir/ItemList.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chris/Projects/C-Projects/Tanks2/ItemList.cpp > CMakeFiles/Tanks2.dir/ItemList.cpp.i

CMakeFiles/Tanks2.dir/ItemList.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tanks2.dir/ItemList.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chris/Projects/C-Projects/Tanks2/ItemList.cpp -o CMakeFiles/Tanks2.dir/ItemList.cpp.s

CMakeFiles/Tanks2.dir/main.cpp.o: CMakeFiles/Tanks2.dir/flags.make
CMakeFiles/Tanks2.dir/main.cpp.o: /home/chris/Projects/C-Projects/Tanks2/main.cpp
CMakeFiles/Tanks2.dir/main.cpp.o: CMakeFiles/Tanks2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chris/Projects/C-Projects/Tanks2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Tanks2.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Tanks2.dir/main.cpp.o -MF CMakeFiles/Tanks2.dir/main.cpp.o.d -o CMakeFiles/Tanks2.dir/main.cpp.o -c /home/chris/Projects/C-Projects/Tanks2/main.cpp

CMakeFiles/Tanks2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tanks2.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chris/Projects/C-Projects/Tanks2/main.cpp > CMakeFiles/Tanks2.dir/main.cpp.i

CMakeFiles/Tanks2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tanks2.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chris/Projects/C-Projects/Tanks2/main.cpp -o CMakeFiles/Tanks2.dir/main.cpp.s

CMakeFiles/Tanks2.dir/DrawText.cpp.o: CMakeFiles/Tanks2.dir/flags.make
CMakeFiles/Tanks2.dir/DrawText.cpp.o: /home/chris/Projects/C-Projects/Tanks2/DrawText.cpp
CMakeFiles/Tanks2.dir/DrawText.cpp.o: CMakeFiles/Tanks2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chris/Projects/C-Projects/Tanks2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Tanks2.dir/DrawText.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Tanks2.dir/DrawText.cpp.o -MF CMakeFiles/Tanks2.dir/DrawText.cpp.o.d -o CMakeFiles/Tanks2.dir/DrawText.cpp.o -c /home/chris/Projects/C-Projects/Tanks2/DrawText.cpp

CMakeFiles/Tanks2.dir/DrawText.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tanks2.dir/DrawText.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chris/Projects/C-Projects/Tanks2/DrawText.cpp > CMakeFiles/Tanks2.dir/DrawText.cpp.i

CMakeFiles/Tanks2.dir/DrawText.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tanks2.dir/DrawText.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chris/Projects/C-Projects/Tanks2/DrawText.cpp -o CMakeFiles/Tanks2.dir/DrawText.cpp.s

CMakeFiles/Tanks2.dir/gameMessageBox.cpp.o: CMakeFiles/Tanks2.dir/flags.make
CMakeFiles/Tanks2.dir/gameMessageBox.cpp.o: /home/chris/Projects/C-Projects/Tanks2/gameMessageBox.cpp
CMakeFiles/Tanks2.dir/gameMessageBox.cpp.o: CMakeFiles/Tanks2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chris/Projects/C-Projects/Tanks2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Tanks2.dir/gameMessageBox.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Tanks2.dir/gameMessageBox.cpp.o -MF CMakeFiles/Tanks2.dir/gameMessageBox.cpp.o.d -o CMakeFiles/Tanks2.dir/gameMessageBox.cpp.o -c /home/chris/Projects/C-Projects/Tanks2/gameMessageBox.cpp

CMakeFiles/Tanks2.dir/gameMessageBox.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tanks2.dir/gameMessageBox.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chris/Projects/C-Projects/Tanks2/gameMessageBox.cpp > CMakeFiles/Tanks2.dir/gameMessageBox.cpp.i

CMakeFiles/Tanks2.dir/gameMessageBox.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tanks2.dir/gameMessageBox.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chris/Projects/C-Projects/Tanks2/gameMessageBox.cpp -o CMakeFiles/Tanks2.dir/gameMessageBox.cpp.s

# Object files for target Tanks2
Tanks2_OBJECTS = \
"CMakeFiles/Tanks2.dir/ItemList.cpp.o" \
"CMakeFiles/Tanks2.dir/main.cpp.o" \
"CMakeFiles/Tanks2.dir/DrawText.cpp.o" \
"CMakeFiles/Tanks2.dir/gameMessageBox.cpp.o"

# External object files for target Tanks2
Tanks2_EXTERNAL_OBJECTS =

Tanks2: CMakeFiles/Tanks2.dir/ItemList.cpp.o
Tanks2: CMakeFiles/Tanks2.dir/main.cpp.o
Tanks2: CMakeFiles/Tanks2.dir/DrawText.cpp.o
Tanks2: CMakeFiles/Tanks2.dir/gameMessageBox.cpp.o
Tanks2: CMakeFiles/Tanks2.dir/build.make
Tanks2: /usr/lib/x86_64-linux-gnu/libSDL.so
Tanks2: /usr/lib/x86_64-linux-gnu/libSDL_ttf.so
Tanks2: /usr/lib/x86_64-linux-gnu/libSDL_mixer.so
Tanks2: CMakeFiles/Tanks2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chris/Projects/C-Projects/Tanks2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable Tanks2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Tanks2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Tanks2.dir/build: Tanks2
.PHONY : CMakeFiles/Tanks2.dir/build

CMakeFiles/Tanks2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Tanks2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Tanks2.dir/clean

CMakeFiles/Tanks2.dir/depend:
	cd /home/chris/Projects/C-Projects/Tanks2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chris/Projects/C-Projects/Tanks2 /home/chris/Projects/C-Projects/Tanks2 /home/chris/Projects/C-Projects/Tanks2/cmake-build-debug /home/chris/Projects/C-Projects/Tanks2/cmake-build-debug /home/chris/Projects/C-Projects/Tanks2/cmake-build-debug/CMakeFiles/Tanks2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Tanks2.dir/depend
