/*
 * buffer_pool_manager.h
 *
 * Functionality: The simplified Buffer Manager interface allows a client to
 * new/delete pages on disk, to read a disk page into the buffer pool and pin
 * it, also to unpin a page in the buffer pool.
 */

#pragma once
#include <list>
#include <mutex>

#include "buffer/lru_replacer.h"
#include "disk/disk_manager.h"
#include "hash/extendible_hash.h"
#include "logging/log_manager.h"
#include "page/page.h"

namespace scudb {
class BufferPoolManager {
public:
  BufferPoolManager(size_t pool_size, DiskManager *disk_manager,
                          LogManager *log_manager = nullptr);

  ~BufferPoolManager();
    //返回包含page_id内容的Page对象
    //函数先检查其内部页表，查看是否存在映射到page_id的Page.如果存在,则返回它.否则,它将从DiskManager中检索物理页.
    //函数需要选择一个Page对象来存储物理页面的内容.如果页表中有空闲帧,则函数将选择一个随机帧来使用.否则,它将使用LRUReplacer选择一个最近被用作“受害者”页面的未固定页面.
    // 如果没有空闲插槽（即,所有页面都被固定）,则返回空指针（nullptr）.如果选定的受害页面是脏的,则需要使用DiskManager将其内容写入磁盘.然后,您将使用DiskManager从磁盘读取目标物理页面.并将其内容复制到该页面对象中.
    //函数必须将页面标记为已固定,并在将其返回给调用者之前从LRUReplacer中删除其条目.
    Page *FetchPage(page_id_t page_id);
    //减少给定page_id指定的page的pin计数器.如果pin计数器为零,那么函数将把Page对象添加到LRUReplacer跟踪器中.
    //如果给定的is_dirty标志为true,则将Page标记为dirty；否则,保留Page的dirty标志.
    //如果页面表中没有给定page_id的条目,则返回false.
    bool UnpinPage(page_id_t page_id, bool is_dirty);
    //检索给定page_id指定的page对象,然后使用DiskManager将其内容写入磁盘.成功完成写入操作后,函数检索给定page_id指定的page对象,然后使用DiskManager将其内容写入磁盘.成功完成将返回true.
    //此函数不应从缓冲池中删除页面.它也不需要更新页面的LRUReplacer.如果页面表中没有给定page_id的条目,则返回false.
    bool FlushPage(page_id_t page_id);
    //在DiskManager中分配一个新的物理页面,将新页面id存储在给定的page_id中,并将新页面存储在缓冲池中.
    //在从LRUReplacer中选择受害页面并初始化页面的内部元数据（包括增加引脚数）方面,这应该具有与FetchPage（）相同的功能.
    Page *NewPage(page_id_t &page_id);
    //指示DiskManager释放由给定page_id标识的物理页.您只能删除当前未固定的页面.
    bool DeletePage(page_id_t page_id);

private:
  size_t pool_size_;
  Page *pages_;
  DiskManager *disk_manager_;
  LogManager *log_manager_;
  HashTable<page_id_t, Page *> *page_table_;
  Replacer<Page *> *replacer_;
  std::list<Page *> *free_list_;
  std::mutex latch_;
  Page *GetVictimPage();
};
} // namespace scudb
