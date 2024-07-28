#define NOB_IMPLEMENTATION
#include "nob.h"

typedef struct {
    const char *src_path;
    const char *bin_path;
    const char *wasm_path;
} Example;

Example examples[] = {
    {
        .src_path   = "./examples/procedural_snake.c",
        .bin_path   = "./build/procedural_snake",
        .wasm_path  = "./wasm/procedural_snake.wasm",
    },
};

bool build_native(void)
{
    Nob_Cmd cmd = {0};
    for (size_t i = 0; i < NOB_ARRAY_LEN(examples); ++i) {
        cmd.count = 0;
        nob_cmd_append(&cmd, "clang", "-I./include/");
        nob_cmd_append(&cmd, "-o", examples[i].bin_path, examples[i].src_path);
        nob_cmd_append(&cmd, "-L./lib/", "-lraylib", "-lm");
        if (!nob_cmd_run_sync(cmd)) return 1;
    }
}

bool build_wasm(void)
{
    Nob_Cmd cmd = {0};
    for (size_t i = 0; i < NOB_ARRAY_LEN(examples); ++i) {
        cmd.count = 0;
        nob_cmd_append(&cmd, "clang");
        nob_cmd_append(&cmd, "--target=wasm32");
        nob_cmd_append(&cmd, "-I./include");
        nob_cmd_append(&cmd, "--no-standard-libraries");
        nob_cmd_append(&cmd, "-Wl,--export-table");
        nob_cmd_append(&cmd, "-Wl,--no-entry");
        nob_cmd_append(&cmd, "-Wl,--allow-undefined");
        nob_cmd_append(&cmd, "-Wl,--export=main");
        nob_cmd_append(&cmd, "-o");
        nob_cmd_append(&cmd, examples[i].wasm_path);
        nob_cmd_append(&cmd, examples[i].src_path);
        nob_cmd_append(&cmd, "-DPLATFORM_WEB");
        if (!nob_cmd_run_sync(cmd)) return 1;
    }
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    if (!nob_mkdir_if_not_exists("build/")) return 1;
    build_native();
    build_wasm();
    return 0;
}
