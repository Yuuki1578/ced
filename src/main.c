#include <ced/string.h>
#include <stdio.h>

int main(int argc, char **argv, char **env) {
    String str = string_new();

    string_pushstr(&str, "Hello!");

    printf("%s\n", string(&str));
    printf("%lu\n", str.layout.cap);

    string_clip(&str);

    printf("%s\n", string(&str));
    printf("%lu\n", str.layout.cap);

    string_dealloc(&str);

    return 0;
}
