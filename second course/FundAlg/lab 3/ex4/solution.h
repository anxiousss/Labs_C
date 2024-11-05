
#ifndef EX3_4_SOLUTION_H
#define EX3_4_SOLUTION_H

#include "string.h"


typedef enum Command {
    HELP = 1,
    ADD,
    REMOVE,
    PRINT,
    SEARCH,
    EXIT
} Command;

typedef struct Address {
    String city;
    String street;
    unsigned int house;
    String building;
    unsigned int apartment;
    String post_index;
} Address;

typedef struct Mail {
    Address address;
    float weight;
    String id;
    String creation_date;
    String delivery_date;
} Mail;

typedef struct Post {
    Address *address;
    Mail** mails;
    int length;
    int capacity;
} Post;

typedef struct Time {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
} Time;

int parse_time(String* src, Time* time);

int is_leap_year(int year);

int days_in_month(int month, int year);

long long seconds_since_epoch(Time* t);

int read_line(char **result);

int delete_address(Address* address);

int delete_mail(Mail* mail);

int init_post(Post** post, int length, int capacity);

int delete_post(Post* post);

int resize_post(Post* post, int size);

int add_mail(Mail* mail, Post* post);

int remove_mail(Mail** mail, Post* post);

int search_mail(Mail* mail, Post* post, int* index);

int cmp1_mail(const void* a, const void* b);

int cmp2_mail(const void* a, const void* b);

void sort_mails(Post* post);

void print_mails(Post* post);

int search_all(Post* post);

int is_expired(Mail* mail);

int search_expired(Post* post);

void print_menu();

int dialog_manager();

void print_help();

int read_choice(int* choice);

int handle_choice(int choice, int* flag , Post* post );

int read_city(String *city);

int read_street(String *street);

int read_house_number(unsigned int *house_number);

int read_building(String *building);

int read_apartment_number(unsigned int *apartment_number);

int read_postal_code(String *postal_code);

int read_address(Address *address);

int read_weight(float *weight);

int read_delivery_time(String *delivery_time);

int read_mail(Mail *mail);

#endif //EX3_4_SOLUTION_H
