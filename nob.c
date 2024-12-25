#include <stdbool.h>
#define NOB_IMPLEMENTATION
#include "nob.h"

#define SRC_DIR "src"
#define BUILD_DIR "build"

void cflags(Nob_Cmd* cmd) {
    nob_cmd_append(cmd, "-Wall", "-Wextra", "-pedantic");
    nob_cmd_append(cmd, "-I.", "-Isrc/include", "-Isrc");
}

void cc(Nob_Cmd* cmd) {
    nob_cmd_append(cmd, "gcc");
    cflags(cmd);
}

void libs(Nob_Cmd* cmd) {
    nob_cmd_append(cmd, "-lm");
}

// ZOZIN TECHNOLOGY FROM PANIM
bool build_object_c(bool force, Nob_Cmd* cmd, const char* source_path, const char* output_path) {
    int rebuild_is_needed = nob_needs_rebuild1(output_path, source_path);
    if (rebuild_is_needed < 0)
        return false;

    if (force || rebuild_is_needed) {
        cmd->count = 0;
        cc(cmd);
        nob_cmd_append(cmd, "-o", output_path);
        nob_cmd_append(cmd, "-c", source_path);
        libs(cmd);
        return nob_cmd_run_sync(*cmd);
    }

    nob_log(NOB_INFO, "%s is up-to-date", output_path);
    return true;
}

bool build_exe(bool force, bool release_build, Nob_Cmd* cmd, const char* output_path, const char** input_paths, size_t input_paths_count) {
    int rebuild_is_needed = nob_needs_rebuild(output_path, input_paths, input_paths_count);
    if (rebuild_is_needed < 0)
        return false;

    if (force || rebuild_is_needed) {
        cmd->count = 0;
        cc(cmd);
        if (release_build) {
            nob_cmd_append(cmd, "-O3");
            nob_cmd_append(cmd, "-s");
        }
        nob_cmd_append(cmd, "-o", output_path);
        nob_da_append_many(cmd, input_paths, input_paths_count);
        libs(cmd);
        return nob_cmd_run_sync(*cmd);
    }

    nob_log(NOB_INFO, "%s is up-to-date", output_path);
    return true;
}

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char* program_name = nob_shift_args(&argc, &argv);
    (void)program_name; // Ignore ./nob

    bool force_rebuild = false;
    bool release_build = false;

    while (argc > 0) {
        const char* arg = nob_shift_args(&argc, &argv);
        if (strcmp(arg, "-f") == 0) {
            nob_log(NOB_INFO, "Forcing rebuild");
            force_rebuild = true;
        } else if (strcmp(arg, "-r") == 0) {
            nob_log(NOB_INFO, "Release build");
            release_build = true;
        } else {
            nob_log(NOB_ERROR, "Unknown argument: %s\n", arg);
        }
    }
    if (!nob_mkdir_if_not_exists(BUILD_DIR))
        return 1;

    Nob_Cmd cmd = { 0 };
    if (!build_object_c(force_rebuild, &cmd, SRC_DIR "/utils.c", BUILD_DIR "/utils.o"))
        return 1;
    if (!build_object_c(force_rebuild, &cmd, SRC_DIR "/terminal.c", BUILD_DIR "/terminal.o"))
        return 1;
    if (!build_object_c(force_rebuild, &cmd, SRC_DIR "/main.c", BUILD_DIR "/main.o"))
        return 1;

    {
        const char* output_path = BUILD_DIR "/main";
        const char* input_paths[] = {
            BUILD_DIR "/utils.o",
            BUILD_DIR "/terminal.o",
            BUILD_DIR "/main.o",
        };
        if (!build_exe(force_rebuild, release_build, &cmd, output_path, input_paths, sizeof(input_paths) / sizeof(input_paths[0])))
            return 1;
    }

    Nob_Cmd exe = { 0 };

    nob_cmd_append(&exe, "./build/main");
    nob_cmd_run_sync(exe);

    return 0;
}
