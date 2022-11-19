/**
 * lru_replacer.h
 *
 * Functionality: The buffer pool manager must maintain a LRU list to collect
 * all the pages that are unpinned and ready to be swapped. The simplest way to
 * implement LRU is a FIFO queue, but remember to dequeue or enqueue pages when
 * a page changes from unpinned to pinned, or vice-versa.
 */

#pragma once


#include <memory>
#include <unordered_map>
#include <mutex>
#include "buffer/replacer.h"

using namespace std;
namespace scudb {

template <typename T> class LRUReplacer : public Replacer<T> {
  struct Node {
    Node() {};
    Node(T val) : val(val) {};
    T val;
    shared_ptr<Node> prev;
    shared_ptr<Node> next;
  };
public:
  // do not change public interface
  LRUReplacer();

  ~LRUReplacer();
  //将元素T标记为在数据库中已经被访问,说明元素T现在是最频繁访问的，不应被选为从缓冲池中删除的牺牲品（假设存在多个元素）
  void Insert(const T &value);
  //删除与替换器跟踪的所有元素相比最近访问最少的对象,将其内容存储在T中,返回true.
  //如果Replacer中只有一个元素,那么它被认为是最近最少使用的元素.
  // 如果Replacer中没有元素,则返回false.
  bool Victim(T &value);
  //从Replacer的内部跟踪数据结构中完全删除元素T,无论它出现在LRU replacer的何处,应该删除这个元素的所有的跟踪数据,如果T存在被删除,返回true,否则返回false
  bool Erase(const T &value);
  //返回Repalcer正在跟踪的元素数
  size_t Size();

private:
  shared_ptr<Node> head;
  shared_ptr<Node> tail;
  unordered_map<T,shared_ptr<Node>> map;
  mutable mutex latch;
  // add your member variables here
};

} // namespace scudb
