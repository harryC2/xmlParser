#include<iostream>
#include"xml/Xml.h"

using namespace yazi::xml;

void test()
{
    Xml xmlTest;
    xmlTest.parse("D:\\chStudly\\vsCodeProject\\xml\\xmlTest.xml");
    std::cout << xmlTest.toString()<<std::endl;
}

int main(){

    test();

    std::getchar();


    Xml xml;
    std::cout<<xml.getName()<<std::endl;
    xml.setName("sadasd");
    std::cout<<xml.getName()<<std::endl;
    xml.setAttr("log",1);
    xml.setAttr("name","chenhao");
    std::cout<<string(xml.getAttr("name"))<<std::endl;
    xml.setAttr("name","lisi");
   
    std::cout<<xml.toString()<<std::endl;



    Xml xml2("dsa");
    xml2.setName("student");
    xml2.setText("good student");
    xml["string"]  = xml2;
    //xml.appendChildren(xml2);
    std::cout<<xml.toString()<<std::endl;
    std::getchar();
    return 0;
}