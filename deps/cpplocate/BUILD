load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)

cmake(
    name = "cpplocate",
    lib_source = ":srcs",
    generate_args = ["-DBUILD_SHARED_LIBS=OFF"],
    out_static_libs = ["cpplocate.lib", "liblocate.lib"],
    visibility = ["//visibility:public"],
)
