#include "solution.h"

int parse_time(String* src, Time* time) {
    int day, month, year, hour, minute, second;
    if (sscanf(src->mas, "%d:%d:%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second) != 6) {
        return 1;
    }

    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 0 ||
        hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 1;
    }

    time->day = day;
    time->month = month;
    time->year = year;
    time->hour = hour;
    time->minute = minute;
    time->second = second;

    return 0;
}


int is_leap_year(int year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int days_in_month(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year)) {
        return 29;
    }
    return days[month - 1];
}

    long long seconds_since_epoch(Time* t) {
    long long seconds = 0;

    for (int y = 1970; y < t->year; y++) {
        seconds += is_leap_year(y) ? 366 * 86400 : 365 * 86400;
    }

    for (int m = 1; m < t->month; m++) {
        seconds += days_in_month(m, t->year) * 86400;
    }

    seconds += (t->day - 1) * 86400;

    seconds += t->hour * 3600;

    seconds += t->minute * 60;

    seconds += t->second;

    return seconds;
}


int read_line(char **result) {
    int buffer_size = 16;
    int length = 0;
    char *buffer = malloc(buffer_size);

    if (!buffer) {
        return Memory_leak;
    }

    int ch;
    while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
        if (length + 1 >= buffer_size) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);

            if (!new_buffer) {
                free(buffer);
                return Memory_leak;
            }

            buffer = new_buffer;
        }

        buffer[length++] = ch;
    }

    buffer[length] = '\0';

    *result = buffer;

    if (ch != '\n' && ch != EOF) {
        while ((ch = fgetc(stdin)) != '\n' && ch != EOF);
    }

    return 0;
}


int delete_address(Address* address) {
    delete_string(&(address->city));
    delete_string(&(address->street));
    delete_string(&(address->building));
    delete_string(&(address->post_index));
    free(address);
    return 0;
}


int delete_mail(Mail* mail) {
    delete_string(&mail->id);
    delete_string(&mail->creation_date);
    delete_string(&mail->delivery_date);
    free(mail);
    return 0;
}

int init_post(Post** post, int length, int capacity) {
    (*post) = (Post*)(malloc(sizeof(Post)));
    if (!*post)
        return Memory_leak;

    (*post)->address = (Address*)(malloc(sizeof(Address)));
    if (!(*post)->address) {
        return Memory_leak;
    }

    (*post)->length = length;
    (*post)->capacity = capacity;
    (*post)->mails = (Mail**)(malloc(sizeof(Mail*) * capacity));
    if (!(*post)->mails) {
        free((*post)->address);
        return Memory_leak;
    }

    return 0;
}

int delete_post(Post* post) {
    delete_address(post->address);
    for (int i = 0; i < post->capacity; ++i) {
        delete_mail(post->mails[i]);
    }
    free(post);
    return 0;
}

int resize_post(Post* post, int size) {
    Mail*** pMail = &(post->mails);
    Mail** tmp = (Mail**)(realloc(*pMail, size * sizeof(Mail*)));
    if (!tmp) {
        delete_post(post);
        return Memory_leak;
    }
    *pMail = tmp;
    return 0;
}

int add_mail(Mail** mail, Post* post) {
    if (post->capacity == post->length) {
        post->length *= 2;
        int err = resize_post(post, post->length);
        if (err)
            return err;
    }
    post->mails[post->capacity] = *mail;
    post->capacity++;
    return 0;
}

int remove_mail(Mail** mail, Post* post) {
    if (post == NULL || mail == NULL || post->mails == NULL) {
        return -1;
    }

    int found = 0;
    int i;

    for (i = 0; i < post->capacity; i++) {
        if (post->mails[i] == *mail) {
            found = 1;
            break;
        }
    }

    if (!found) {
        return -1;
    }

    free(post->mails[i]);

    for (int j = i; j < post->capacity - 1; j++) {
        post->mails[j] = post->mails[j + 1];
    }

    post->capacity--;

    return 0;
}

int search_mail(Mail* mail, Post* post, int* index) {
    for (int i = 0; i < post->capacity; ++i) {
        if (cmp1_mail(mail, post->mails[i]) == 0) {
            *index = i;
            return 0;
        }
    }
    return 1;
}

int cmp1_mail(const void* a, const void* b) {
    Mail* mail1 = *(Mail**)a;
    Mail* mail2 = *(Mail**)b;
    int res = cmp_string(&mail1->address.post_index, &mail2->address.post_index);
    if (res != 0) {
        return res;
    }
    return cmp_string(&mail1->id, &mail2->id);
}

