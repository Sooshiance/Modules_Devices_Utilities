#include <stdio.h>
#include <stdlib.h>

void change_dns(const char *dns)
{
    FILE *file = fopen("/etc/resolv.conf", "w");
    if (file == NULL)
    {
        perror("Failed to open /etc/resolv.conf");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "nameserver %s\n", dns);
    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <DNS_IP>\n", argv[0]);
        return EXIT_FAILURE;
    }

    change_dns(argv[1]);

    printf("DNS changed to %s\n", argv[1]);

    return EXIT_SUCCESS;
}
