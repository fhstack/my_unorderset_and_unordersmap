#pragma once
#include <cstdio>
#include "HashTable.h"


template <class K,class V>
class my_unorder_map
{
public:
    struct mapKeyOfValue
    {
        K operator() (const pair<K,V>& kv)
        {
            return kv.first;
        }
    };
    typedef typename  HASH_BUCKET::HashTable<K,pair<K,V>,mapKeyOfValue,Hash_Func<K,mapKeyOfValue> > HT;
    typedef typename  HASH_BUCKET::HashTable<K,pair<K,V>,mapKeyOfValue,Hash_Func<K,mapKeyOfValue> >::iterator _iterator;
    typedef HASH_BUCKET::HashNode<pair<K,V> > Node;
    _iterator Begin()
    {
    size_t index = 0;
    Node* node = (ht.Get_ht())[index];
    if(node)
        return _iterator(node);
    else
    {
        while(index < (ht.Get_ht()).size())
        {
            ++index;
            node = (ht.Get_ht())[index];
            if(node)
            return _iterator(node);
        }
        return _iterator(NULL);
    }
    }
    _iterator End()
    {
    return _iterator(NULL);
    }
    pair<_iterator,bool> Find(const K& key)
    {
        return ht.Find(key);
    }
    pair<_iterator,bool> Insert(const pair<K,V>& v)
    {
        return ht.Insert(v);
    }
    bool remove(const K& key)
    {
        return ht.remove(key);
    }
protected:
    HT ht;
};
void TestUnorderMap()
{
    int a[] = {89,99,65,45,12,110,100,111};
    my_unorder_map<int,int> map;
    my_unorder_map<int,int>::_iterator it;
    for(size_t i = 0; i< sizeof(a)/sizeof(a[0]); i++)
    {
       // cout<<"insert"<<"("<<(a[i]<<,o)<<"success?"<<set.Insert(a[i]).second<<endl;
       printf("insert <%d,%lu> success? %d\n",a[i],i,map.Insert(make_pair(a[i],i)).second);
    }
    cout<<"test iterator"<<endl;
    it = map.Begin();
    cout<<(*it).first<<endl;
    it++;
    cout<<(*it).first<<endl;
    cout<<"test remove"<<endl;
    cout<<"remove 99 success?"<<map.remove(99)<<endl;
    cout<<"find 99?"<<map.Find(99).second<<endl;


}
