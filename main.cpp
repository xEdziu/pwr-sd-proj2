#include <cstdlib>
#include <ncurses.h>
#include <menu.h>
#include <filesystem>
#include <cstring>  
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include "ArrayPriorityQueue.hpp"
#include "HeapPriorityQueue.hpp"

// Data structures
ArrayPriorityQueue<std::string>* arrayPriorityQueue;
HeapPriorityQueue<std::string>* heapPriorityQueue;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

// We use fs for filesystem
namespace fs = std::filesystem;

std::vector<std::string> txtFilesInDirectory() {
    std::vector<std::string> files;
    for (const auto & entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".txt") {
            files.push_back(entry.path().filename());
        }
    }
    return files;
}

void printIntro() {
    // Print ASCII art and project info
    mvprintw(0, 0, " _____  _    _  _____ ");
    mvprintw(1, 0, "|  __ \\| |  | ||  __ \\");
    mvprintw(2, 0, "| |__) | |/\\| || |__) |");
    mvprintw(3, 0, "|  ___/|      ||  _  / ");
    mvprintw(4, 0, "| |    |  /\\  || | \\ \\ ");
    mvprintw(5, 0, "|_|    |_/  \\_||_|  \\_\\");
    mvprintw(6, 0, "Project 2 - Data Structures | Adrian Goral 272545 | Paulina Szulc 272592");
}

template<typename Structure>
void displaySubChoices(const char *choice, MENU* main_menu, Structure* &structure) {
    const char *subChoices[] = {
        "Create Empty Structure",
        "insert(e, p) - element e of priority p",
        "extractMax() - remove and return element of highest priority",
        "findMax() - return element of highest priority",
        "modifyKey(e, p) - change priority of element e to p",
        "size() - return number of elements",
        "Print",
        "Exit"
    };

    ITEM **my_items;
    int c;
    MENU *my_menu;
    int n_choices = ARRAY_SIZE(subChoices);
    ITEM *cur_item;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    int startx = 0;
    int starty = 8;

    my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for(int i = 0; i < n_choices; ++i) {
        my_items[i] = new_item(subChoices[i], "");
    }
    my_items[n_choices] = (ITEM *)NULL;

    my_menu = new_menu((ITEM **)my_items);
    mvprintw(LINES - 2, 0, "F1 to Exit, ESC to go back to main menu");
    set_menu_mark(my_menu, " * ");
    set_menu_win(my_menu, stdscr);
    set_menu_sub(my_menu, derwin(stdscr, n_choices + 1, 40, starty, startx));
    post_menu(my_menu);
    refresh();

    bool leave = false;

    while((c = getch()) != KEY_F(1) && !leave) {
        switch(c) {
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
            case 10: /* Enter */
                cur_item = current_item(my_menu);
                if (strcmp(item_name(cur_item), "Create Empty Structure") == 0) {
                    clear();
                    mvprintw(0, 0, "Press any key to return to main menu");
                    structure = new Structure();
                    mvprintw(1, 0, "Empty structure created");
                    leave = true;
                    break;
                } else if (strcmp(item_name(cur_item), "insert(e, p) - element e of priority p") == 0) {
                    clear();
                    echo();
                    mvprintw(0, 0, "Enter element e: ");
                    char e_cstr[1000]; // Adjust the size as needed
                    scanw("%s", e_cstr);
                    std::string e = e_cstr;
                    mvprintw(1, 0, "Enter priority p: ");
                    int p;
                    scanw("%d", &p);
                    structure->insert(e, p);
                    mvprintw(2, 0, "Element inserted");
                    mvprintw(3, 0, "Press any key to return to main menu");
                    noecho();
                    leave = true;
                    break;
                } else if (strcmp(item_name(cur_item), "extractMax() - remove and return element of highest priority") == 0) {
                    clear();
                    mvprintw(0, 0, "Extracted element: %s", structure->extractMax().c_str());
                    mvprintw(1, 0, "Press any key to return to main menu");
                    leave = true;
                    break;
                } else if (strcmp(item_name(cur_item), "findMax() - return element of highest priority") == 0) {
                    clear();
                    mvprintw(0, 0, "Element of highest priority: %s", structure->findMax().c_str());
                    mvprintw(1, 0, "Press any key to return to main menu");
                    leave = true;
                    break;
                } else if (strcmp(item_name(cur_item), "modifyKey(e, p) - change priority of element e to p") == 0) {
                    clear();
                    echo();
                    mvprintw(0, 0, "Enter element e: ");
                    char e_cstr[1000]; // Adjust the size as needed
                    scanw("%s", e_cstr);
                    std::string e = e_cstr;
                    mvprintw(1, 0, "Enter new priority p: ");
                    int p;
                    scanw("%d", &p);
                    structure->modifyKey(e, p);
                    mvprintw(2, 0, "Element priority modified");
                    mvprintw(3, 0, "Press any key to return to main menu");
                    noecho();
                    leave = true;
                    break;
                } else if (strcmp(item_name(cur_item), "size() - return number of elements") == 0) {
                    clear();
                    mvprintw(0, 0, "Number of elements: %zu", structure->size());
                    mvprintw(1, 0, "Press any key to return to main menu");
                    leave = true;
                    break;
                } else if (strcmp(item_name(cur_item), "Print") == 0) {
                    clear();
                    mvprintw(0, 0, "Printing 10 first elements of structure...");
                    for (int i = 3; i < 13; i++) {
                        structure->print();
                    }
                    mvprintw(1, 0, "Press any key to return to main menu");
                    leave = true;
                    break;
                } else if (strcmp(item_name(cur_item), "Exit") == 0) {
                    leave = true;
                    break;
                }

                    
            case 27: /* ESC */
                leave = true;
                break;
            if (!leave) {
                // Jeśli nie opuszczamy, odśwież menu
                refresh();
            }
        }
    }
    // Przed opuszczeniem funkcji:
    unpost_menu(my_menu);  // Odpięcie menu
    for(int i = 0; i < n_choices; ++i) {
        free_item(my_items[i]);  // Zwolnienie pamięci zajmowanej przez elementy
    }
    free_menu(my_menu);  // Zwolnienie pamięci zajmowanej przez menu
    
    clear();
    printIntro();
    post_menu(main_menu);  // Ponowne postawienie menu
    mvprintw(LINES - 2, 0, "F1 to Exit");
    refresh();  // Odświeżenie ekranu
}

