
# EasyPack

An easy way to pack your application with all of it's dependencies into a single
binary.

# How does it work?

It's easy, if you have a bash program that's separated into multiple scripts you can run a command like this to generate a package out of it:
```
EASYPACK_ROOT=/path/to/sources \
EASYPACK_OUT=executable_name \
EASYPACK_ADDONS=addons/autorun_creator/libautoruncreator.so \
ARC_FILE=script_path_relative_to_EASYPACK_ROOT \
./easypack
```

If you then run the executale package it will automatically export all sources in a directory under `/tmp`
and execute the file set in `ARC_FILE`.

This makes ensures that:
- Your program will load correctly no matter what the caller's CWD is.
- Can be installed with all of it's dependencies in a single file under `/bin`.
- Allows programs with multiple dependencies to be installed without the need of any system directories.

# Parameters

- **EASYPACK_ROOT**: The root of a new package. When defined it's assumed we are creating a new package.
- **EASYPACK_OUT**: Output path. When extracting it's a directory, when packing it's a filename.
- **EASYPACK_ADDONS**: A comma separated list of specifically created .so files to be executed at package time. (The execution happens at the same order as the list is defined)
- **EASYPACK_MOUNTPOINTS**: A comma separated list of files and the location they should have inside the package separated by a colon. (Example: my/assets/directory:assets,my/data/directory:data)

# Runtime environment

The CWD is set to the directory in which the package was extracted. (eg, /tmp/easypack_XXXXXX/)

Environment variables:
- **EASYPACK_NAME**: The full path to the package (eg, /usr/bin/program)
- **EASYPACK_CWD**: The CWD in which the package was called (eg, /home/user)

# License

EasyPack is licensed under the MIT License.
