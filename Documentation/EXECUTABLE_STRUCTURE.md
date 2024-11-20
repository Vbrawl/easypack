
# Structure

- code
- embedded filesystem (optional)
- filesystem size

# Code

The general code of the application

# Embedded FileSystem

An embedded filesystem just to allow movement of files bundled with the executable

# Filesystem size

A 32-bit unsigned integer at the end of the executable that represents the size
of the embedded filesystem.

Performing [Total Executable Size] - [Embedded FileSystem Size] - 4 is equal to the
code's size