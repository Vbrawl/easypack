
# EasyPack

An easy way to pack your application with all of it's dependencies into a single
binary.

# How does it work?

It's easy, if you use python then your project's source code will be copied into a folder
where both your code and the python's interpreter will be included together
with a specially crafted executable that will execute the python interpreter pointing
it to your program's entry-point.

If you compile for single-file executable the program will unpack the whole directory
to a temporary one and perform the actions as above

This ensures that your program will load correctly and you will not need to
make so many adjustments to your code

# Supported Languages?

No support at the moment

# Developing For Languages?

* Python

# Parameters

- **EASYPACK_ROOT**: The root of a new package. When defined it's assumed we are creating a new package.
- **EASYPACK_OUT**: Output path. When extracting it's a directory, when packing it's a filename.
- **EASYPACK_ADDONS**: A comma separated list of specifically created .so files to be executed at package time. (The execution happens at the same order as the list is defined)

# License

EasyPack is licensed under the MIT License.
