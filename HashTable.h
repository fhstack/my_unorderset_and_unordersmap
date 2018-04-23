#pragma once
#include <iostream>
using namespace std;
#include <vector>

enum State
{
	EMPTY,
	EXIST,
	DELETE
};
namespace HASH_
{
template <class K,class V>
struct HashNode
{
	K _key;
	V _value;
	State _state;
	HashNode(const K& key = K(),const V& value = V())
		:_key(key)
		,_value(value)
		,_state(EMPTY)
		{}
};
template<class K>
struct HashFunc
{
size_t operator() (const K& key)
{
	return key;
}
};
template <class K,class V,class HashF = HashFunc<K> >
class HashTable
{
typedef HashNode<K,V> Node;

public:
	HashTable()
	{
		_v.resize(10);
		_size = 0;
	}
	bool Insert(const K& key,const V& value)
	{
		HashF _HashF;
		CheckLoad();
        int i = 1;
		size_t index = _HashF(key) % _v.size();
		while(1)
		{
			if(_v[index]._state != EXIST)
			{
				_v[index]._key = key;
				_v[index]._value = value;
				_v[index]._state = EXIST;
				_size++;
				return true;
			}
			else
			{
				index = index + i*i;
				if(index >= _v.size())
					index = 0;
			    i++;	
			}
		}
	}
    Node* Find(const K& key)
	{
	HashF _HashF;
	int find_lenth = 0;
	int i = 1;
    size_t index = _HashF(key)%_v.size();
	while(1)
	{
		if(_v[index]._state == EXIST && _v[index]._key ==key )
		{
			return &_v[index];
		}
		else
		{
			index = index + i*i;
            find_lenth += i*i;
            if(find_lenth >=  _v.size())
            return NULL;
			if(index >= _v.size())
            {
				index = 0;
				++find_lenth;
			}
			
		}
	}
	}
	bool erase(const K& key)
	{
		Node*  find = Find(key);
		if(find != NULL)
		{
			find->_state =  DELETE;
			return true;
		}
		else
			return false;
	}

	void CheckLoad()
	{
        if((_size*10)/_v.size() >= 7)
		{
			HashTable newht;
			newht._v.resize(2*_v.size());
			for(size_t index = 0;index < _v.size();index++)
			{
				if(_v[index]._state == EXIST)
				{
					newht.Insert(_v[index]._key,_v[index]._value);
				}
			}
			_v.swap(newht._v);
		}
	}
protected:
	vector<Node> _v;
	size_t _size;
};
}


template <class K,class V>
struct set_KeyOfValue
{
const K& operator() (const V& v)
{
    return v;
}
};


template<class K,class KeyOfValue>
struct Hash_Func
{
    size_t operator() (const K& key,size_t size)
    {
    return key%size;
    }
};


namespace HASH_BUCKET
{
template<class V>
struct HashNode
{
    V _v;
    HashNode<V>* _next;
    HashNode(const V& v = V())
    :_v(v)
    ,_next(NULL)
    {}
};


template<class K,class V,class KeyOfValue,class __HashFunc >
struct Iterator;


template<class K,class V,class KeyOfValue,class __HashFunc >
class HashTable
{
public:
    friend struct Iterator<K,V,KeyOfValue,__HashFunc>;
    typedef HashNode<V> Node;
    typedef Iterator<K,V,KeyOfValue,__HashFunc> iterator;
    __HashFunc HashFunc;
    KeyOfValue kov;
    HashTable()
    {
        _size = 0;
    }
    

