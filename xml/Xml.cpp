#include"Xml.h"
#include<sstream>


using namespace yazi::xml;


Xml::Xml(/* args */)
{
}

Xml::~Xml()
{
}
Xml::Xml(const char* name){
// 声明一个node节点
 m_name = new string(name);
}
Xml::Xml(const string& name)
{
    m_name = new string(name);
}
Xml::Xml(const Xml & other)
{
    m_name = other.m_name;

    m_attrs = other.m_attrs;

    m_text = other.m_text;


    m_child = other.m_child;
}


string Xml::getName() const
{
    if (m_name == nullptr)
    {
        return "";
    }
    return *m_name;
}
string Xml::getText() const
{
    if (m_text == nullptr)
    {
        return "";
    }
    return *m_text;
}

void Xml::setName(const string &name)
{
    if (m_name != nullptr)
    {
        SAFEDELETE(m_name);
    }
    m_name = new string(name);
}
void Xml::setText(const string &test)
{
    if (m_text != nullptr)
    {
        SAFEDELETE(m_text);
    }
    m_text = new string(test);
}

string Xml::getAttr(const string& key)
{
    if(m_attrs == nullptr)
    {
        return "";
    }
    return m_attrs->at(key);
    //std::map<string,string>* m_attrs{nullptr}; // 属性
}

void Xml::setAttr(const string& key,const string & val)
{
    if(m_attrs == nullptr)
    {
        m_attrs = new std::map<string,string>();
    }
   auto resultPair = m_attrs->emplace(key,val); // 避免了临时创建对象，但是如果不存在的话，插入不进去
   if (!resultPair.second)
   {
    resultPair.first->second = val;
    //m_attrs->insert(resultPair.first,std::make_pair(key,val));
    //m_attrs->insert(std::make_pair(key,val));
   }  
}
string Xml::toString()const
{
    if(m_name == nullptr)
    {
        throw std::logic_error("element name is empty");
    }
    std::stringstream ss;
    ss<< "<";
    ss << *m_name;
    if(m_attrs != nullptr)
    {
        for(const auto& item : *m_attrs)
        {
            ss<< " ";
            ss<< item.first ;
            ss<< " =\"";
            ss<< item.second;
            ss<< "\"";
        }
    }
    ss<< ">";
    if(m_child != nullptr)
    {
    for(auto item : *m_child)
        {
            ss<<item.toString();
        }
    }

    if(m_text != nullptr)
    {
        ss<< *m_text;
    }
    ss<< "</" <<*m_name << ">";
    return ss.str();
}

void Xml::appendChildren(const Xml& children)
{
    if(m_child == nullptr)
    {
        m_child = new std::list<Xml>();
    }
    m_child->emplace(m_child->end(), children);
}
// return left refrence  是为了可以修改当前的子节点 同步生效？
Xml& Xml::operator[](int index)
{
    if(index < 0)
    {
        throw std::logic_error(" idnex less than zero");
    }
    if(m_child == nullptr)
    {
        m_child = new std::list<Xml>(); // 保存的是对象 node节点有点大 可以保存指针 //增删改查子节点的时候得注意内存释放
    }
    int size = m_child->size();
    // 注意边界条件
    if(index >= 0 && index < size)
    {
        auto it = m_child->begin();
        for(int i = 0; i < index; i++)
        {
            it++;
        }
        return *it;
    }
    else if( index >= size)
    {
        //扩容或者返回空？
        throw std::logic_error(" idnex less than zero");
        //return Xml();
    }

}

Xml& Xml::operator[] (const char* name)
{
    return (*this)[string(name)];
}

Xml& Xml::operator[] (const string& name)
{
    if(m_child == nullptr)
    {
        m_child = new std::list<Xml>();
    }

    for (auto& item : (*m_child))
    {
        if(item.getName() == name)
        {
            return item; 
        }
    }
    appendChildren(Xml(name)); 
    return m_child->back();
}

Xml& Xml::operator = (const Xml& other)
{
    clear();

    m_name = other.m_name;

    m_attrs = other.m_attrs;

    m_text = other.m_text;


    m_child = other.m_child;
}
void Xml::clear()
{
    SAFEDELETE(m_name);
    SAFEDELETE(m_attrs);
    SAFEDELETE(m_text);
    if (m_child != nullptr)
    {
        for (auto it = m_child->begin(); it != m_child->end(); it++)
        {
            it->clear();
        }
        SAFEDELETE(m_child);
    }
}

void Xml::removeChildren(int index)
{
    if(m_child == nullptr)
    {
       return;
    }

    int nSize = m_child->size();
    if(index <0 || index >=nSize)
    {
        throw logic_error("index is illower number");
    }
    auto iter  = m_child->begin();
    while (--index)
    {
        iter++;
    }
    iter->clear();//防止内存泄露  memory leak
    m_child->erase(iter);
}
void Xml::removeChildren(const char* name)
{
    removeChildren(string(name));
}
void Xml::removeChildren(const string& name)
{
    if(m_child == nullptr)
    {
       return;
    }
    for(auto it = m_child->begin(); it != m_child->end();)
    {
        if(it->getName() == name)
        {
             it->clear();
            it = m_child->erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Xml::removeAttribute(const char* name)
{
    removeAttribute(string(name));
}
void Xml::removeAttribute(const string& name)
{
    if(m_attrs == nullptr)
    {
       return;
    }
    for(auto it = m_attrs->begin(); it != m_attrs->end();it++)
    {
        if(it->first == name)
        {
            it = m_attrs->erase(it);
            return;
        }
    }
}
