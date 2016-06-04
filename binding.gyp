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
        "<!(node -e \"console.log(require.resolve('nangraph.cc/gyp/nangraph.cc.gyp') + ':nangraph')\")"
      ],
      'all_dependent_settings': {
        'include_dirs': [
          './src'
        ],
      },
      "cflags" : [ "-std=c++11" ],
      "conditions": [
          [ 'OS!="win"', {
            "cflags+": [ "-std=c++11" ],
            "cflags_c+": [ "-std=c++11" ],
            "cflags_cc+": [ "-std=c++11" ],
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
