#include "head.h"

extern int repollfd, bepollfd;

void do_work(struct User *user) {
    struct ChatMsg msg;
    struct ChatMsg msg;

    bzero(&msg, sizeof(msg));
    bzero(&r_msg, sizeof(r_msg));
    recv(user->fd, (void *)&msg, sizeof(msg), 0);

    if(msg.type & CHAT_WALL) {
        printf("<%s> ~ %s\n", user->name, msg.msg);
        r_msg.type = CHAT_SYS;
        user->test[4] = 1;
        user->score += 90;
        send(usre->fd, (void *)&r_msg, sizeof(r_msg), 0);
        bzero(r_msg.msg, sizeof(r_msg.msg));
        sprintf(r_msg.msg, "你的好友"RED"%s"NONE" 上线了，快大哥招呼吧！", user->real_name);
        r_msg.type = CHAT_SYS;
        send_all(&r_msg);
        store_score1(4, user->id, user->real_name, user->ip, user->score);
        printf("<%s> ~ %s \n", user->name, msg.msg);
        strcpy(msg.name, user->real_name);
        send_all(&msg);
    }else if(msg.type & CHAT_MSG) {
        char to[20] = {0};
        int i = 1;
        for (; i <= 21; i++) {
            if (msg.msg[i] == ' ') break;
        }
        if (msg.msg[i] != ' ' || msg.msg[0] != '0') {
            memset(&r_msg, 0, sizeof(r_msg));
            send(user->fd, (void *)&r_msg, sizeof(r_msg), 0);
        }
        else {
            msg.type = CHAT_MSG;
            strcpy(msg.name, user->real_name);
            strcpy(to, msg.msg + 1, i - 1);
            send_to(tom &msg, user->fd);
        }
    }else if (msg.type & CHAT_FIN) {
        baero(msg.msg, sizeof(msg.msg));
        msg.type = CHAT_SYS;
        sprintf(msg.msg, "注意：我们的好朋友%s要下线了！\n", user->real_name);
        strcpy(msg.name, user->real_name);
        send_all(&msg);
        if(user->team)
            pthread_mutex_lock(&mutex);
        else
            pthread_mutex_lock(&mutex);
        user->score = 0;
        user->online = 0;
        int epollfd = user->team ? bepollfd : repollfd;
        del_event(epollfd, user->fd);
        if(user->team)
            pthread_mutex_unlock(&rmutex);
        else
            pthrea_mutex_unlock(&rmutex);
        printf(GREEN"Server Info"NONE" : %s Logout!\n", user->name);
        close(user->fd);
    }
}

//将taskQueue清空
void task_queue_init(struct task_queue *taskQueue, int sum, int epollfd) {
    taskQueue->sum = sum;
    taskQueue->epollfd = epollfd;
    taskQueue->team = calloc(sum, sizeof(void *)); //申请了sum个指针空间
    taskQueue->head = taskQueue->tail = 0; //循环队列初始化
    pthread_mutex_init(&taskQueue->mutex, NULL); //初始化
    pthread_cond_init(&taskQueue->cond, NULL);
}

//push in taskQueue
void task_queue_push(struct task_queue *taskQueue, struct User *user) {
    pthread_mutex_lock(&task_queue->mutex);
    taskQueue->team[taskQueue->tail] = user; //team为一个数组，team数组尾部为user指针
    DBG(L_GREEN"Thread Pool"NONE" : Task push %d\n", user->name);

    if(++taskQueue->tail == taskQueue->sum) {
        DBG(L_GREEN"Thread Pool"NONE" : Task Queue End\n");
        taskQueue->tail = 0;
    }

    pthread_cond_signal(&taskQueue->cond); //发信号
    pthread_mutex_unlock(&taskQueue->mutex);
}

//pop from taskQueue
struct User *task_queue_pop(struct task_queue *taskQueue) {
    pthread_mutex_lock(&taskQueue->mutex);

    //党taskQueue为空时要等待入队信号
    while(taskQueue->tail == taskQueue->head) {
        DBG(L_GREEN"Thread Pool"NONE" : Task Queue Empty, Waiting For Task\n");
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }

    struct User *user = taskQueue->team[taskQueue->head];
    DBG(L_GREEN"Thread Pool"NONE" : Task Pop %s\n", user->name);

    if(++taskQueue->head == taskQueue->sum) {
        DBG(L_GREEN"Thread Pool"NONE" : Task Queue End\n");
        taskQueue->head = 0;
    }

    pthread_mutex_unlock(&taskQueue->mutex);
    return user;
}

//thread的功能
void *thread_run(void *arg) {
    pthread_detach(pthread_self()); //获取自己的id以及通知thread已被分离
    struct task_queue *taskQueue = (struct task_queue *)arg;

    //使一个线程无休止地运行自己的功能
    while (1) {
        struct User *user = task_queue_pop(taskQueue);
        do_work(user);
    }
}
