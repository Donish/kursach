#include "message_queue.h"

message_queue::message_queue()
{
    if((_key = ftok(SERVER_KEY_PATHNAME, PROJECTD_ID)) == -1)
    {
        perror("ftok error");
        exit(1);
    }
    if((_msg_queue_id = msgget(_key, IPC_CREAT | 0660)) == -1)
    {
        perror("msgget error");
        exit(1);
    }
    _message._message_text._qid = _msg_queue_id;
    _message._message_type = 1;
}

void message_queue::send_message(const std::string &str)
{
    strcpy(_message._message_text._buff, str.c_str());
    if(msgsnd(_msg_queue_id, &_message, sizeof(struct message_text), 0) == -1)
    {
        throw std::invalid_argument("msgsnd error");
    }
}

void message_queue::receive_message(std::string *&str)
{
    if(msgrcv(_msg_queue_id, &_message, sizeof(struct message_text), 0, 0) == -1)
    {
        throw std::invalid_argument("msgrcv error");
    }

    *str = _message._message_text._buff;
}