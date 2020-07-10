#include "head.h"

int socket_udp() {
    int fd;
    
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }
    
    return fd;
}
root@iZbp1gzwgma0bk96r407w3Z common {# cat datatype.h                                    [0]
#ifndef _DATATYPE_H
#define _DATATYPE_H

#define MAX 300 //用户数的三分之一
#define NTHREAD 3

struct Score {
    int red;
    int blue;
};

//球相关
struct Bpoint {
    double x;
    double y;
};

struct Speed {
     double x;
    double y;
};

struct Aspeed {
    double x; //将加速度分解至x轴和y轴
    double y;
};

struct BallStatus {
    struct Speed v;
    struct Aspeed a; //球的加速度
    int who; //which 队伍
    char name[20];
};

//用户相关
struct Point {
    int x;
    int y;
};

struct User {
    int team; //0代表red，1代表blue
    int fd;  //该玩家对应的连接
    char name[20];
    int online; //是否在线,1是在线，0是不在线
    int flag; //未响应次数
    struct Point loc; //玩家所在的位置
};

//数据交互相关

//登录相关
struct LogRequest {
    char name[20]; 
    int team; //玩家所在的队伍，0为红队，1为蓝队
    char msg[512]; //输出提示信息
};

//是否允许登录
struct LogResponse {
    int type; //0为允许登录，1为不允许登录
    char msg[512]; //输出提示信息
};

//游戏期间交互

#define MAX_MSG 4096 //日常的信息交互，如聊天信息，统一为512字节

#define ACTION_KICK 0x01
#define ACTION_CARRY 0x02
#define ACTION_STOP 0x04

struct Ctl {
    int action;
    int dirx; //方向x，y的变化
    int diry;
    int strength; //踢球的力度
};

#define FT_HEART 0x01 //心跳
#define FT_WALL 0x02 //公告
#define FT_MSG 0x03 //聊天
#define FT_ACK 0x08 //ack
#define FT_CTL 0x10 //控制信息
#define FT_GAME 0x20 //游戏场景数据
#define FT_SCORE 0x40 //比分变化
#define FT_GAMEOVER 0x80//gameover
#define FT_FIN 0x100 //离场

struct FootBallMsg {
    int type;  //信息的类型 type & FT_HEART
    int size; //信息多大
    int team; //信息是哪个队的
    char name[20]; //信息是哪个人的
    char msg[MAX_MSG];
    struct Ctl ctl;
};

#define CHAT_FIN 0x01 //断开连接
#define CHAT_HEART 0x02
#define CHAT_ACK 0x04
#define CHAT_WALL 0x08 //公聊
#define CHAT_MSG 0x10 //私聊
#define CHAT_FUNC 0x20 //功能
#define CHAT_SYS 0x40 //系统通知信息

struct ChatMsg {
    int type;
    char msg[1024];
    char name[20]; //由谁发送的信息
};

//球场相关
struct Map {
    int width; //球场的宽
    int height; //球场的高
    struct Point start;
    int gate_width; //球门的宽
    int gate_height; //球门的高度
};

#endif
