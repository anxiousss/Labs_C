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

    // Считаем количество секунд в годах
    for (int y = 1970; y < t->year; y++) {
        seconds += is_leap_year(y) ? 366 * 86400 : 365 * 86400;
    }

    // Считаем количество секунд в месяцах
    for (int m = 1; m < t->month; m++) {
        seconds += days_in_month(m, t->year) * 86400;
    }

    // Считаем количество секунд в днях
    seconds += (t->day - 1) * 86400;

    // Считаем количество секунд в часах
    seconds += t->hour * 3600;

    // Считаем количество секунд в минутах
    seconds += t->minute * 60;

    // Считаем количество секунд
    seconds += t->second;

    return seconds;
}


int init_address(Address** address, String* city, String* street, unsigned int house, String* building, unsigned int apartment, String* post_index) {
    *address = (Address*)(malloc(sizeof(Address)));
    if (!*address)
        return Memory_leak;
    int err = copy_str(&(*address)->city, city);
    if (err) {
        free(*address);
        return Memory_leak;
    }
    err = copy_str(&(*address)->street, street);
    if (err) {
        delete_string(&(*address)->city);
        free(*address);
        return Memory_leak;
    }
    (*address)->house = house;
    err = copy_str(&(*address)->building, building);
    if (err) {
        delete_string(&(*address)->city);
        delete_string(&(*address)->street);
        free(*address);

        return Memory_leak;
    }
    (*address)->apartment = apartment;
    err = copy_str(&(*address)->post_index, post_index);
    if (err) {
        delete_string(&(*address)->city);
        delete_string(&(*address)->street);
        delete_string(&(*address)->building);
        free(*address);
        return Memory_leak;
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

int init_mail(Mail** mail, Address* address, float weight, String* id, String* creation_date, String* delivery_date) {
    *mail = (Mail*)(malloc(sizeof(Mail)));
    if (!*mail)
        return Memory_leak;
    (*mail)->address = *address;
    (*mail)->weight = weight;
    int err = copy_str(&(*mail)->id, id);
    if (err) {
        free(*mail);
        return Memory_leak;
    }
    err = copy_str(&(*mail)->creation_date, creation_date);
    if (err) {
        delete_string(&(*mail)->id);
        free(*mail);
        return Memory_leak;
    }
    err = copy_str(&(*mail)->delivery_date, delivery_date);
    if (err) {
        delete_string(&(*mail)->id);
        delete_string(&(*mail)->creation_date);
        free(*mail);
        return Memory_leak;
    }
    return 0;
}

int delete_mail(Mail* mail) {
    delete_string(&mail->id);
    delete_string(&mail->creation_date);
    delete_string(&mail->delivery_date);
    free(mail);
    return 0;
}

int init_post(Post** post, Address* address, Mail** mails, int length, int capacity) {
    (*post) = (Post*)(malloc(sizeof(Post)));
    if (!*post)
        return Memory_leak;

    (*post)->address = address;
    (*post)->mails = mails;
    (*post)->length = length;
    (*post)->capacity = capacity;
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

int add_post(Mail** mail, Post* post) {
    if (post->capacity == post->length) {
        post->length *= 2;
        int err = resize_post(post, post->length);
        if (err)
            return err;
    }
    post->mails[post->capacity] = *mail;
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

