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
CMAKE_SOURCE_DIR = "/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation/build"

# Utility rule file for BookReaderView_lrelease.

# Include any custom commands dependencies for this target.
include CMakeFiles/BookReaderView_lrelease.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/BookReaderView_lrelease.dir/progress.make

CMakeFiles/BookReaderView_lrelease: BookReaderView_en_US.qm

BookReaderView_en_US.qm: /home/cedric/Qt/6.5.3/gcc_64/bin/lrelease
BookReaderView_en_US.qm: ../BookReaderView_en_US.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir="/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Generating BookReaderView_en_US.qm"
	/home/cedric/Qt/6.5.3/gcc_64/bin/lrelease "/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation/BookReaderView_en_US.ts" -qm "/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation/build/BookReaderView_en_US.qm"

BookReaderView_lrelease: BookReaderView_en_US.qm
BookReaderView_lrelease: CMakeFiles/BookReaderView_lrelease
BookReaderView_lrelease: CMakeFiles/BookReaderView_lrelease.dir/build.make
.PHONY : BookReaderView_lrelease

# Rule to build all files generated by this target.
CMakeFiles/BookReaderView_lrelease.dir/build: BookReaderView_lrelease
.PHONY : CMakeFiles/BookReaderView_lrelease.dir/build

CMakeFiles/BookReaderView_lrelease.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BookReaderView_lrelease.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BookReaderView_lrelease.dir/clean

CMakeFiles/BookReaderView_lrelease.dir/depend:
	cd "/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation" "/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation" "/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation/build" "/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation/build" "/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation/build/CMakeFiles/BookReaderView_lrelease.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/BookReaderView_lrelease.dir/depend

