#ifndef _UDP_EPOLL_H
#define _UDP_EPOLL_H
int udp_accpet();
int udp_connect();
void send_all();
void send_to();
void add_event_ptr();
void del_event();
int udp_connect();
int check_online();
int udp_accept();
int find_sub();
void add_to_sub_reactor();
#endif
