#include<iostream>
#include <fstream>
#include<vector>
#include <cctype>
int main(){
    std::vector<char> src;
    std::ifstream file("test.txt");
    char ch;

    if(!file.is_open()){
        std::cerr<<"Error opening the file \n";
        return 1;
    }

    while(file.get(ch))
        src.push_back(ch);

    int m = 0;
    while(m < src.size()){
        if(!std::isdigit(static_cast<unsigned char>(src[m]))) {
            m++;
            continue;
        }
        int numberStart = m;
        bool dot = false;
        while(m < src.size() && (std::isdigit(static_cast<unsigned char>(src[m])) || src[m]=='.')){
            if(src[m]=='.'){
                if(dot || m+1 >= src.size())
                    throw std::runtime_error("Lexer Error: Invalid or misplaced decimal point.");
                if(!std::isdigit(static_cast<unsigned char>(src[m+1])) || !std::isdigit(static_cast<unsigned char>(src[m-1])))
                   throw std::runtime_error("Lexer Error: numbers must be before and after '.'");
                dot = true;
            }
            m++;
        }
        for(int j = numberStart ; j < m ; j++){
            std::cout<< src[j];
        }
        std::cout<<" ";
    }
    return 0;
}