    pair<iterator,bool>  Insert(const V& v)
    {
        if(Find(kov(v)).second)
        {
        return make_pair(iterator(NULL,this),false);
        }
        CheckLoad();
        size_t index = HashFunc(kov(v),_ht.size());
        Node* node = new Node(v);
        node->_next = _ht[index];
        _ht[index] = node;
        ++_size;
        return make_pair(iterator(node,this),true);
    }
    
    
    pair<iterator,bool> Find(const K& key)
    {
        if(_ht.size() == 0)
        {
            return make_pair(iterator(NULL,this),false);
        }
        size_t index = HashFunc(key,_ht.size());
        Node* cur =  _ht[index];
        while(cur)
        {
            if(kov(cur->_v) == key)
            {
                return make_pair(iterator(cur,this),true);
            }
            cur = cur->_next;
        }
        return make_pair(iterator(NULL,this),false);
    }

    
    bool remove(const K& key)
    {
        if(Find(key).second)
        {
          //  cout<<"start deleting"<<endl;
            size_t index = HashFunc(key,_ht.size());
            if(key == kov(_ht[index]->_v))
            {
               Node*  del = _ht[index];
               _ht[index] = _ht[index]->_next;
               delete del;
               del = NULL;
               return true;
            }
            Node* cur = _ht[index];
            Node* prev = NULL;
            while(cur)
            {
                if(kov(cur->_v) == key)
                {
                  if(cur->_next == NULL)
                  {   
                  prev->_next = NULL;
                  }
                  else
                  {
                    prev->_next = cur->_next;
                  }
                  delete cur;
                  cur = NULL;
                  return true;
                 // cout<<"deleted"<<endl;
                }
                prev = cur;
                cur = cur->_next;
            }
            
        }
        else
            return false;
    }
    
    void CheckLoad()
    {
        if(_ht.size() == 0)
            _ht.resize(10);
        if(_size == _ht.size())
        {
            HashTable newht;
            newht._ht.resize(2*_size);
            for(size_t i=0;i<_ht.size();i++)
            {
                Node*  cur = _ht[i];
                while(cur)
                {
                    newht.Insert(cur->_v);
                    cur = cur->_next;
                }
            }
            _ht.swap(newht._ht);
        }
    }

    vector<Node*> Get_ht()
    {
        return _ht;
    }
private:
    vector<Node*> _ht;
    size_t _size;
};



template <class K,class V,class KeyOfValue,class __HashFunc>
struct Iterator
{
    typedef HashNode<V>    Node;
    typedef HashTable<K,V,KeyOfValue,__HashFunc>  HT;
    typedef Iterator<K,V,KeyOfValue,__HashFunc>   Self;
    Node*  _node;
    HT* ht;
    Self& operator++ ()
    {
    Node* next = _node->_next;
    if(next != NULL)
    {
        _node = next;
        return *this;
    }
    else
    {
        KeyOfValue kov;
        size_t index =ht->HashFunc(kov(_node->_v),(ht->_ht).size());
        ++index;
        while(index < (ht->_ht).size())
        {
        if((ht->_ht)[index])
        {
        _node = (ht->_ht)[index];
        return *this;
        }
        ++index;
        }
    }
    return *this;
    }
    Self operator++(int)
    {
        Self tmp(*this);
        ++(*this);
        return tmp;
    }

    V& operator* ()
    {
        return _node->_v;
    }
    V& operator-> ()
    {
        return &_node->_v;
    }
    bool operator == (Self it)
    {
        if(_node == it._node)
        return true;
        else
        return false;
    }
    bool operator != (Self it)
    {
        return !(*this == it);
    }
    Iterator(Node* node = NULL,HT* ht = NULL)
      :_node(node)
      ,ht(ht)
      {}
};

}
void Test()
{
	int a[] = {89,99,65,45,12,110,100,111};
    HASH_BUCKET::HashTable<int,int,set_KeyOfValue<int,int>,Hash_Func<int,set_KeyOfValue<int,int> > >  ht;
	for(size_t i = 0;i<sizeof(a)/sizeof(a[0]);i++)
	{
	    cout<<ht.Insert(a[i]).second<<endl;
	}
	cout<<"exist 65?"<<ht.Find(65).second<<endl;
	cout<<"exist 1?"<<ht.Find(1).second<<endl;
	cout<<"remove 65 success?"<<ht.remove(65)<<endl;
    cout<<"exist 65?"<<ht.Find(65).second<<endl;

}
