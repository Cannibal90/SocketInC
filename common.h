#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <endian.h>

#define GET_TIME 1
#define SET_TIME 2
#define GET_SQRT 3
#define SET_SQRT 4

struct data {
    char first_b;
    char second_b;
    char third_b;
    char four_b;
    u_int32_t request_id;
    char variable[64];
};

static int id = 1;

int check_type(struct data* d);
void set_type(struct data *d, int type);

void get_sqrt(struct data* d);
void show_sqrt(struct data* d);
void get_time(struct data* d);
void show_time(struct data* d);

void show_struct(struct data* d);

int safe_write(int file_descriptor, void* buffer, size_t size_to_write);
int safe_read(int file_descriptor, void* buffer, size_t size_to_read);

double reverse_number(double number);

#endif 