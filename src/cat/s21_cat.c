#include <stdio.h>
#include <string.h>

#define B_FLAG 0b1
#define E_FLAG 0b10
#define N_FLAG 0b1000
#define S_FLAG 0b10000
#define T_FLAG 0b100000
#define V_FLAG 0b1000000

void PrintFile(char* filename, int flags);
int GetFlags(int argc, char* argv[], int* flags_ptr);

int main(int argc, char* argv[]) {
    int flags = 0;
    int i = GetFlags(argc, argv, &flags);
    if (argc >= 2) {
        for (; argc > i; ++i) {
            PrintFile(argv[i], flags);
        }
    }
     return 0;
}

void PrintFile(char* filename, int flags) {
    int count = 2;
    int first_line = 1;
    int c;
    int prev_char = '\n';
    int prev_prev = 0;

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("%s: No such file or directory\n", filename);
    } else {
        c = getc(file);
        while (c != EOF) {
            if (first_line == 1 && ((flags & B_FLAG && c != '\n') || (flags & N_FLAG))) {
                printf("%6d\t", first_line);
                first_line = 0;
            } else if ((flags & B_FLAG) && ((prev_char == '\n' && c != '\n')) && first_line == 0) {
                    printf("%6d\t", count++);
            } else if ((flags & N_FLAG) && (prev_char == '\n')) {
                    printf("%6d\t", count++);
            }
            if ((flags & E_FLAG) && (c == '\n')) {
                    printf("$");
            }
            if ((flags & T_FLAG) && (c == '\t')) {
                printf("^I");
            } else if (flags & V_FLAG) {
                if ((c >= 0) && (c <= 31) && (c != 9) && (c != 10)) {
                    printf("^");
                    c +=64;
                } else if (c == 127) {
                    printf("^");
                    c -= 64;
                }

                printf("%c", c);

            } else  if (!(((flags & S_FLAG) && prev_prev == '\n' && prev_char == '\n' && c == '\n'))) {
                printf("%c", c);
            }

            prev_prev = prev_char;
            prev_char = c;
            c = getc(file);
        }
        fclose(file);
    }
}

int GetFlags(int argc, char* argv[], int* flags_ptr) {
    int i = 1;
    int flags = 0;
    while (i < argc && argv[i][0] == '-') {
        if (!strcmp(argv[i], "-b") || (!strcmp(argv[i], "--number-nonblank"))) {
            flags |= B_FLAG;
        } else if (!strcmp(argv[i], "-e")) {
            flags |= E_FLAG;
            flags |= V_FLAG;
        } else if (!strcmp(argv[i], "-E")) {
            flags |= E_FLAG;
        } else if (!strcmp(argv[i], "-s") || (!strcmp(argv[i], "--squeeze-blank"))) {
            flags |= S_FLAG;
        } else if (!strcmp(argv[i], "-n") || (!strcmp(argv[i], "--number"))) {
            flags |= N_FLAG;
        } else if (!strcmp(argv[i], "-t") || (!strcmp(argv[i], "-T"))) {
            flags |= T_FLAG;
            flags |= V_FLAG;
        } else if (!strcmp(argv[i], "-v")) {
            flags |= V_FLAG;
        }
        i++;
    }
    *flags_ptr = flags;
    return i;
}


