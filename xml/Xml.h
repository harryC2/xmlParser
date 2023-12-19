#pragma once
#include<string>
#include<map>
#include<list>
#define SAFEDELETE(a)  \
        if(a!=nullptr) {\
        delete a;a = nullptr; \
        }

using namespace std;

namespace yazi{
    namespace xml{

// 强制类型转换
class Value
{
private:

public:
    Value();
    Value(bool value);
    Value(int value);
    Value(double value);
    Value(const char * value);
    Value(const string & value);
    ~Value();

    Value & operator = (bool value);
    Value & operator = (int value);
    Value & operator = (double value);
    Value & operator = (const char * value);
    Value & operator = (const string & value);
    Value & operator = (const Value & value);

    bool operator == (const Value & other);
    bool operator != (const Value & other);

    operator bool();
    operator int();
    operator double();
    operator string();

    private:
    string m_value;
};

class Xml
{
private:

    // 考虑性能 拷贝；都用指针来代替
   string*  m_name{nullptr}; // 节点名城
   string*  m_text{nullptr}; // 节点内容
   std::map<string,Value>* m_attrs{nullptr}; // 属性
   std::list<Xml>* m_child{nullptr};//也可以vector ，但是删除和添加中间节点时，效率低，会操作多个内存区域

public:
    Xml&  parse(const string& strName);
    string getName() const;
    string getText() const;
    

    void setName(const string &name);
    void setText(const string& test); 

    Value getAttr(const string& key);
    void setAttr(const string& key,const Value & val);

    // xml输出
    string toString()const;

    void appendChildren(const Xml& children);

    Xml& operator[] (int index);
    Xml& operator[] (const char* name);
    Xml& operator[] (const string& name);
    Xml& operator= (const Xml& xml);
    void removeChildren(int index);
    void removeChildren(const char* name);
    void removeChildren(const string& name);

    void removeAttribute(const char* name);
    void removeAttribute(const string& name);
    void clear();

    // iterator
    typedef std::list<Xml>::iterator iterator;
    iterator begin()
    {
        return m_child->begin();
    };
    iterator end()
    {
        return m_child->begin();
    }
    iterator erase(iterator it)
    {
        it->clear();
        return m_child->erase(it);
    }
    int size() const 
    {
        return m_child->size();
    }
public:

    Xml(); 
    Xml(const char* name);
    Xml(const string& name);
    Xml(const Xml & other);
    ~Xml();
};





    }

}