int cmp2_mail(const void* a, const void* b) {
    Mail* mail1 = *(Mail**)a;
    Mail* mail2 = *(Mail**)b;
    String time1 = (String)mail1->creation_date;
    String time2 = (String)mail2->creation_date;

    Time t1, t2;
    if (!parse_time(&time1, &t1) || !parse_time(&time2, &t2)) {
        return 0;
    }

    if (t1.year != t2.year) return t1.year - t2.year;

    if (t1.month != t2.month) return t1.month - t2.month;

    if (t1.day != t2.day) return t1.day - t2.day;

    if (t1.hour != t2.hour) return t1.hour - t2.hour;

    if (t1.minute != t2.minute) return t1.minute - t2.minute;

    return t1.second - t2.second;
}

void sort_mails(Post* post) {
    qsort(post->mails, post->capacity, sizeof(Mail*), cmp2_mail);
}


int is_expired(Mail* mail) {
    Time t;
    parse_time(&mail->delivery_date, &t);
    if (time(NULL) > seconds_since_epoch(&t)) {
        return 1;
    }
    return 0;
}

int read_city(String *city) {
    char *tmp = NULL;
    int error_code = 0;

    printf("CITY: ");
    error_code = read_line(&tmp);

    if (error_code == 0)
    {
        error_code = init_string(city, tmp);
    }

    return error_code;
}


int read_street(String *street)
{
    char *tmp = NULL;
    int error_code = 0;

    printf("Input street: ");
    error_code = read_line(&tmp);

    if (error_code == 0)
    {
        error_code = init_string(street, tmp);
    }

    return error_code;
}

int read_house_number(unsigned int *house_number)
{
    int error_code = 0;
    char end;

    printf("Number of house: ");
    if (scanf("%u%c", house_number, &end) != 2)
    {
        error_code = Invalid_input;
    }

    return error_code;
}

int read_building(String *building)
{
    char *tmp = NULL;
    int error_code = 0;

    printf("Building: ");
    error_code = read_line(&tmp);

    if (error_code == 0)
    {
        error_code = init_string(building, tmp);
    }

    return error_code;
}

int read_apartment_number(unsigned int *apartment_number)
{
    int error_code = 0;
    char end;

    printf("Apartment number: ");
    if (scanf("%d%c", apartment_number, &end) != 2)
    {
        error_code = Invalid_input;
    }

    return error_code;
}

int read_postal_code(String *postal_code)
{
    char *tmp = NULL;
    int error_code = 0;

    printf("Post Index 6 symbols: ");
    error_code = read_line(&tmp);

    if (error_code == 0)
    {
        if (strlen(tmp) > 6)
        {
            error_code = Invalid_input;
        }
    }

    if (error_code == 0)
    {
        error_code = init_string(postal_code, tmp);
    }

    return error_code;
}


int read_address(Address *address) {
    int error_code = 0;

    printf("Address of receiver\n");
    error_code = read_city(&address->city);

    if (error_code == 0)
    {
        error_code = read_street(&address->street);
    }

    if (error_code == 0)
    {
        error_code = read_house_number(&address->house);
    }

    if (error_code == 0)
    {
        error_code = read_building(&address->building);
    }

    if (error_code == 0)
    {
        error_code = read_apartment_number(&address->apartment);
    }

    if (error_code == 0)
    {
        error_code = read_postal_code(&address->post_index);
    }

    return error_code;
}


int read_weight(float *weight)
{
    int error_code = 0;
    char end;

    printf("Введите вес посылки: ");
    if (scanf("%f%c", weight, &end) != 2)
    {
        error_code = Invalid_input;
    }

    if (error_code == 0)
    {
        if (*weight < 0)
        {
            error_code = Invalid_input;
        }
    }

    return error_code;
}

int read_delivery_time(String *delivery_time)
{
    char *tmp = NULL;
    int error_code = 0;

    printf("Введите дату доставки: ");
    error_code = read_line(&tmp);

    if (error_code == 0)
    {
        error_code = init_string(delivery_time, tmp);
    }

    return error_code;
}

int read_mail(Mail *mail)
{
    int error_code = 0;

    error_code = read_address(&mail->address);

    printf("Ввод информации о посылке\n");
    if (error_code == 0)
    {
        error_code = read_weight(&mail->weight);
    }

    if (error_code == 0)
    {
        error_code = read_delivery_time(&mail->delivery_date);
    }

    return error_code;
}
