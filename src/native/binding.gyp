{
    "targets": [
        {
            "target_name": "embed-desktop",
            "sources": [
                "embed-desktop.cpp",
            ],
            "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
            "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "msvs_settings": {
                "VCCLCompilerTool": {"ExceptionHandling": 1},
            },
        }
    ]
}
