#pragma once
#include "HashTable.h"


static size_t BKDRHash(const char*str)
{
	unsigned int seed= 131;// 31 131 1313 13131 131313
	unsigned int hash= 0;
	while(*str)
	{
	 hash=hash*seed+ (*str++);
	}
	return(hash& 0x7FFFFFFF);
}


/*template<class K,class V,class KeyOfValue>
struct HashFunc
{
    size_t operator() (const V& v,size_t size)
    {
    KeyOfValue kov;
    return kov(v)%size;
    }
};*/


template <class K>
class my_unorder_set
{
public:
struct setKeyOfValue;
typedef typename HASH_BUCKET::HashTable<K,K,setKeyOfValue,Hash_Func<K,setKeyOfValue> > HT;
typedef typename HASH_BUCKET::HashTable<K,K,setKeyOfValue,Hash_Func<K,setKeyOfValue> >::iterator _iterator; 
typedef HASH_BUCKET::HashNode<K> Node;
struct setKeyOfValue
{
    size_t operator() (const K& key)
    {
    return key;
    }
};
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
    pair<_iterator,bool> Insert(const K& key)
    {
        return ht.Insert(key);
    }
    bool remove(const K& v)
    {
        return ht.remove(v);
    }
protected:
    HT ht;
};
void TestUnorderSet()
{
    int a[] = {89,99,65,45,12,110,100,111};
    my_unorder_set<int> set;
    my_unorder_set<int>::_iterator it;
    for(size_t i = 0; i< sizeof(a)/sizeof(a[0]); i++)
    {
        cout<<"insert"<<a[i]<<"success?"<<set.Insert(a[i]).second<<endl;
    }
    cout<<"test iterator"<<endl;
    it = set.Begin();
    cout<<*it<<endl;
    it++;
    cout<<*it<<endl;
    cout<<"test remove"<<endl;
    cout<<"remove 99 success?"<<set.remove(99)<<endl;
    cout<<"find 99?"<<set.Find(99).second<<endl;


}
