

# AutoRun Creator

This addon enabled automatic "autorun.easypack" creation at build time.

# Parameters

- **ARC_FILE**: The entry point of the application
- **ARC_EXECUTABLES**: A comma-separated list containing all files that should be executable (ARC_FILE is automatically added)

# Known Issues

1) Paths can't contain quotes (escaping doesn't work)
2) ARC_EXECUTABLES paths can't contain commas (escaping doesn't work)
