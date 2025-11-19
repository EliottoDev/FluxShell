//
// Created by elias on 19/11/2025.
//

#include "executor.h"

char* evaluate_to_string(node_t* node) {
    if (!node) return strdup("");

    switch (node->type) {
        case NODE_LITERAL:
            return strdup(node->value);

        case NODE_VAR_REF:
            // TODO: Implement variable searching
            // i find it very hard :(
            return strdup("[VAR]");

        case NODE_BINARY_OP:
            // TODO: Implement binary operation evaluation
            return strdup("[EXPRESSION]");

        default:
            return strdup("");
    }
}

int execute_function_node(const node_t* node) {
    char** args_str = malloc(sizeof(char*) * node->argc);
    for(int i = 0; i< node->argc; i++) {
        args_str[i] = evaluate_to_string(node->args[i]);
    }

    int status = 0;

    const builtin_t *internal_cmd = find_builtin(node->cname);

    if (internal_cmd != NULL) {
        status = internal_cmd->func(args_str, node->argc);
    } else {
        status = exec_external_program(node->cname, args_str, node->argc);
    }

    for(int i = 0; i < node->argc; i++) free(args_str[i]);
    free(args_str);

    return status;
}

#ifdef _WIN32
char* build_windows_command_line(const char* cmd, char** argv, const int count) {
    if (!cmd || !argv) return NULL;

    size_t len = strlen(cmd) + 1;
    for (int i = 0; i < count; i++) {
        len += strlen(argv[i]) + 1;
    }

    char* cmd_line = malloc(len);
    if (!cmd_line) return NULL;

    strcpy(cmd_line, cmd);
    for (int i = 0; i < count; i++) {
        strcat(cmd_line, " ");
        strcat(cmd_line, argv[i]);
    }
    return cmd_line;
}
#endif

int exec_external_program(char* command, char** args, const int arg_count) {

#ifdef _WIN32
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD exit_code = 1;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char* cmd_line = build_windows_command_line(command, args, arg_count);

    // CreateProcess = fork() + exec()
    if (!CreateProcess(
            NULL,           // Module name
            cmd_line,       // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory
            &si,            // Pointer to STARTUPINFO structure
            &pi             // Pointer to PROCESS_INFORMATION structure
        ))
    {
        printf("Execution Error: Couldn't execute '%s' (Error %lu)\n", command, GetLastError());
        free(cmd_line);
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    GetExitCodeProcess(pi.hProcess, &exit_code);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    free(cmd_line);

    return (int)exit_code;

#else
    char** argv = malloc(sizeof(char*) * (arg_count + 2));
    argv[0] = command;
    for (int i = 0; i < arg_count; i++) argv[i + 1] = args[i];
    argv[arg_count + 1] = NULL;

    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        if (execvp(command, argv) == -1) {
            perror("Flux Error");
        }
        exit(EXIT_FAILURE); // Kill child if error
    } else if (pid < 0) {
        // Error when cloning
        perror("Fork failed");
        free(argv);
        return 1;
    } else {
        // Father process (Shell)
        int status;
        waitpid(pid, &status, 0);
        free(argv);

        if (WIFEXITED(status)) return WEXITSTATUS(status);
        return 1;
    }
#endif
}

int execute(node_t* node) {
    if (!node) return 0;

    switch (node->type) {
        case NODE_FUNCTION:
            return execute_function_node(node);

        case NODE_FLUX:
            // TODO: Implement Pipes (->) (dup2/pipe)
            printf("Unimplemented\n");
            execute(node->left);
            return execute(node->right);

        case NODE_BLOCK:
            printf("INFO: Executing transactional block [ ... ]\n");
            return execute(node->left);

        case NODE_ERROR_HANDLER:
            if (execute(node->left) != 0) {
                printf("INFO: Error detected (!>), executing handler...\n");
                return execute(node->right);
            }
            return 0;

        default:
            return 0;
    }
}