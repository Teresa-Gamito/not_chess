#define MESSAGE_MAX_LENGHT 255U
#define HISTORY_MAX_SIZE 255U

typedef struct Message
{
    char message[MESSAGE_MAX_LENGHT];
} Message;

typedef struct History
{
    Message message[];
} History;

void history_add_message(History *history, Message message);


