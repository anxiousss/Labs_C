#include "solution.h"

void print_menu() {
    printf("1 - HELP\n");
    printf("2 - ADD\n");
    printf("3 - REMOVE\n");
    printf("4 - PRINT\n");
    printf("5 - SEARCH BY ID\n");
    printf("6 - PRINT DELIVERED MAILS\n");
    printf("7 - PRINT EXPIRED MAILS\n");
    printf("8 - EXIT\n");
}

void print_help() {
    printf("2/ADD - Add a new mail to post.\n");
    printf("3/REMOVE - Remove a specific mail from post.\n");
    printf("4/PRINT - Print a content of post.\n");
    printf("5/SEARCH BY ID - Search a mail in post\n");
    printf("6/PRINT DELIVERED MAILS - Print mails which were delivered\n");
    printf("7/PRINT EXPIRED MAILS - Print mails which were expired\n");
    printf("8/EXIT\n");
}

int read_choice(int* choice) {
    char c;
    scanf("%d%c", choice, &c);
    if (*choice < 1 || *choice > 8)
        return Invalid_input;
    return 0;
}

void print_mail_info(const Mail *mail) {
    printf("ID: %s\n", mail->id.mas);
    printf("Address: %s, %s, house %d, building %s, apt %d, postal code %s\n",
           mail->address.city.mas,
           mail->address.street.mas,
           mail->address.house,
           mail->address.building.mas,
           mail->address.apartment,
           mail->address.post_index.mas);
    printf("Weight: %.2f\n", mail->weight);
    printf("Creation Time: %s\n", mail->creation_date.mas);
    printf("Delivery Time: %s\n", mail->delivery_date.mas);
    printf("\n");
}

void print_mails(Post* post) {
    if (post == NULL || post->mails == NULL) {
        printf("Invalid input\n");
        return;
    }

    if (post->length == 0)
    {
        printf("No mails yet...\n");
        return;
    }

    printf("LIST OF MAILS:\n");
    for (int i = 0; i < post->length; i++) {
        print_mail_info(post->mails[i]);
    }
}


int handle_choice(int choice, int* flag, Post* post) {

    int err;
    Mail* tmp;
    int index = 0;
    switch (choice) {
        case HELP:
            print_help();
            return 0;
        case ADD:
            if (*flag == 0) {
                printf("Address of post\n");
                err = read_address(post->address);
                if (err)
                    return err;
                *flag = 1;
            }
            printf("Address of receiver\n");
            tmp = (Mail*)(malloc(sizeof(Mail)));
            if (!tmp)
                return Memory_leak;
            err = read_mail(tmp, post);
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
            err = read_mail(tmp, post);
            if (err)
                return err;
            err = remove_mail(tmp, post);
            if (err)
                return err;
            return 0;
        case PRINT:
            sort_mails_by_index(post);
            print_mails(post);
            return 0;
        case SEARCH:
            String id;
            printf("Input id: ");
            err = read_id(&id);
            if (err)
                return err;

            err = search_mail(&id, post, &index);
            if (err) {
                printf("Not found\n");
                return 0;
            }
            print_mail_info(post->mails[index]);
            printf("Found\n");
            return 0;

        case SEARCH_DELIVERED:
            sort_mails_by_time(post);
            print_delivered_mails(post);
            return 0;

        case SEARCH_EXPIRED:
            sort_mails_by_time(post);
            print_expired_mails(post);
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
    while (1) {
        print_menu();
        if (flag == -1) {
            delete_post(post);
            return 0;
        }
        int choice;
        err = read_choice(&choice);
        if (err)
            return err;
        err = handle_choice(choice, &flag, post);
        if (err)
            return err;
    }
}
