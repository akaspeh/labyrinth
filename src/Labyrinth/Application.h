#pragma once

#include "Menu.h"

class Application {
private:
    Menu m_menu;
public:
    Application(){
        std::cout << "Aplication start" << std::endl;
    }
    ~Application(){
        std::cout << "Aplication stop"  << std::endl;
    }
    void Run(){
        while(1){
            short a;
            std::cout << std::endl;
            std::cout << "1.CreateMaze()" << std::endl << "2.ShowMaze()" << std::endl << "3.PathFind()" << std::endl << "4.Exit" << std::endl;
            std::cin >> a;
            system("cls");
            switch(a){
                case 1: m_menu.CreateMaze(); break;
                case 2: m_menu.ShowMaze(); break;
                case 3: m_menu.PathFind(); break;
                case 4: a = -1;
                default: break;
            }
            if(a == -1){
                break;
            }
        }
    }
};


