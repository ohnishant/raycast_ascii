#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_Cmd cmd = { 0 };

    nob_cmd_append(&cmd, "cc", "-Isrc/include", "-Wall", "-Wextra", "-o", "build/main", "src/main.c");
    if (!nob_cmd_run_sync(cmd))
        return 1;
    Nob_Cmd exe = { 0 };

    nob_cmd_append(&exe, "./build/main");
    nob_cmd_run_sync(exe);

    return 0;
}
