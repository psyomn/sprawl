cc_binary(
    name = "examples_sdl",

    srcs = [
        "render_tilemap_example.cc",
    ],

    linkopts=["-lSDL2", "-lSDL2_image", "-lpthread"],

    deps = [
        "//gamejam:gamejam"
    ],

    data = [
        "//gamejam/examples_sdl:sprites"
    ]
)

filegroup(
    name = "sprites",
    srcs = [
      "assets/Autumn_Ground_32x32.png",
      "assets/Summer_Ground_32x32.png",
    ],
)
