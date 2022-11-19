/*
 * extendible_hash.h : implementation of in-memory hash table using extendible
 * hashing
 *
 * Functionality: The buffer pool manager must maintain a page table to be able
 * to quickly map a PageId to its corresponding memory location; or alternately
 * report that the PageId does not match any currently-buffered page.
 */

#pragma once

#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <mutex>

#include "hash/hash_table.h"
using namespace std;


namespace scudb {


template <typename K, typename V>
class ExtendibleHash : public HashTable<K, V> {
  struct Bucket {
    Bucket(int depth) : localDepth(depth) {};
    int localDepth;
    map<K, V> kmap;
    mutex latch;
  };
public:
    // constructor
    ExtendibleHash(size_t size);
    ExtendibleHash();
    // helper function to generate hash addressing
    //对于给定的键key,返回Bucket应该存储的偏移量
    size_t HashKey(const K &key) const;
    // helper function to get global & local depth
    //返回整个hash表的当前的全局深度
    int GetGlobalDepth() const;
    //返回给定偏移的Bucket的当前的本地深度
    int GetLocalDepth(int bucket_id) const;
    //返回hash表中分配的Bucket的总数
    int GetNumBuckets() const;
    // lookup and modifier
    //对于给定的键key,检查其是否在hash表中.如果存在将指针存储到V中的相应值中并返回true,否则返回false
    bool Find(const K &key, V &value) override;
    //对于给定的键key,从hash表中移除其对应的键/值并返回true,如果hash中不存在键key,返回false
    bool Remove(const K &key) override;
    //插入键/值对到hash表在.如果键key已经存在,则用新值Vand覆盖并返回true
    void Insert(const K &key, const V &value) override;

  int getIdx(const K &key) const;

private:
  // add your own member variables here
  int globalDepth;
  size_t bucketSize;
  int bucketNum;
  vector<shared_ptr<Bucket>> buckets;
  mutable mutex latch;
};
} // namespace scudb
