#pragma once

#include<string>
#include "Xml.h"

namespace yazi
{
    namespace xml{
        class Parser
        {
            public:
            Parser();
            ~Parser();
            void loadStr(const string& str);
            void loadFile(const string& str);
            Xml parse();

            private:
            char skipWhiteChar();
            bool parse_declaration();// 找到第一个是尖括号
            bool parser_comment();

            string parse_element_name();
            string parser_text();
            string parser_attr_key();
            string parser_attr_val();



            string  m_str;// 保存需要处理的字符串
            int m_index{0};// 处理字符串的索引

        };

        // 参考tinyxml2.h 写的工具
        class  XMLUItil
        {
            public:
           inline static bool IsNameStartChar(unsigned char c)
           {
            if(c >= 128){
                return true;
            }
            if( isalpha(c))
            {
                return true;
            }
            return c == '_' || c == ':'; 
           }
           inline static bool IsNameChar(unsigned char c)
           {
            return IsNameStartChar(c) || isdigit(c)||c == '.' ||c == '-';
           }

        };

    }

} // namespace yazi
