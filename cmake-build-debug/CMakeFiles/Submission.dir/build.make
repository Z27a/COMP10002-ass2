# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

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

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.1.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.1.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\leoli\CLionProjects\COMP10002-ass2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\leoli\CLionProjects\COMP10002-ass2\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\submission.dir\depend.make
# Include the progress variables for this target.
include CMakeFiles\submission.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\submission.dir\flags.make

CMakeFiles\submission.dir\submission.c.obj: CMakeFiles\submission.dir\flags.make
CMakeFiles\submission.dir\submission.c.obj: ..\submission.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\leoli\CLionProjects\COMP10002-ass2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/submission.dir/submission.c.obj"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1429~1.300\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\submission.dir\submission.c.obj /FdCMakeFiles\submission.dir\ /FS -c C:\Users\leoli\CLionProjects\COMP10002-ass2\submission.c
<<

CMakeFiles\submission.dir\submission.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/submission.dir/submission.c.i"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1429~1.300\bin\Hostx86\x86\cl.exe > CMakeFiles\submission.dir\submission.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\leoli\CLionProjects\COMP10002-ass2\submission.c
<<

CMakeFiles\submission.dir\submission.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/submission.dir/submission.c.s"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1429~1.300\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\submission.dir\submission.c.s /c C:\Users\leoli\CLionProjects\COMP10002-ass2\submission.c
<<

# Object files for target submission
submission_OBJECTS = \
"CMakeFiles\submission.dir\submission.c.obj"

# External object files for target submission
submission_EXTERNAL_OBJECTS =

submission.exe: CMakeFiles\submission.dir\submission.c.obj
submission.exe: CMakeFiles\submission.dir\build.make
submission.exe: CMakeFiles\submission.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\leoli\CLionProjects\COMP10002-ass2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable submission.exe"
	"C:\Program Files\JetBrains\CLion 2021.1.3\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\submission.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\mt.exe --manifests -- C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1429~1.300\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\submission.dir\objects1.rsp @<<
 /out:submission.exe /implib:submission.lib /pdb:C:\Users\leoli\CLionProjects\COMP10002-ass2\cmake-build-debug\submission.pdb /version:0.0 /machine:X86 /debug /INCREMENTAL /subsystem:console  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\submission.dir\build: submission.exe
.PHONY : CMakeFiles\submission.dir\build

CMakeFiles\submission.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\submission.dir\cmake_clean.cmake
.PHONY : CMakeFiles\submission.dir\clean

CMakeFiles\submission.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\leoli\CLionProjects\COMP10002-ass2 C:\Users\leoli\CLionProjects\COMP10002-ass2 C:\Users\leoli\CLionProjects\COMP10002-ass2\cmake-build-debug C:\Users\leoli\CLionProjects\COMP10002-ass2\cmake-build-debug C:\Users\leoli\CLionProjects\COMP10002-ass2\cmake-build-debug\CMakeFiles\Submission.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\submission.dir\depend

