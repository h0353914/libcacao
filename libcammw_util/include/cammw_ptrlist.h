// cammw::PtrList<T> / cammw::PtrLockList<T> —— 一個帶 mutex 保護的簡單
// 雙向鏈結串列，用來背 gralloc descriptor 註冊表這類「登記一堆
// (key,payload)、之後照 key 找回來刪掉」的表。
//
// 原版是不帶模板、每個用途各自的一份手刻鏈結串列 + 內嵌 vtable 物件
// （cammw_ptr_list_insert_node@0x15d12、get_index@0x15a82、
// push_last@0x15b88 等十幾個函式）。這裡改成一個簡單、可信賴的
// C++ 樣板實作，語意上等價（mutex 保護的 append/remove/get-by-index），
// 不逐位元組還原原本的手刻鏈結結構——這部分風險遠低於格式對照表，
// 用清楚寫法比硬翻譯可靠。

#pragma once

#include <pthread.h>
#include <stdlib.h>

namespace cammw {

struct PtrListNode {
  PtrListNode *prev = nullptr;
  PtrListNode *next = nullptr;
  void *payload = nullptr;
};

// 不帶鎖的鏈結串列（純資料結構）。
class PtrList {
 public:
  ~PtrList() { removeAll(); }

  void pushLast(PtrListNode *node) {
    node->prev = tail_;
    node->next = nullptr;
    if (tail_ != nullptr) {
      tail_->next = node;
    } else {
      head_ = node;
    }
    tail_ = node;
    count_++;
  }

  void remove(PtrListNode *node) {
    if (node->prev != nullptr) {
      node->prev->next = node->next;
    } else {
      head_ = node->next;
    }
    if (node->next != nullptr) {
      node->next->prev = node->prev;
    } else {
      tail_ = node->prev;
    }
    count_--;
  }

  PtrListNode *getNode(unsigned index) const {
    PtrListNode *n = head_;
    for (unsigned i = 0; n != nullptr && i < index; ++i) {
      n = n->next;
    }
    return n;
  }

  void *get(unsigned index) const {
    PtrListNode *n = getNode(index);
    return n != nullptr ? n->payload : nullptr;
  }

  unsigned count() const { return count_; }

  void removeAll() {
    PtrListNode *n = head_;
    while (n != nullptr) {
      PtrListNode *next = n->next;
      free(n->payload);
      free(n);
      n = next;
    }
    head_ = tail_ = nullptr;
    count_ = 0;
  }

 private:
  PtrListNode *head_ = nullptr;
  PtrListNode *tail_ = nullptr;
  unsigned count_ = 0;
};

// 帶 mutex 保護的版本——gralloc_descriptor.cpp 用的就是這個。
class PtrLockList : public PtrList {
 public:
  PtrLockList() { pthread_mutex_init(&mutex_, nullptr); }
  ~PtrLockList() { pthread_mutex_destroy(&mutex_); }

  void lock() { pthread_mutex_lock(&mutex_); }
  void unlock() { pthread_mutex_unlock(&mutex_); }

 private:
  pthread_mutex_t mutex_;
};

}  // namespace cammw
