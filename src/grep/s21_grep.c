#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#define E_FLAG 0b1
#define I_FLAG 0b10
#define V_FLAG 0b100
#define C_FLAG 0b1000
#define L_FLAG 0b10000
#define N_FLAG 0b100000

int PrintResult(FILE* file, int flags, regex_t* regex, int num_of_files, char* file_name);
int GetFlags(int argc, char* argv[], int* flags_ptr);

int main(int argc, char* argv[]) {
    int flags = 0;
    int reg_flag = 0;
    int goal_fiile = 0;
    int num_of_files = 0;

    int i = GetFlags(argc, argv, &flags);
    if (flags & I_FLAG) {
        reg_flag |= REG_ICASE;
    }
    if (flags & E_FLAG) {
        reg_flag |= REG_EXTENDED;
    }
    regex_t* regex = malloc(sizeof(regex_t));
    if (i < argc) {
    regcomp(regex, argv[i], reg_flag);
    }
    i++;
    num_of_files = argc - i;

    for (; i < argc; ++i) {
        FILE* file = fopen(argv[i], "r");
        if (!file) {
            printf("%s: No such file or directory\n", argv[i]);
        } else {
            goal_fiile = PrintResult(file, flags, regex, num_of_files, argv[i]);
            if (goal_fiile == 1 && flags & L_FLAG) {
                printf("%s\n", argv[i]);
            }
            fclose(file);
        }
    }
    regfree(regex);
    free(regex);

    return 0;
}

int PrintResult(FILE* file, int flags, regex_t* regex, int num_of_files, char* file_name) {
    char* string = NULL;
    size_t str_size = 0;
    regmatch_t match[1];
    int couter = 0;
    int line_counter = 0;
    int goal_flag = 0;

    while (!feof(file)) {
        line_counter++;
        getline(&string, &str_size, file);
        int find = regexec(regex, string, 1, match,  0);

        if (find == 0 && flags & C_FLAG) {
            couter++;
        } else if ((find == 0 && !(flags & V_FLAG)) || (find == 1 && flags & V_FLAG)) {
            if (num_of_files > 1 && !(flags & L_FLAG)) {
                printf("%s:", file_name);
            }
            if (flags & N_FLAG) {
                printf("%d:", line_counter);
            }
            if (flags & L_FLAG) {
                goal_flag = 1;
            } else {
                printf("%s", string);
                if ((flags & (V_FLAG | E_FLAG | I_FLAG | N_FLAG)) && \
                (feof(file)) && (string[strlen(string) - 1] != '\n')) {
                    printf("\n");
                }
            }
        }
    }
    if (flags & C_FLAG) {
        if (num_of_files > 1) {
            printf("%s:", file_name);
        }
        printf("%d\n", couter);
    }

    free(string);
    return goal_flag;
}

int GetFlags(int argc, char* argv[], int* flags_ptr) {
    int i = 1;
    int flags = 0;
    while (i < argc && argv[i][0] == '-') {
        if (!strcmp(argv[i], "-e")) {
            flags |= E_FLAG;
            i++;
            break;
        } else if (!strcmp(argv[i], "-i")) {
            flags |= I_FLAG;
        } else if (!strcmp(argv[i], "-v")) {
            flags |= V_FLAG;
        } else if (!strcmp(argv[i], "-c")) {
            flags |= C_FLAG;
        } else if (!strcmp(argv[i], "-l")) {
            flags |= L_FLAG;
        } else if (!strcmp(argv[i], "-n")) {
            flags |= N_FLAG;
        }
        i++;
    }
    *flags_ptr = flags;
    return i;
}
