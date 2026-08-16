#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_fd;
    int client_fd;
    char *led_path = "/sys/class/leds/beaglebone:green:usr0/brightness";
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5000);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        printf("Client Connected!\n");

        char buffer[10] = {0};
        recv(client_fd, buffer, sizeof(buffer), 0);

        FILE *led_file = fopen(led_path, "w");
        if (led_file == NULL) {
            printf("ERROR!");
            return 1;
        }
        fprintf(led_file, "%s", buffer);
        fclose(led_file);

        printf("Received: %s\n", buffer);
        close(client_fd);
    }

    return 0;
}
