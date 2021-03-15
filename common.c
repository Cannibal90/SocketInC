#include "common.h"

int check_type(struct data* d){
    if(d->first_b == 0 && d->second_b == 0 && d->third_b == 0 && d->four_b == 2){
        return GET_TIME;
    }
    else if(d->first_b == 1 && d->second_b == 0 && d->third_b == 0 && d->four_b == 2){
        return SET_TIME;
    }
    else if(d->first_b == 0 && d->second_b == 0 && d->third_b == 0 && d->four_b == 1){
        return GET_SQRT;
    }
    else if(d->first_b == 1 && d->second_b == 0 && d->third_b == 0 && d->four_b == 1){
        return SET_SQRT;
    }
    return -1;
}

void set_type(struct data *d, int type){
    if(type == GET_TIME){
        d->first_b = 0; d->second_b = 0; d->third_b = 0; d->four_b = 2;
    }
    else if(type == SET_TIME){
        d->first_b = 1; d->second_b = 0; d->third_b = 0; d->four_b = 2;
    }
    else if(type == GET_SQRT){
        d->first_b = 0; d->second_b = 0; d->third_b = 0; d->four_b = 1;
    }
    else if(type == SET_SQRT){
        d->first_b = 1; d->second_b = 0; d->third_b = 0; d->four_b = 1;
    }
}

//LITTLE_ENDIAN - MSB z lewej strony -> maszyny oparte na intelu
//BIG_ENDIAN - MSB z prawej strony -> dane wysylane i odbierane po sieci
//file <bits/endian.h> defines order for the particular machine 
void get_sqrt(struct data* d){
    d->request_id = ++id;
    set_type(d,SET_SQRT);
    double square_root;
    memcpy(&square_root, d->variable, sizeof(double));

    if(__FLOAT_WORD_ORDER__ == LITTLE_ENDIAN){
        square_root = reverse_number(square_root);
    }
    square_root = sqrt(square_root);
    if(__FLOAT_WORD_ORDER__ == LITTLE_ENDIAN){
        square_root = reverse_number(square_root);
    }
    memcpy(d->variable, &square_root, sizeof(double));
}

void get_time(struct data* d){
    d->request_id = ++id;
    set_type(d,SET_TIME);
    time_t time_to_set = time(NULL);
    size_t size = sizeof(time_to_set);
    memcpy(d->variable, &size, sizeof(time_to_set));
    memcpy(d->variable + sizeof(time_to_set), &time_to_set, sizeof(time_to_set));
}

void show_sqrt(struct data* d){
    double square_root; 
    memcpy(&square_root, d->variable, sizeof(double));
    printf("Square root: %lf\n", square_root);
}

void show_time(struct data* d){
    size_t size;
    time_t time_to_get;
    struct tm* tm_info;
    char time_to_show[20];
    memcpy(&size, d->variable, sizeof(size));
    memcpy(&time_to_get, d->variable+size, sizeof(size));
    tm_info = localtime(&time_to_get);
    strftime(time_to_show, 20, "%H:%M:%S",tm_info);
    printf("time: %s\n", time_to_show);
}

int safe_read(int file_descriptor, void* buffer, size_t size_to_read){
    size_t readed_bytes = 0;
    int status_read;
    while(readed_bytes < size_to_read){
        if((status_read = read(file_descriptor, (char*)buffer + readed_bytes, size_to_read - readed_bytes)) == -1){
            if(errno != EINTR){
                return -1;
            }
        }
        readed_bytes += status_read;
    }
    return 0;
}

int safe_write(int file_descriptor, void* buffer, size_t size_to_write){
    size_t written_bytes = 0;
    int status_write;
    while(written_bytes < size_to_write){
        if((status_write = write(file_descriptor, (char*)buffer + written_bytes, size_to_write - written_bytes)) == -1){
            if(errno != EINTR){
                return -1;
            }
        }
        written_bytes += status_write;
    }
    return 0;
}

void show_struct(struct data* d){
    
    printf("CODE: %d %d %d %d\n", d->first_b, d->second_b, d->third_b, d->four_b);
    printf("ID: %d\n", d->request_id);
    int type = check_type(d);
    if(type == SET_TIME){
        show_time(d);
    }
    else if(type == SET_SQRT){
        show_sqrt(d);
    }
    
}

//LITTLE_ENDIAN - MSB z lewej strony -> maszyny oparte na intelu
//BIG_ENDIAN - MSB z prawej strony -> dane wysylane i odbierane po sieci
//plik <byte/endian.h> okresla dla danej maszyny jakiej kolejnosci uzywa BIG czy LITTLE ENDIAN
double reverse_number(double number){
    //konwersja kolejnosci bajtow hosta do BIG ENDIAN
    //odwrotnoscia jest funkcja htole64
    return htobe64(number);
}
