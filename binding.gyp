{
  "targets": [
    {
      "target_name": "nangraph",
      "sources": [
        "src/index.cc",
        "src/NanGraph.cc",
        "src/NanGraph.h",
        "src/NanGraphUtils.cc",
        "src/NanGraphUtils.h",
        "src/Forwarder.cc",
        "src/Forwarder.h",
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "dependencies": [
        "<!(node -e \"console.log(require.resolve('nangraph.cpp/gyp/package.gyp') + ':nangraph')\")"
      ],
      "cflags" : [ "-std=c++11", "-stdlib=libc++" ],
      "conditions": [
          [ 'OS!="win"', {
            "cflags+": [ "-std=c++14" ],
            "cflags_c+": [ "-std=c++14" ],
            "cflags_cc+": [ "-std=c++14" ],
          }],
          [ 'OS=="mac"', {
            "xcode_settings": {
              "OTHER_CPLUSPLUSFLAGS" : [ "-std=c++11", "-stdlib=libc++" ],
              "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
              "MACOSX_DEPLOYMENT_TARGET": "10.7"
            },
          }],
        ],
    }
  ]
}
