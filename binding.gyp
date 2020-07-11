{
  "variables": {
    'v8_enable_pointer_compression': 0,
    'v8_enable_31bit_smis_on_64bit_arch': 0,
  },
  "targets": [
    {
      "target_name": "turbo-linecount",
      "sources": ["addon.cpp", "turbo_linecount.cpp"],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      "conditions": [
        ["OS==\"mac\"", {
          "cflags!": [ "-fno-exceptions" ],
          "cflags_cc!": [ "-fno-exceptions", "-pthread", "-Wl,--no-as-needed", "-ldl" ],
          "cflags_cc": [ "-Wno-ignored-qualifiers" ],
          'cflags+': ['-fvisibility=hidden'],
          "cflags": [ "-std=c++11", "-stdlib=libc++" ],
          "include_dirs": [
            "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1"
          ],
          "xcode_settings": {
            'BUILD_DIR':'build',
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "GCC_ENABLE_CPP_RTTI": "YES",
            "OTHER_CFLAGS": ["-mmacosx-version-min=10.7", "-std=c++11", "-stdlib=libc++"],
            "OTHER_CPLUSPLUSFLAGS": ["-mmacosx-version-min=10.7", "-std=c++11", "-stdlib=libc++"],
            'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES'
          }
        }]
      ]
    }
  ]
}
