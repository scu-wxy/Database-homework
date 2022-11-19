/**
 * LRU implementation
 */
#include "buffer/lru_replacer.h"
#include "page/page.h"

namespace scudb {

template <typename T> LRUReplacer<T>::LRUReplacer() {
  head = make_shared<Node>();
  tail = make_shared<Node>();
  head->next = tail;
  tail->prev = head;
}

template <typename T> LRUReplacer<T>::~LRUReplacer() {}

/*
 * Insert value into LRU
 * 将元素T标记为在数据库中已经被访问,说明元素T现在是最频繁访问的，不应被选为从缓冲池中删除的牺牲品（假设存在多个元素）
 */
template <typename T> void LRUReplacer<T>::Insert(const T &value) {
  lock_guard<mutex> lck(latch);
  shared_ptr<Node> cur;
  if (map.find(value) != map.end()) {
    cur = map[value];
    shared_ptr<Node> prev = cur->prev;
    shared_ptr<Node> succ = cur->next;
    prev->next = succ;
    succ->prev = prev;
  } else {
    cur = make_shared<Node>(value);
  }
  shared_ptr<Node> fir = head->next;
  cur->next = fir;
  fir->prev = cur;
  cur->prev = head;
  head->next = cur;
  map[value] = cur;
  return;
}

/* If LRU is non-empty, pop the head member from LRU to argument "value", and
 * return true. If LRU is empty, return false
 * 删除与替换器跟踪的所有元素相比最近访问最少的对象,将其内容存储在T中,返回true.
 * 如果Replacer中只有一个元素,那么它被认为是最近最少使用的元素.
 * 如果Replacer中没有元素,则返回false.
 */
template <typename T> bool LRUReplacer<T>::Victim(T &value) {
  lock_guard<mutex> lck(latch);
  if (map.empty()) {
    return false;
  }
  shared_ptr<Node> last = tail->prev;
  tail->prev = last->prev;
  last->prev->next = tail;
  value = last->val;
  map.erase(last->val);
  return true;
}

/*
 * Remove value from LRU. If removal is successful, return true, otherwise
 * return false
 * 从Replacer的内部跟踪数据结构中完全删除元素T,无论它出现在LRU replacer的何处,应该删除这个元素的所有的跟踪数据,如果T存在被删除,返回true,否则返回false
 */
template <typename T> bool LRUReplacer<T>::Erase(const T &value) {
  lock_guard<mutex> lck(latch);
  if (map.find(value) != map.end()) {
    shared_ptr<Node> cur = map[value];
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
  }
  return map.erase(value);
}
 //返回Repalcer正在跟踪的元素数
template <typename T> size_t LRUReplacer<T>::Size() {
  lock_guard<mutex> lck(latch);
  return map.size();
}

template class LRUReplacer<Page *>;
// test only
template class LRUReplacer<int>;

} // namespace scudb
