{
    "targets": [
        {
            "target_name": "djiThermal",
            "sources": ["src/thermal.cc"],
            "libraries": ["-L<!(pwd)/src/libs", "-ldirp"],
            "ldflags": ["-Wl,-rpath,'$$ORIGIN'"],
            "include_dirs": ["src"],
        }
    ],
}
