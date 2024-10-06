cc_library(
    name = "static",
    includes = ["."],
    srcs = [
        "Camera.cpp",
        "Light.cpp",
        "Main.cpp",
        "Mesh.cpp",
        "Model.cpp",
        "Shader.cpp",
    ],
    deps = [
        "//deps/GLFW:glfw_static",
        "//deps/glm:glm",
        "//deps/glad:glad",
        "//deps/assimp:assimp",
        "//deps/stb:image",
        "//deps/cpplocate:cpplocate",
    ],
    visibility = ["//visibility:public"],
)
