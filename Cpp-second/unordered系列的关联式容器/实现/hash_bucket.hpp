#pragma once
#include <iostream>
#include <utility>
#include <vector>
template<class K, class V>
//这里现在存的就是链表结点
//这里我们使用单链表就行了
struct HashNode
{
    HashNode(const std::pair<K, V>& data = std::pair<K, V>())
        :_data(data)
        ,_next(nullptr)
    {
    }
    std::pair<K, V> _data;
    HashNode<K, V>* _next;
};
template<class K, class V>
class HashTable
{
public:
    typedef HashNode<K, V> Node;
    HashTable(size_t n = 10)
        :_size(0)
    {
        _ht.resize(n);
    }
    //插入
    bool Insert(const std::pair<K, V>& data)
    {
        //检查负载因子，超过阈值进行扩容
        CheckCapacity();
        //计算位置
        int index = data.first % _ht.size();
        //遍历单链表
        Node* cur = _ht[index];
        while(cur)
        {
            if(cur->_data.first == data.first)
            {
                return false;
            }
            cur = cur->_next;
        }
        //插入，这里用头插其实比较方便，尾插也可以
        cur = new Node(data);
        cur->_next = _ht[index];
        _ht[index] = cur;
        ++_size;
        return true;
    }
    //查找
    Node* Find(const K& key)
    {
        int index = key % _ht.size();
        Node* cur = _ht[index];
        while(cur)
        {
            if(cur->_data.first == key)
            {
                return cur;
            }
            cur = cur->_next;
        }
        return nullptr;
    }
    //删除
    bool Erase(const K& key)
    {
        int index = key % _ht.size();
        Node* cur = _ht[index];
        Node* parent = nullptr;
        while(cur)
        {
            if(cur->_data.first == key)
            {
                //删除
                if(parent == nullptr)
                {
                    _ht[index] = cur->_next;
                }
                else
                {
                    parent->_next = cur->_next;
                }
                delete cur;
                --_size;
                return true;
            }
            parent = cur;
            cur = cur->_next;
        }
        return false;
    }
    void CheckCapacity()
    {
        //这里的阈值可以设置的稍微高一些，毕竟哈希桶的冲突不会像闭散列呢样严重
        //这里我们设定为插入元素数 >= 哈希表总长度时扩容
        if(_size >= _ht.size())
        {
            //扩容
            size_t newC = _ht.size() == 0 ? 10 : 2 * _ht.size();
            std::vector<Node*> newHt;
            newHt.resize(newC);
            //搬运数据，将原哈希表中的结点连接到新哈希表上
            for(int i = 0; i < _ht.size(); i++)
            {
                Node* cur = _ht[i];
                while(cur)
                {
                    int index = cur->_data.first % newHt.size();
                    Node* next = _ht[i]->_next;
                    //头插进新表
                    cur->_next = newHt[index];
                    newHt[index] = cur;
                    cur = next;
                }
                _ht[i] = nullptr;
            }
            //_ht = newHt;   //这里会进行深拷贝，消耗很大，为了防止深拷贝我们直接交换
            std::swap(_ht, newHt);
        }
    }

private:
    //此时哈希表就相当于一个链表指针数组
    std::vector<Node*> _ht;
    size_t _size;
};