#include <ced/string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    string_t str = string_new();

    string_pushstr(&str, "Hello, ");
    string_pushstr(&str, "world!: ");
    string_pushstr(&str, __FILE__);

    printf("%s\n", string_into(&str));

    string_free(&str);

    return 0;
}
