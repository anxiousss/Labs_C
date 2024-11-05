#include "solution.h"

void print_menu() {
    printf("1 - HELP\n2 - ADD\n3 - REMOVE\n4 - PRINT\n5 - SEARCH\n6 - EXIT\n");
}

void print_help() {
    printf("2/ADD - Add a new mail to post.\n");
    printf("3/REMOVE - Remove a specific mail from post.\n");
    printf("4/PRINT - Print a content of post.\n");
    printf("5/SEARCH - Search a mail in post\n");
    printf("6/EXIT - End of dialogue\n");
}

int read_choice(int* choice) {
    char c;
    scanf("%d%c", choice, &c);
    if (*choice < 1 || *choice > 6)
        return Invalid_input;
    return 0;
}

void print_mails(Post* post) {
    if (post == NULL || post->mails == NULL) {
        printf("Invalid input\n");
        return;
    }

    printf("LIST OF MAILS:\n");
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

    int err;
    Mail* tmp;
    switch (choice) {
        case HELP:
            print_help();
            return 0;
        case ADD:
            if (*flag == 0) {
                err = read_address(post->address);
                if (err)
                    return err;
                *flag = 1;
            }
            tmp = (Mail*)(malloc(sizeof(Mail)));
            if (!tmp)
                return Memory_leak;
            err = read_mail(tmp);
            if (err)
                return err;
            err = add_mail(tmp, post);
            if (err)
                return err;
            return 0;
        case REMOVE:
            tmp = (Mail*)(malloc(sizeof(Mail)));
            if (!tmp)
                return Memory_leak;
            err = read_mail(tmp);
            if (err)
                return err;
            err = remove_mail(&tmp, post);
            if (err)
                return err;
            return 0;
        case PRINT:
            //sort_mails(post);
            print_mails(post);
            return 0;
        case SEARCH:
            int index = 0;
            tmp = (Mail*)(malloc(sizeof(Mail)));
            if (!tmp)
                return Memory_leak;
            err = read_mail(tmp);
            if (err)
                return err;
            err = search_mail(tmp, post, &index);
            if (err) {
                printf("Not found\n");
                return 0;
            }
            printf("Found\n");
            return 0;
        case EXIT:
            printf("DIALOG IS ENDED\n");
            *flag = -1;
            return 0;
            default:
                return Invalid_input;
    }
}

int dialog_manager() {
    Post *post;
    int err = init_post(&post, 0, 2);
    if (err)
        return Memory_leak;
    int flag = 0;
    print_menu();
    while (1) {
        if (flag == -1)
            return 0;
        int choice;
        err = read_choice(&choice);
        if (err)
            return err;
        err = handle_choice(choice, &flag, post);
        if (err)
                return err;
    }
}
