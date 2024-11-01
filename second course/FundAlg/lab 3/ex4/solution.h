
#ifndef EX3_4_SOLUTION_H
#define EX3_4_SOLUTION_H

#include "string.h"


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

int init_address(Address** address, String* city, String* street, unsigned int house, String* building, unsigned int apartment, String* post_index);

int delete_address(Address* address);

int init_mail(Mail** mail, Address* address, float weight, String* id, String* creation_date, String* delivery_date);

int delete_mail(Mail* mail);

int init_post(Post** post, Address* address, Mail** mails, int length, int capacity);

int delete_post(Post* post);

int resize_post(Post* post, int size);

int add_post(Mail** mail, Post* post);

int search_mail(Mail* mail, Post* post, int* index);

int cmp1_mail(const void* a, const void* b);

int cmp2_mail(const void* a, const void* b);

void sort_mails(Post* post);

int search_all(Post* post);

int is_expired(Mail* mail);

int search_expired(Post* post);

#endif //EX3_4_SOLUTION_H
