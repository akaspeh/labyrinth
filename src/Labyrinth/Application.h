#pragma once

#include <limits>

#include "Menu.h"
#include "Battle.h"

#if defined(_WIN32) || defined(WIN32)
    #define CLEAR_SCREEN() system("cls")
#else
    #define CLEAR_SCREEN() system("clear")
#endif

class Application {
private:
    Menu m_menu;
    BattleContext m_battle;
    
public:
    Application()
        : m_battle(std::make_shared<Maze>(10, 10))
    {
        std::cout << "Aplication start" << std::endl;
    }
    ~Application(){
        std::cout << "Aplication stop"  << std::endl;
    }
    void Run(){
        while(true){
            // std::cin.clear();
            int32_t code;
            std::cout << std::endl;
            std::cout << "1.CreateMaze()" << std::endl << "2.ShowMaze()" << std::endl << "3.PathFind()" << std::endl << "4.Exit" << std::endl;
            // std::cin.clear();
            std::cin >> code;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            // CLEAR_SCREEN();
            switch (code)
            {
                case 1: m_menu.CreateMaze(); break;
                case 2: m_menu.ShowMaze(); break;
                case 3: m_menu.PathFind(); break;
                case 4: code = -1; break;
                case 5:
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    m_battle.Run();
                    // we want to clear cin buffer before using it (because we use C io like getchar())
                    // std::cin.ignore(std::numeric_limits<std::streamsize>::max());
                }; break;
            }
            if(code == -1){
                break;
            }
        }
    }
};


