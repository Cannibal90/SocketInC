#include "common.h"

struct data date_to_process;

int main(){
    int error_status;
    
    int server_file_descriptor, client_file_descryptor;
    socklen_t size_of_server_struct, size_of_client_struct;    

    struct sockaddr_in server_address, client_address;
    int connection_status;

    server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if(server_file_descriptor == -1){
        perror("Something gone wrong!!\n");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    size_of_server_struct = sizeof(server_address);

    //przypisanie struktury do socketu
    error_status = bind(server_file_descriptor, (struct sockaddr *)&server_address, size_of_server_struct);
    if(error_status == -1){
        perror("Something gone wrong!!\n");
        exit(1);
    }

    //ustawienie kolejki -> tylko 5 zgloszen bedzie pzyjetych, reszta bedzie odrzucona
    error_status = listen(server_file_descriptor, 5);
    if(error_status == -1){
        perror("Something gone wrong!!\n");
        exit(1);
    }

    while(1){
        printf("server wating...\n");
        size_of_client_struct = sizeof(client_address);
        client_file_descryptor = accept(server_file_descriptor, (struct sockaddr *) &client_address, &size_of_client_struct);

        //forkowanie procesu -> tworzymy kopie procesu dla danego klienta
        //pamiec procesu jest kopiowana do dziecka
        //jesli zwrocone jest 0 oznacza to ze jestesmy w procesie potomnynm a nie macierzystym
        //jesli jestesmy procesem macierzystym, konczymy polaczenie
        if(fork() == 0){
            safe_read(client_file_descryptor, &date_to_process, sizeof(struct data));
            printf("---------------------------\n");
            printf("Received new data!\n");
            show_struct(&date_to_process);
            int type = check_type(&date_to_process);
            if(type == GET_SQRT){
                get_sqrt(&date_to_process);
            }
            else if(type == GET_TIME)
            {
                 get_time(&date_to_process);
            }
            printf("Before send(SQRT IN BIG ENDIAN!!): \n");
            show_struct(&date_to_process);
            sleep(3);
            safe_write(client_file_descryptor, &date_to_process, sizeof(struct data));
            close(client_file_descryptor);
            printf("---------------------------\n");
            exit(0);
        }
        else
        {
            close(client_file_descryptor);
        }
        
    }

       exit(0);
}