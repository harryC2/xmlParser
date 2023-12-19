#include"Parser.h"
using namespace std;
using namespace yazi::xml;
#include <sstream>
#include <fstream>
Parser::Parser()
{

}
Parser::~Parser()
{

}
void Parser::loadFile(const string & filename)
{
    ifstream fin(filename);
    if (fin.fail())
    {
        return ;
    }
    stringstream ss;
    ss << fin.rdbuf();
    m_str = ss.str();
    m_index = 0;
    return ;
}

// bool Parser::load_string(const string & str)
// {
//     m_str = str;
//     m_idx = 0;
//     return true;
// }

void Parser::loadStr(const string &str)
{
    m_str = str;
    m_index = 0;
}

char Parser::skipWhiteChar()
{
    if (m_index<0 || m_index >= m_str.size())
    {
        throw logic_error("index is invalid");
    }
    while (m_str[m_index] == ' ' || m_str[m_index] == '\r' || m_str[m_index] == '\n' || m_str[m_index] == '\t')
    {
        m_index++;
    }
    return m_str[m_index];
}

Xml Parser::parse()
{
    // 头部有<？xml version ?>  这个是声明
    // <!--   -->注释用 m_str.find("-->",m_index) 来找;

    // 开始解析name
    Xml resXml;
    char chNow = skipWhiteChar();
    if (m_index == m_str.size() || chNow == '\0')
    {
        throw std::logic_error("xml document is empty");
    }

        if (m_str[m_index + 0] == '<' &&
        m_str[m_index + 1] == '?' &&
        m_str[m_index + 2] == 'x' &&
        m_str[m_index + 3] == 'm' &&
        m_str[m_index + 4] == 'l')
    {
        if (!parse_declaration())
        {
            throw std::logic_error("xml declaration is error");
        }
    }
    string name;
    chNow = skipWhiteChar();
    if (m_str[m_index + 0] == '<' &&
        m_str[m_index + 1] == '!' &&
        m_str[m_index + 2] == '-' &&
        m_str[m_index + 3] == '-')
    {
        if (!parser_comment())
        {
            throw std::logic_error("xml comment is error");
        }
    }
    chNow  = skipWhiteChar();
    if(chNow == '<')
    {
        name =parse_element_name();     
        resXml.setName(name);
    }
    else{
        return resXml;
    }
    chNow = skipWhiteChar();
    while(chNow != '\0') // 只要不是截至符号就一直 按照规则读取
    {
        //先return 结果
        if(chNow == '/')
        {
            m_index++;
            chNow == m_str[m_index]; // 下一个符号必须是>
            if(chNow != '>')
            {
                throw logic_error("xml element end error");
            }
            return resXml;
        }
        else if(chNow == '<')
        {
            // 这个可以是注释
            // 也可以是截至符号
            if(m_str[m_index+1] == '!'&&
                m_str[m_index+1] == '-'&&
                m_str[m_index+1] == '-')
                {
                    // 跳过注释
                    if(!parser_comment())
                    {
                        throw logic_error(" comment fromat error" );
                    }
                }
            else if(m_str[m_index+1] == '/')
            {
                //截至符号
                string end_tag = "</" + name + ">";
                int pos = m_str.find(end_tag,m_index);
                if(pos == std::string::npos)
                {
                    throw logic_error("error endTag");
                }
                else if(pos == m_index) 
                {
                    m_index += end_tag.size();
                    return resXml; //当前这个节点 解析完了
                }
            }
            else
            {
                // 解析子节点
                Xml xmlChild;
                xmlChild = parse();
                resXml.appendChildren(xmlChild);
            }
        
        }
        else if(chNow == '>') // 如果是> 符号那么下面就开始解析test
        {
            
            m_index ++;
            chNow = skipWhiteChar();
            string strText =  parser_text();
            if(strText != "")
            {
                resXml.setText(strText);
            }
            else
            {
                Xml element;
                element = parse();
                resXml.appendChildren(element);
            }
        }
        else
        {
            // 解析属性
            string key = parser_attr_key();
            chNow = skipWhiteChar();
            if (chNow != '=')
            {
                throw logic_error("attribute is error");
            }
            m_index++; // 跳过=号
            string value = parser_attr_val();
            resXml.setAttr(key, value);
        }
        chNow = skipWhiteChar();
    }
    return resXml;
}

bool Parser::parse_declaration() // 找到第一个是尖括号
{
    size_t pos = m_str.find("?>", m_index);
    if (pos == std::string::npos)// find函数在找不到指定的值情况下会返回npos
    {
        return false;
    }
    m_index = pos + 2;
    return true;
}
bool Parser::parser_comment()
{
    size_t pos = m_str.find("-->", m_index);
    if (pos == std::string::npos)
    {
        return false;
    }
    m_index = pos + 3;
    return true;

}

string Parser::parse_element_name()
{
    m_index++;
    stringstream ss;
    char chNow = skipWhiteChar();
    if(!XMLUItil::IsNameStartChar)
    {
        throw logic_error("name first char is error");
    }

    while(XMLUItil::IsNameChar(chNow))//|| chNow != '='
    {
        ss << chNow;
        m_index++;
        chNow = m_str[m_index];
    }
    return ss.str();
}
string Parser::parser_text()
{
    int pos = m_index;
    stringstream ss;
    while (m_str[m_index] != '<')
    {
        ss<< m_str[m_index];
        m_index++;
    }
    return ss.str(); 
}
string Parser::parser_attr_key()
{
    char chNow = skipWhiteChar();
    stringstream ss;

    // key
    if(XMLUItil::IsNameStartChar(chNow))
    {
        while(XMLUItil::IsNameChar(chNow))
        {
            ss<< chNow;
            m_index ++;
            chNow = m_str[m_index];
        }
    }

    return ss.str();
}
string Parser::parser_attr_val()
{

    char chNow = skipWhiteChar();
    // key
    stringstream ss;
    if(chNow != '"')
    {
        throw logic_error("attribute value format error");
    }
    m_index++;//跳过"""
    while (m_str[m_index] != '"')
    {
        if(m_index == m_str.size())
        {
              throw std::logic_error("unexpected end of input in string");
        }

        chNow = m_str[m_index];
        m_index++;
        if (chNow != '\\') // 跳过转义符 如果是转义符则不加进去
        {
            ss<<chNow;
            continue;
        }
    }
     m_index++;
    return ss.str();
}