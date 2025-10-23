#ifndef COMMON_H
#define COMMON_H

#define _POSIX_C_SOURCE 199309L  // Required for clock_gettime

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <ncurses.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// Constants
#define BUFFER_SIZE 1024
#define MAX_ATTEMPTS 5
#define ROT4_SHIFT 4

// Encryption functions
void rot4_encrypt(char *input, char *output);
void rot4_decrypt(char *input, char *output);

#endif