template<typename Structure>
long long performTestModifyKey(int priority, std::string value, Structure* &structure){
    auto start = std::chrono::high_resolution_clock::now();
    structure->modifyKey(value, priority);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

template<typename Structure>
long long performTestInsert(int priority, std::string value, Structure* &structure){
    auto start = std::chrono::high_resolution_clock::now();
    structure->insert(value, priority);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

std::string generateValue() {
    std::string str = "";
    for (int j = 0; j < 10; j++) {
        str += (char)(rand() % 4 + 65);
    }
    return str;
}

int generatePriority() {
    return rand() % 3999999999 + 1;
}

void testStructures(MENU *main_menu) {
    endwin();
    std::cout << "Performing structures tests..." << std::endl;
    std::ofstream output("results.csv");
    output << "action;type;size;timeUs\n";
    int line = 0;

    int dataSets[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, 512000};

    auto startMain = std::chrono::high_resolution_clock::now();

    //modifyKey tests for ArrayPriorityQueue
    int priority = generatePriority();
    for (int size : sizes) {
        int sum = 0;
        for (int set : dataSets) {
            std::cout << "Set: " << set << " Size: " << size << " ArrayPriorityQueue" << " modifyKey" << std::endl;
            for (int i = 1; i <= 100; i++){
                std::string filename = "zbior_" + std::to_string(set) + "_" + std::to_string(size) + ".txt";
                ArrayPriorityQueue<std::string>* structure = new ArrayPriorityQueue<std::string>(filename.c_str(), size);
                bool found = false;
                do {
                    std::string value = generateValue();
                    if (structure->hasValue(value)) {
                        found = true;
                        sum += performTestModifyKey<ArrayPriorityQueue<std::string>>(priority, value, structure);
                    }
                } while (!found);
                delete structure;
            }
        }
        output << "modifyKey;ArrayPriorityQueue;" << size << ";" << (sum/1000) << "\n";
        sum = 0;
    }

    //modifyKey tests for HeapPriorityQueue
    for (int size : sizes) {
        int sum = 0;
        for (int set : dataSets) {
            std::cout << "Set: " << set << " Size: " << size << " HeapPriorityQueue" << " modifyKey" << std::endl;
            for (int i = 1; i <= 100; i++){
                std::string filename = "zbior_" + std::to_string(set) + "_" + std::to_string(size) + ".txt";
                HeapPriorityQueue<std::string>* structure = new HeapPriorityQueue<std::string>(filename.c_str(), size);
                bool found = false;
                do {
                    std::string value = generateValue();
                    if (structure->hasValue(value)) {
                        found = true;
                        sum += performTestModifyKey<HeapPriorityQueue<std::string>>(priority, value, structure);
                    }
                } while (!found);
                delete structure;
            }
        }
        output << "modifyKey;HeapPriorityQueue;" << size << ";" << (sum/1000) << "\n";
        sum = 0;
    }

    
    std::string valueInsert = generateValue();
    int priorityInsert = generatePriority();

    //insert tests for ArrayPriorityQueue
    for (int size : sizes) {
        int sum = 0;
        for (int set : dataSets) {
            std::cout << "Set: " << set << " Size: " << size << " ArrayPriorityQueue" << " insert" << std::endl;
            for (int i = 1; i <= 100; i++) {
                std::string filename = "zbior_" + std::to_string(set) + "_" + std::to_string(size) + ".txt";
                ArrayPriorityQueue<std::string>* structure = new ArrayPriorityQueue<std::string>(filename.c_str(), size);
                sum += performTestInsert(priorityInsert, valueInsert, structure);
                delete structure;
            }
        }
        output << "insert;ArrayPriorityQueue;" << size << ";" << (sum / 1000) << "\n";
        sum = 0;
    }

    //insert tests for HeapPriorityQueue
    for (int size : sizes) {
        int sum = 0;
        for (int set : dataSets) {
            std::cout << "Set: " << set << " Size: " << size << " HeapPriorityQueue" << " insert" << std::endl;
            for (int i = 1; i <= 100; i++) {
                std::string filename = "zbior_" + std::to_string(set) + "_" + std::to_string(size) + ".txt";
                HeapPriorityQueue<std::string>* structure = new HeapPriorityQueue<std::string>(filename.c_str(), size);
                sum += performTestInsert(priorityInsert, valueInsert, structure);
                delete structure;
            }
        }
        output << "insert;HeapPriorityQueue;" << size << ";" << (sum / 1000) << "\n";
        sum = 0;
    }

    //extractMax tests for ArrayPriorityQueue
    for (int size : sizes) {
        int sum = 0;
        for (int set : dataSets) {
            std::cout << "Set: " << set << " Size: " << size << " ArrayPriorityQueue" << " extractMax" << std::endl;
            for (int i = 1; i <= 100; i++){
                std::string filename = "zbior_" + std::to_string(set) + "_" + std::to_string(size) + ".txt";
                ArrayPriorityQueue<std::string>* structure = new ArrayPriorityQueue<std::string>(filename.c_str(), size);
                auto start = std::chrono::high_resolution_clock::now();
                std::string tmp = structure->extractMax();
                auto end = std::chrono::high_resolution_clock::now();
                sum += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                delete structure;
            }
        }
        output << "extractMax;ArrayPriorityQueue;" << size << ";" << (sum/ 1000) << "\n";
        sum = 0;
    }

    //extractMax tests for HeapPriorityQueue
    for (int size : sizes) {
        int sum = 0;
        for (int set : dataSets) {
            std::cout << "Set: " << set << " Size: " << size << " HeapPriorityQueue" << " extractMax" << std::endl;
            for (int i = 1; i <= 100; i++){
                std::string filename = "zbior_" + std::to_string(set) + "_" + std::to_string(size) + ".txt";
                HeapPriorityQueue<std::string>* structure = new HeapPriorityQueue<std::string>(filename.c_str(), size);
                auto start = std::chrono::high_resolution_clock::now();
                std::string tmp = structure->extractMax();
                auto end = std::chrono::high_resolution_clock::now();
                sum += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                delete structure;
            }
        }
        output << "extractMax;HeapPriorityQueue;" << size << ";" << (sum/ 1000) << "\n";
        sum = 0;
    }

    //findMax tests for ArrayPriorityQueue
    for (int size : sizes) {
        int sum = 0;
        for (int set : dataSets) {
            std::cout << "Set: " << set << " Size: " << size << " ArrayPriorityQueue" << " findMax" << std::endl;
            for (int i = 1; i <= 100; i++){
                std::string filename = "zbior_" + std::to_string(set) + "_" + std::to_string(size) + ".txt";
                ArrayPriorityQueue<std::string>* structure = new ArrayPriorityQueue<std::string>(filename.c_str(), size);
                auto start = std::chrono::high_resolution_clock::now();
                std::string tmp = structure->findMax();
                auto end = std::chrono::high_resolution_clock::now();
                sum += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                delete structure;
            }
        }
        output << "findMax;ArrayPriorityQueue;" << size << ";" << (sum/1000) << "\n";
        sum = 0;
    }

    //findMax tests for HeapPriorityQueue
    for (int size : sizes) {
        int sum = 0;
        for (int set : dataSets) {
            std::cout << "Set: " << set << " Size: " << size << " HeapPriorityQueue" << " findMax" << std::endl;
            for (int i = 1; i <= 100; i++){
                std::string filename = "zbior_" + std::to_string(set) + "_" + std::to_string(size) + ".txt";
                HeapPriorityQueue<std::string>* structure = new HeapPriorityQueue<std::string>(filename.c_str(), size);
                auto start = std::chrono::high_resolution_clock::now();
                std::string tmp = structure->findMax();
                auto end = std::chrono::high_resolution_clock::now();
                sum += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                delete structure;
            }
        }
        output << "findMax;HeapPriorityQueue;" << size << ";" << (sum/1000) << "\n";
        sum = 0;
    }

    //size tests for ArrayPriorityQueue
    for (int size : sizes) {
        int sum = 0;
        for (int set : dataSets) {
            std::cout << "Set: " << set << " Size: " << size << " ArrayPriorityQueue" << " size" << std::endl;
            for (int i = 1; i <= 100; i++){
                std::string filename = "zbior_" + std::to_string(set) + "_" + std::to_string(size) + ".txt";
                ArrayPriorityQueue<std::string>* structure = new ArrayPriorityQueue<std::string>(filename.c_str(), size);
                auto start = std::chrono::high_resolution_clock::now();
                size_t tmp = structure->size();
                auto end = std::chrono::high_resolution_clock::now();
                sum += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                delete structure;
            }
        }
        output << "size;ArrayPriorityQueue;" << size << ";" << (sum/ 1000) << "\n";
        sum = 0;
    }

    //size tests for HeapPriorityQueue
    for (int size : sizes) {
        int sum = 0;
        for (int set : dataSets) {
            std::cout << "Set: " << set << " Size: " << size << " HeapPriorityQueue" << " size" << std::endl;
            for (int i = 1; i <= 100; i++){
                std::string filename = "zbior_" + std::to_string(set) + "_" + std::to_string(size) + ".txt";
                HeapPriorityQueue<std::string>* structure = new HeapPriorityQueue<std::string>(filename.c_str(), size);
                auto start = std::chrono::high_resolution_clock::now();
                size_t tmp = structure->size();
                auto end = std::chrono::high_resolution_clock::now();
                sum += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                delete structure;
            }
        }
        output << "size;HeapPriorityQueue;" << size << ";" << (sum/ 1000) << "\n";
        sum = 0;
    }
    
    auto endMain = std::chrono::high_resolution_clock::now();

    output.close();

    refresh();
    clear();

    mvprintw(line++, 0, "Tests have ended. Results saved to results.csv");
    mvprintw(line++, 0, "Time elapsed: %ld seconds", std::chrono::duration_cast<std::chrono::seconds>(endMain - startMain).count());
    mvprintw(line++, 0, "Press any key to return to main menu");
    refresh();
    getch();
    clear();
    printIntro();
    post_menu(main_menu);
    mvprintw(LINES - 2, 0, "F1 to Exit");
    refresh();
}

int main() {

    const char *choices[] = {
        "Array Priority Queue",
        "Heap Priority Queue",
        "Perform Structures Tests"
    };

    ITEM **my_items;
    int c;
    MENU *my_menu;
    int n_choices = ARRAY_SIZE(choices);
    ITEM *cur_item;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    int startx = 0;
    int starty = 8;

    printIntro();

    my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for(int i = 0; i < n_choices; ++i) {
        my_items[i] = new_item(choices[i], "");
    }
    my_items[n_choices] = (ITEM *)NULL;

    my_menu = new_menu((ITEM **)my_items);
    mvprintw(LINES - 2, 0, "F1 to Exit");
    set_menu_mark(my_menu, " * ");
    set_menu_win(my_menu, stdscr);
    set_menu_sub(my_menu, derwin(stdscr, n_choices + 1, 40, starty, startx));
    post_menu(my_menu);
    refresh();

    while((c = getch()) != KEY_F(1)) {
        switch(c) {
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
            case 10: /* Enter */
            { 
                if (strcmp(item_name(current_item(my_menu)), "Perform Structures Tests") == 0) {
                    unpost_menu(my_menu);
                    clear();
                    testStructures(my_menu);
                    refresh();
                } else {
                    ITEM *cur_item = current_item(my_menu);
                    unpost_menu(my_menu);  // Odpięcie menu przed przejściem do sub-menu
                    clear();  // Wyczyszczenie ekranu przed przejściem do sub-menu
                    printIntro();

                    if (strcmp(item_name(cur_item), "Array Priority Queue") == 0) {
                        displaySubChoices(item_name(cur_item), my_menu, arrayPriorityQueue);
                    } else if (strcmp(item_name(cur_item), "Heap Priority Queue") == 0) {
                        displaySubChoices(item_name(cur_item), my_menu, heapPriorityQueue);
                    }

                    refresh();
                }
                break;
            }
        }
    }
    
    unpost_menu(my_menu);  // Odpięcie menu
    for(int i = 0; i < n_choices; ++i) {
        free_item(my_items[i]);  // Zwolnienie pamięci zajmowanej przez elementy
    }
    free_menu(my_menu);  // Zwolnienie pamięci zajmowanej przez menu
    endwin();
    
    return 0;
}