#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <iostream>
#include <string>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

class message_queue
{
private:

    struct message_text
    {
        int _qid;
        char _buff[500];
    };

    struct message
    {
        long int _message_type;
        struct message_text _message_text;
    };

private:

    int _msg_queue_id;
    struct message _message;
    key_t _key;

private:

    char* SERVER_KEY_PATHNAME = "../tmp/mqueue_server_key";
    char PROJECTD_ID = 'M';

public:

    message_queue();

    void send_message(const std::string &str);

    void receive_message(std::string &str);

};

#endif //MESSAGE_QUEUE_H