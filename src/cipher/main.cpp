#include <iostream>
#include "Blowfish.hpp"

// DEAD SIMPLE SYMMETRIC ENCRYPTION

int main(int argc, const char * argv[])
{
    std::string key = "SECRET";
    
    Blowfish blowfish;
    blowfish.setKey(key);
    
    std::string text = "Hello, Blowfish!----";
    
    blowfish.encrypt(text, text);
    std::cout << text << std::endl;
    
    blowfish.decrypt(text, text);
    std::cout << text << std::endl;
    
    return 0;
}