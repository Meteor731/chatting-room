#include "head.h"

extern int port;
extern int repollfd, bepollfd;
struct User *rteam, *bteam;
extern pthread_mutex_t bmutex, rmutex;

void send_all(struct ChatMsg *msg) {
    for(int i = 0; i < MAX; i++) {
        if(bteam[i].online) send(bteam[i].fd, (void *)msg, sizeof(struct ChatMsg), 0);
        if(rteam[i].online) send(rteam[i].fd, (void *)msg, sizeof(struct ChatMsg), 0);
    }
}

void send_to(char *to, struct ChatMsg *msg. int fd) {
    int flag = 0;
    for (int i = 0; i < MAX; i++) {
        if (rteam[i].online && (!strcmp(to, rteam[i].name) || !strcmp(to, rteam[i].real_name))) {
            send(rteam[i].fd, msg, sizeof(struct ChatMsg), 0);
            flag = 1;
            break;
        }
    }
    if (!flag) {
        memset(msg->msg, 0, sizeof(msg->msg));
        sprintf(msg->msg, "用户 %s不在线，或用户名错误！", to);
        msg->type = CHAT_SYS;
        send(fd, msg, sizeof(struct ChatMsg), 0);
    }
}

void add_event_ptr(int epollfd, int fd, int events, struct User *user) {
    struct epoll_event ev;
    ev.data.ptr = user;
    ev.events = events;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void del_event(epollfd, int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}

int udp_connect(struct sockaddr_in *client) {
    int sockfd;

    if ((sockfd = socket_create_udp(port)) < 0) {
        perror("socket_create_udp");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)client, sizeof(struct sockaddr)) < 0) {
        return -1;
    }

    return sockfd;
}

int check_online(struct LogRequest *request) {

    for (int i = 0; i < MAX; i++) {
        }
        if (bteam[i].online && !strcmp(request->name, rteam[i].name)) {
    }

    return 0;
}


int udp_accept(int fd, struct User *user) {
    int new_fd, ret;
    struct sockaddr_in client;
    struct LogRequest request; //用于交互
    struct LogResponse response; //用于交互
    bzero(&request, sizeof(request)); //将结构体初始化
    bzero(&response, sizeof(response));
    socklen_t len = sizeof(client);

    ret = recvfrom(fd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&client, &len);

    if (ret != sizeof(request)) {
        response.type = 1;
        strcpy(response.msg, "Login failed with Data errors!");
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        return -1;
    }

   if (check_online(&request)) {
        response.type = 1; //登录成功，返回为真
        strcpy(response.msg, "You are Already Login!");
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        return -1;
    }

    response.type = 0;
    strcpy(response.msg, "Login Success. Enjoy yourself!");
    sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);

    if (request.team) {
    }
    else {
    }

    strcpy(user->name, request.name);
    user->team = request.team;
    new_fd = udp_connect(&client);
    user->fd = new_fd;
    return new_fd;
}

//找下标
int find_sub(struct User *user) {

    for (int i = 0; i < MAX; i++) {
        if (!team[i].online) return i;
    }

    return -1;
}

//for chat
void add_to_sub_reactor(struct User *user) {
    struct User *team = (user -> team ? bteam : rteam);
    DBG(YELLO"Main Thread :"NONE"Add to sub_reactor\n");
    if(user->team)
        pthread_mutex_lock(&mutex);
    else
        pthread_mutex_lock(&mutex);

    int sub = find_sub(team);

    /*if (sub < 0) {
        fprintf(stderr, "Full Tram!\n");
        return;
    } */
    team[sub] = *user;
    team[sub].online = 1; //表示已经上线
    team[sub].flag = 10;

    if(user->team)
        pthread_mutex_unlock(&bmutex);
    else
        pthread_mutex_unlock(&rmutex);

    DBG(L_RED"sub = %d, name = %s\n", sub, team[sub],name);
    if (user->team) {
        add_event_ptr(bepollfd, team[sub].fd, EPOLLIN | EPOLLET, &team[sub]);
    }
    else {
        add_event_ptr(repollfd, team[sub].fd, EPOLLIN | EPOLLET, &team[sub]);
    }

}
