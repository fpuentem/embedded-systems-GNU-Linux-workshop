#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define GPIO_PIN "12"  // Example: GPIO15 (change as needed)
#define GPIO_PATH "/sys/class/gpio"
#define BUFFER_SIZE 64

void write_to_file(const char *path, const char *value) {
    int fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }
    if (write(fd, value, strlen(value)) == -1) {
        perror("Error writing to file");
        close(fd);
        exit(1);
    }
    close(fd);
}

int main() {
    char path[BUFFER_SIZE];

    // Export the GPIO pin
    write_to_file(GPIO_PATH "/export", GPIO_PIN);

    // Set direction to output
    snprintf(path, BUFFER_SIZE, GPIO_PATH "/gpio%s/direction", GPIO_PIN);
    write_to_file(path, "out");

    // Blink loop
    snprintf(path, BUFFER_SIZE, GPIO_PATH "/gpio%s/value", GPIO_PIN);
    for (int i = 0; i < 10; i++) {
        write_to_file(path, "1");
        sleep(1);
        write_to_file(path, "0");
        sleep(1);
    }

    // Unexport the pin
    write_to_file(GPIO_PATH "/unexport", GPIO_PIN);

    return 0;
}
