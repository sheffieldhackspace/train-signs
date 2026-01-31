#include <Arduino.h>
#include <cppQueue.h>

#include "MessageQueue.h"

MessageQueue::MessageQueue() {
  this->queue = new cppQueue(sizeof(Record *), 10, FIFO, true);
}

bool MessageQueue::push(Record *record) {
  updatedAt = millis();
  return queue->push(&record);
}

Record *MessageQueue::getCurrent() {
  int index = (queue->getCount() - 1) - ((millis() - updatedAt) / 5000) % queue->getCount();

  Record *record;
  queue->peekIdx(&record, index);

  return record;
}

