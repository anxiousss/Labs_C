#include "solution.h"

void print_menu() {
    printf("1 - HELP\n 2 - ADD\n 3 - REMOVE\n 4 - PRINT\n 5 - SEARCH\n 6 - EXIT");
}

void print_help() {
    printf("2/ADD - Add a new mail to post.\n");
    printf("3/REMOVE - Remove a specific mail from post.\n");
    printf("4/PRINT - Print a content of post.\n");
    printf("5/SEARCH - Search a mail in post\n");
    printf("6/EXIT - End of dialogue\n");
}

int read_choice(int* choice) {
    scanf("%d", choice);
    if (*choice < 1 || *choice > 6)
        return Invalid_input;
    return 0;
}

int read_address(String* city, String* street, unsigned int* house, String* building, unsigned int* apartment, String* post_index) {
    char* tmp_city, *tmp_street, *tmp_building, *tmp_post_index;
    int err = read_line(&tmp_city);
    if (err)
        return err;
    err = read_line(&tmp_street);
    if (err)
        return err;
    scanf("%u", house);
    err = read_line(&tmp_building);
    if (err)
        return err;
    scanf("%u", apartment);
    err = read_line(&tmp_post_index);
    if (err)
        return err;

    int c = fgetc(stdin);
    if (c == EOF)
        return err;

    err = init_string(city, tmp_city);
    if (err)
        return err;
    err = init_string(street, tmp_street);
    if (err)
        return err;
    err = init_string(building, tmp_building);
    if (err)
        return err;
    err = init_string(post_index, tmp_post_index);
    if (err)
        return err;

    return 0;
}

int read_mail(float* weight, String* id, String* creation_date, String* delivery_date) {
    scanf("%f", weight);
    char* tmp_id, *tmp_creation_date, *tmp_delivery_date;
    int err = read_line(&tmp_id);
    if (err)
        return err;
    err = read_line(&tmp_creation_date);
    if (err)
        return err;
    err = read_line(&tmp_delivery_date);
    if (err)
        return err;

    err = init_string(id, tmp_id);
    if (err)
        return err;
    err = init_string(creation_date, tmp_creation_date);
    if (err)
        return err;
    err = init_string(delivery_date, tmp_delivery_date);
    if (err)
        return err;
    return 0;
}

int read_init_mail(Mail* mail, Address* address, String* city, String* street, unsigned int* house, String* building, unsigned int* apartment, String* post_index, float* weight, String* id, String* creation_date, String* delivery_date) {
    int err = read_address(city, street, house, building, apartment, post_index);
    if (err)
        return  Memory_leak;
    err = init_address(&address, city, street, *house, building, *apartment, post_index);
    if (err)
        return Memory_leak;

    err = read_mail(weight, id, creation_date, delivery_date);
    if (err)
        return err;
    err = init_mail(&mail, address, *weight, id, creation_date, delivery_date);
    if (err)
        return err;
    return 0;
}

void print_mails(Post* post) {
    if (post == NULL || post->mails == NULL) {
        printf("Ошибка: некорректные входные данные\n");
        return;
    }

    printf("Список почтовых отправлений:\n");
    for (int i = 0; i < post->length; i++) {
        Mail* mail = post->mails[i];
        printf("Mail ID: %s\n", mail->id.mas);
        printf("  Address: %s, %s, %u, %s, %u, %s\n",
               mail->address.city.mas, mail->address.street.mas, mail->address.house,
               mail->address.building.mas, mail->address.apartment, mail->address.post_index.mas);
        printf("  Weight: %.2f\n", mail->weight);
        printf("  Creation Date: %s\n", mail->creation_date.mas);
        printf("  Delivery Date: %s\n", mail->delivery_date.mas);
        printf("\n");
    }
}


int handle_choice(int choice, int* flag, Post* post) {
    Mail* mail;
    Address* address;
    String city, street, building, post_index;
    unsigned house, apartment;
    float weight;
    String id, creation_date, delivery_date;

    int err;
    switch (choice) {
        case HELP:
            print_help();
            break;
        case ADD:
            if (*flag == 0) {
                Address* post_address;
                String post_city, post_street, post_building, post_post_index;
                unsigned int post_house, post_apartment;
                err = read_address(&post_city, &post_street, &post_house, &post_building, &post_apartment, &post_post_index);
                if (err)
                    return err;
                err = init_address(&post_address, &post_city, &post_street, post_house, &post_building, post_apartment, &post_post_index);
                if (err)
                    return Memory_leak;

                post->address = post_address;
                *flag = 1;
            }
            err = read_init_mail(mail, address, &city, &street, &house, &building, &apartment, &post_index, &weight, &id, &creation_date, &delivery_date);
            if (err)
                return err;
            err = add_mail(&mail, post);
            if (err)    
                return err;
            break;

        case REMOVE:
            err = read_init_mail(mail, address, &city, &street, &house, &building, &apartment, &post_index, &weight, &id, &creation_date, &delivery_date);
            if (err)
                return err;
            err = remove_mail(&mail, post);
            if (err)
                return err;
            break;
        case PRINT:
            sort_mails(post);
            print_mails(post);
            break;
        default:
            return Invalid_input;
    }
    return 0;
}

int dialog_manager() {
    Post* post;
    int err = init_post(&post, NULL, NULL, 0, 0);
    if (err)
        return Memory_leak;
    int flag = 0;
    while (1) {
        print_menu();
        int choice;
        err = read_choice(&choice);
        if (err)
            return err;
        err = handle_choice(choice, &flag, post);
        if (err)
            return err;
    }
}
