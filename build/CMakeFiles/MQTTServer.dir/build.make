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
CMAKE_SOURCE_DIR = /home/mars/MQTT_SERVER

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mars/MQTT_SERVER/build

# Include any dependencies generated for this target.
include CMakeFiles/MQTTServer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MQTTServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MQTTServer.dir/flags.make

CMakeFiles/MQTTServer.dir/src/main.cpp.o: CMakeFiles/MQTTServer.dir/flags.make
CMakeFiles/MQTTServer.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mars/MQTT_SERVER/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MQTTServer.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MQTTServer.dir/src/main.cpp.o -c /home/mars/MQTT_SERVER/src/main.cpp

CMakeFiles/MQTTServer.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MQTTServer.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mars/MQTT_SERVER/src/main.cpp > CMakeFiles/MQTTServer.dir/src/main.cpp.i

CMakeFiles/MQTTServer.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MQTTServer.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mars/MQTT_SERVER/src/main.cpp -o CMakeFiles/MQTTServer.dir/src/main.cpp.s

# Object files for target MQTTServer
MQTTServer_OBJECTS = \
"CMakeFiles/MQTTServer.dir/src/main.cpp.o"

# External object files for target MQTTServer
MQTTServer_EXTERNAL_OBJECTS =

../bin/MQTTServer: CMakeFiles/MQTTServer.dir/src/main.cpp.o
../bin/MQTTServer: CMakeFiles/MQTTServer.dir/build.make
../bin/MQTTServer: /usr/lib/x86_64-linux-gnu/libmosquitto.so
../bin/MQTTServer: CMakeFiles/MQTTServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mars/MQTT_SERVER/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/MQTTServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MQTTServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MQTTServer.dir/build: ../bin/MQTTServer

.PHONY : CMakeFiles/MQTTServer.dir/build

CMakeFiles/MQTTServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MQTTServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MQTTServer.dir/clean

CMakeFiles/MQTTServer.dir/depend:
	cd /home/mars/MQTT_SERVER/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mars/MQTT_SERVER /home/mars/MQTT_SERVER /home/mars/MQTT_SERVER/build /home/mars/MQTT_SERVER/build /home/mars/MQTT_SERVER/build/CMakeFiles/MQTTServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MQTTServer.dir/depend

