#include "../include/Editor.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

string massage = "";

int main() {
    TextEditor editor;
    initEditor(&editor);
    int command, commend_2;
    string input;

    do {
        bool found = false;
        bool file_open = false;
        string file = "";
        menu();
        commend_2 = chose_2();
        if (commend_2 == 2) {
            cout << MAGENTA << "\n\tEnter filename: " << RESET;
            cin.ignore();
            getline(cin, file);
            loadFromFile(&editor, file);
            file_open = true;
        }
        if (commend_2 == 3) {
            cout << "Goodbye!\n";
            break;
        }
        do {
            displayContent(&editor, file);
            cout << setw(20) << massage << RESET << endl;
            command = chose();
            cin.ignore(); // clear newline from input buffer
            massage = "";
            switch (command) {
                case 1: // Insert text
                    cout << "Enter text: ";
                    getline(cin, input);
                    insertText(&editor, input);
                    break;
                case 4: // New line
                    insertNewLine(&editor);
                    break;
                case 2: // Delete character
                    deleteChar(&editor);
                    break;
                case 3: // Delete line
                    deleteLine(&editor);
                    break;
                case 5:
                case 6:
                case 7:
                case 8:
                case 11:
                case 12:
                    moveCursor(&editor, command);
                    break;
                case 10: // Undo
                    undo(&editor);
                    break;
                case 9: // Redo
                    redo(&editor);
                    break;
                case 16: // Save to file
                {
                    if(!file_open && !found){
                        cout << MAGENTA << "\n\tEnter filename: " << RESET;
                        getline(cin, input);
                        file = input;
                        saveToFile(&editor, input);
                        found = true;
                    }
                    if(file_open || found){
                        saveToFile(&editor, file);
                        found = true;
                    }
                }
                break;
                case 17: // Load from file
                {
                    if(!file_open && !found){
                        cout << MAGENTA << "\n\tEnter filename: " << RESET;
                        getline(cin, input);
                        loadFromFile(&editor, input);
                    }
                    if(file_open){
                        cout<< "File is open "<<endl;
                    }
                }
                break;
                case 14: // Find text
                    cout << MAGENTA << "\n\tEnter text to search: " << RESET;
                    getline(cin, input);
                    searchText(&editor, input);
                    break;
                case 13: // Replace
                {
                    string oldText, newText;
                    cout << MAGENTA << "\n\tEnter text to replace: " << RESET;
                    getline(cin, oldText);
                    cout << MAGENTA << "\n\tEnter new text: " << RESET;
                    getline(cin, newText);
                    char choice;
                    cout << "Replace all? (y/n): ";
                    cin >> choice;
                    cin.ignore();
                    replaceText(&editor, oldText, newText, (choice == 'y' || choice == 'Y'));
                    cout << "Press Enter to continue...";
                    cin.get();
                }
                break;
                case 15: // Statistics
                    showStatistics(&editor);
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                case 18: // Exit
                {
                    if (!found && !file_open) {
                        char saveChoice;
                        cout << "Do you want to save changes? (y/n): ";
                        cin >> saveChoice;
                        cin.ignore();
                        if (saveChoice == 'y' || saveChoice == 'Y') {
                            cout << "Enter filename: ";
                            getline(cin, input);
                            saveToFile(&editor, input);
                        }
                    }
                    cout << "Goodbye!\n";
                    deleteAllLines(editor.head);
                    clearStack(&editor.undoStack);
                    clearStack(&editor.redoStack);
                    cout << "Press Enter to continue...";
                    cin.get();
                }
                break;
                default:
                    cout << "Unknown command!\n";
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
            }
        } while (command != 18);
    } while (commend_2 != 3);

    return 0;
}
