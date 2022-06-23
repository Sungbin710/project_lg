#ifndef _LOGIN_H
#define _LOGIN_H

#define LIST_SIZE 3

// static gboolean get_cb (char *buf, char *id);
int register_user(int clnt_sock);
int login(int clnt_sock);
int request_userinfo(int clnt_sock);
int request_recent_list(int sockfd);
int request_popular_list(int sockfd);

int logout();


// gboolean set_user_state(int state);
int get_user_state();
int get_user_id();

#endif /* _LOGIN_H */