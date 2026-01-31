#ifndef MessageQueue_h
#define MessageQueue_h

#include <Arduino.h>
#include <cppQueue.h>

typedef struct msgRec {
  String *message;
} Record;

class MessageQueue {
  public:
    MessageQueue();
    bool push(Record *record);
    Record *getCurrent();

  private:
    cppQueue *queue;
    uint32_t updatedAt;
};

#endif
