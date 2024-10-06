load("@aspect_bazel_lib//lib:copy_to_directory.bzl", "copy_to_directory")

copy_to_directory(
    name = "res",
    srcs = ["model", "texture", "shader"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "bin",
    data = [
        "res",
    ],
    deps = [
        "//engine:static",
    ],
)
