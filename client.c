#include "common.h"

int main(){
    //zmienne do odczytywania i wysylani danych pomiedzy socketami
    struct data date_to_read;
    struct data date_to_send;
    
    //deskryptor pliku dla danego klienta
    int client_file_descriptor;

    //rozmiar struktury sockaddr_in
    socklen_t client_size_of_struct;    

    //struktura przechowujaca takie zmienne zwiazane z socketami i polaczeniem
    struct sockaddr_in client_address;
    int connection_status;

    //tworzenie socketu
    //argumenty to: Rodzina protokolow, okreslenie protokolu, 3 argument nie jest istotny
    client_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(client_file_descriptor == -1){
        perror("Something gone wrong!!\n");
        exit(1);
    }

    //ustawienie zmiennych dla struktury sockaddr_in
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_address.sin_port = htons(9734);//konwersja na BIG ENDIAN
    client_size_of_struct = sizeof(client_address);

    //rzutujemy na strukture sockaddr poniewaz jest ona ogolniejsza w tym przypadku
    //mozemy wysylac nie tylko przez TCP
    connection_status = connect(client_file_descriptor, (struct sockaddr *) &client_address, client_size_of_struct);
    if(connection_status == -1){
        perror("Something gone wrong...");
        exit(1);
    }
    
    int choice;
    printf("Check 1 - to get time, 2 - to get square root\n");
    scanf("%d",&choice);
    if(choice == 1){
        set_type(&date_to_send, GET_TIME);
        date_to_send.request_id = ++id;
    }
    else if(choice == 2){
        double variable;
        printf("Input number to sqrt: ");
        scanf("%lf", &variable);
        set_type(&date_to_send, GET_SQRT);
        date_to_send.request_id = ++id;
        if(__FLOAT_WORD_ORDER__ == LITTLE_ENDIAN){
            variable = reverse_number(variable);
        }
        memcpy(date_to_send.variable,&variable,sizeof(double));
    }
    else
    {
        printf("Wrong number!\n");
        close(client_file_descriptor);
        exit(1);
    }

    printf("---------------------------\n");
    printf("Before send: \n");
    show_struct(&date_to_send);
    safe_write(client_file_descriptor, &date_to_send, sizeof(struct data));
    safe_read(client_file_descriptor, &date_to_read, sizeof(struct data));
    printf("Received answer: \n");
    if(__FLOAT_WORD_ORDER__ == LITTLE_ENDIAN){
        double variable;
        memcpy(&variable, date_to_read.variable, sizeof(double));
        variable = reverse_number(variable);
        memcpy(date_to_read.variable, &variable, sizeof(double));
    }
    show_struct(&date_to_read);
    close(client_file_descriptor);
    printf("---------------------------\n");
    exit(0);
}