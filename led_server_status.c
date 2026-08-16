#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

int main() {

int server_fd;
int client_fd;
int led_state = 0;
char *led_path = "/sys/class/leds/beaglebone:green:usr0/brightness";
struct sockaddr_in address;
socklen_t addrlen = sizeof(address);

server_fd = socket(AF_INET, SOCK_STREAM, 0);
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(5000);
  
bind(server_fd, (struct sockaddr*)&address, sizeof(address));
listen(server_fd, 3);

while(1){
client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
printf("Client Connected!\n");
char buffer[10] = {0};
recv(client_fd, buffer, sizeof(buffer), 0);

if(strcmp(buffer, "1")==0){
FILE *led_file = fopen(led_path, "w");
fprintf(led_file, "1");
fclose(led_file);
led_state = 1;
}
else if (strcmp(buffer, "0")==0){
FILE *led_file = fopen(led_path, "w");
fprintf(led_file, "0");
fclose(led_file);
led_state = 0;
}
else if (strcmp(buffer, "status")==0){
char *msg;
if (led_state == 1)
msg = "LED Status is ON\n";
else
msg = "LED Status is OFF\n";
size_t len = strlen(msg);
send(client_fd, msg, len, 0);

}
close(client_fd);
}
return 0;
}
