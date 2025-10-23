#include "../include/common.h"

// Global variables
WINDOW *chat_win, *input_win;
int send_fd, recv_fd;
volatile bool running = true;

// ROT4 encryption function
void rot4_encrypt(char *input, char *output) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            char base = isupper(input[i]) ? 'A' : 'a';
            output[i] = (input[i] - base + ROT4_SHIFT) % 26 + base;
        } else {
            output[i] = input[i];
        }
    }
    output[strlen(input)] = '\0';
}

// ROT4 decryption function
void rot4_decrypt(char *input, char *output) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            char base = isupper(input[i]) ? 'A' : 'a';
            output[i] = (input[i] - base - ROT4_SHIFT + 26) % 26 + base;
        } else {
            output[i] = input[i];
        }
    }
    output[strlen(input)] = '\0';
}

// Thread function to receive messages
void *receive_task(void *arg) {
    char buf[BUFFER_SIZE], decrypted[BUFFER_SIZE];
    struct timespec start, end;
    
    while (running) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        int len = recv(recv_fd, buf, sizeof(buf) - 1, 0);
        if (len <= 0) {
            wprintw(chat_win, "Peer disconnected.\n");
            wrefresh(chat_win);
            break;
        }
        buf[len] = '\0';
        
        // Remove newline if present
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
        
        rot4_decrypt(buf, decrypted);
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_taken_ms = (end.tv_sec - start.tv_sec) * 1000.0 + 
                              (end.tv_nsec - start.tv_nsec) / 1000000.0;
        
        wprintw(chat_win, "Peer: %s [%.2f ms]\n", decrypted, time_taken_ms);
        wrefresh(chat_win);
    }
    return NULL;
}

int main() {
    char peer_ip[16];
    int my_port = 5556;   // Listen on port 5556
    int peer_port = 5555; // Connect to peer on port 5555

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Create windows
    int msg_height = LINES - 2;
    int width = COLS;
    chat_win = newwin(msg_height, width, 0, 0);
    scrollok(chat_win, TRUE);
    input_win = newwin(2, width, msg_height, 0);
    wrefresh(chat_win);
    wrefresh(input_win);

    // Get peer IP
    echo();
    mvwprintw(input_win, 1, 1, "Enter peer IP: ");
    wrefresh(input_win);
    wgetnstr(input_win, peer_ip, sizeof(peer_ip) - 1);
    noecho();
    wclear(input_win);
    wrefresh(input_win);

    // Validate peer IP
    if (inet_addr(peer_ip) == INADDR_NONE) {
        wprintw(chat_win, "Invalid IP address!\n");
        wrefresh(chat_win);
        getch();
        endwin();
        exit(1);
    }

    // Create and setup listening socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        endwin();
        perror("Socket creation failed");
        exit(1);
    }

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(my_port);

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        endwin();
        perror("Bind failed");
        exit(1);
    }

    if (listen(listen_fd, 5) == -1) {
        endwin();
        perror("Listen failed");
        exit(1);
    }

    wprintw(chat_win, "Listening on port %d...\n", my_port);
    wrefresh(chat_win);

    // Start receive thread first to accept connections
    wprintw(chat_win, "Waiting for incoming connection...\n");
    wrefresh(chat_win);
    
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    recv_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
    if (recv_fd == -1) {
        endwin();
        perror("Accept failed");
        exit(1);
    }

    wprintw(chat_win, "Incoming connection accepted from %s:%d\n", 
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    wrefresh(chat_win);

    // Connect to peer
    send_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (send_fd == -1) {
        endwin();
        perror("Client socket creation failed");
        exit(1);
    }

    struct sockaddr_in peer_addr;
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr.s_addr = inet_addr(peer_ip);
    peer_addr.sin_port = htons(peer_port);

    wprintw(chat_win, "Connecting to %s:%d...\n", peer_ip, peer_port);
    wrefresh(chat_win);
    
    if (connect(send_fd, (struct sockaddr *)&peer_addr, sizeof(peer_addr)) == -1) {
        wprintw(chat_win, "Connection failed. Make sure peer is running.\n");
        wrefresh(chat_win);
        getch();
        close(send_fd);
        close(recv_fd);
        close(listen_fd);
        endwin();
        exit(1);
    }

    wprintw(chat_win, "Connected to peer!\n");
    wrefresh(chat_win);

    // Start receive thread
    pthread_t thread;
    pthread_create(&thread, NULL, receive_task, NULL);

    // Main chat loop
    char msg[BUFFER_SIZE], encrypted[BUFFER_SIZE];
    struct timespec start, end;
    
    wclear(chat_win);
    wprintw(chat_win, "Chat started! Type /quit to exit.\n");
    wrefresh(chat_win);

    while (running) {
        wclear(input_win);
        mvwprintw(input_win, 0, 0, "Enter message: ");
        mvwprintw(input_win, 1, 0, "> ");
        wrefresh(input_win);

        echo();
        wgetnstr(input_win, msg, sizeof(msg) - 1);
        noecho();

        if (strncmp(msg, "/quit", 5) == 0) {
            running = false;
            break;
        }

        if (strlen(msg) == 0) continue;

        clock_gettime(CLOCK_MONOTONIC, &start);
        
        rot4_encrypt(msg, encrypted);
        if (send(send_fd, encrypted, strlen(encrypted), 0) == -1) {
            wprintw(chat_win, "Send failed!\n");
            wrefresh(chat_win);
            break;
        }
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_taken_ms = (end.tv_sec - start.tv_sec) * 1000.0 + 
                              (end.tv_nsec - start.tv_nsec) / 1000000.0;

        wprintw(chat_win, "You: %s [%.2f ms]\n", msg, time_taken_ms);
        wrefresh(chat_win);
    }

    // Cleanup
    running = false;
    close(recv_fd);
    close(send_fd);
    close(listen_fd);
    pthread_join(thread, NULL);
    endwin();
    printf("Chat closed gracefully.\n");
    return 0;
}
