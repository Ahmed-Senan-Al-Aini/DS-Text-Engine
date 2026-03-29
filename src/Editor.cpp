#include "../include/Editor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cctype>

using namespace std;

// Delete all lines
void deleteAllLines(LineNode *head) {
    LineNode *current = head;
    while (current) {
        LineNode *next = current->next;
        delete current;
        current = next;
    }
}

// Copy lines
LineNode *copyLines(LineNode *sourceHead, LineNode *sourceCursor, LineNode **destCursorPtr) {
    if (!sourceHead) {
        *destCursorPtr = nullptr;
        return nullptr;
    }
    LineNode *newHead = nullptr;
    LineNode *newTail = nullptr;
    LineNode *current = sourceHead;
    while (current) {
        LineNode *newNode = new LineNode;
        newNode->text = current->text;
        newNode->next = nullptr;
        newNode->prev = nullptr;
        if (!newHead) {
            newHead = newNode;
            newTail = newNode;
        } else {
            newTail->next = newNode;
            newNode->prev = newTail;
            newTail = newNode;
        }
        if (current == sourceCursor) {
            *destCursorPtr = newNode;
        }
        current = current->next;
    }
    if (!(*destCursorPtr) && newHead)
        *destCursorPtr = newHead;
    return newHead;
}

// Stack functions
StackNode *createStackNode(LineNode *head, LineNode *cursor, int pos, int totalLines) {
    StackNode *node = new StackNode;
    node->cursorPos = pos;
    node->totalLines = totalLines;
    node->next = nullptr;
    if (head)
        node->editorHead = copyLines(head, cursor, &node->cursorLinePtr);
    else {
        node->editorHead = nullptr;
        node->cursorLinePtr = nullptr;
    }
    return node;
}

void pushStack(LinkedListStack *stack, LineNode *head, LineNode *cursor, int pos, int totalLines) {
    StackNode *node = createStackNode(head, cursor, pos, totalLines);
    node->next = stack->top;
    stack->top = node;
    stack->size++;
}

bool popStack(LinkedListStack *stack) {
    if (!stack->top) return false;
    StackNode *temp = stack->top;
    stack->top = temp->next;
    delete temp;
    stack->size--;
    return true;
}

StackNode *peekStack(LinkedListStack *stack) {
    return stack->top;
}

bool isStackEmpty(LinkedListStack *stack) {
    return stack->top == nullptr;
}

void clearStack(LinkedListStack *stack) {
    while (!isStackEmpty(stack)) popStack(stack);
}

void initStack(LinkedListStack *stack) {
    stack->top = nullptr;
    stack->size = 0;
}

// Editor functions
void initEditor(TextEditor *editor) {
    editor->head = new LineNode;
    editor->head->text = "";
    editor->head->next = nullptr;
    editor->head->prev = nullptr;
    editor->tail = editor->head;
    editor->cursorLinePtr = editor->head;
    editor->cursorPos = 0;
    editor->totalLines = 1;
    initStack(&editor->undoStack);
    initStack(&editor->redoStack);
    pushStack(&editor->undoStack, editor->head, editor->cursorLinePtr, editor->cursorPos, editor->totalLines);
}

void saveState(TextEditor *editor) {
    pushStack(&editor->undoStack, editor->head, editor->cursorLinePtr, editor->cursorPos, editor->totalLines);
    clearStack(&editor->redoStack);
}

void restoreState(TextEditor *editor, StackNode *state) {
    if (!state || !state->editorHead) return;
    deleteAllLines(editor->head);
    editor->head = copyLines(state->editorHead, state->cursorLinePtr, &editor->cursorLinePtr);
    editor->cursorPos = state->cursorPos;
    editor->totalLines = state->totalLines;
    editor->tail = editor->head;
    if (editor->tail) {
        while (editor->tail->next) editor->tail = editor->tail->next;
    }
}

int countWords(TextEditor *editor) {
    int wordCount = 0;
    bool inWord = false;
    LineNode *current = editor->head;
    while (current) {
        for (char c : current->text) {
            if (isalnum(c) || c == '_') {
                if (!inWord) { wordCount++; inWord = true; }
            } else inWord = false;
        }
        inWord = false;
        current = current->next;
    }
    return wordCount;
}

int countChars(TextEditor *editor) {
    int charCount = 0;
    LineNode *current = editor->head;
    while (current) {
        charCount += current->text.length();
        current = current->next;
    }
    return charCount;
}

void content(string arr[], int end) {
    for (int i = 0; i < end; i++)
        cout << YELLOW << i + 1 << ((i >= 9) ? "" : " ") << "> " << RESET << arr[i] << endl;
}

void menu() {
    system(CLEAR_COMMAND);
    cout << "\n\t\t" << CYAN << " WELLCOM TO" << YELLOW << " TEXT EDITOR " << RESET;
    cout << GREEN << "\n ----- >>> Commands <<< -----\n" << RESET;
    string arr[] = {" New File ", " Open File ", " Exit "};
    content(arr, 3);
    cout << GREEN << "\n ----- >>> Commands <<< -----\n" << RESET;
}

void displayContent(TextEditor *editor, string file) {
    system(CLEAR_COMMAND);
    cout << GREEN << "\n\t File : " << file << "\n" << RESET;
    cout << YELLOW << "\n ----- >>> Commands <<< -----\n" << RESET;
    string arr[] = {"Insert text", "Delete character", "Delete line", "New line", "Up", "Down", "Left", "Right", "Redo", "Undo", "Begin line", "End line", "Replace", "Find word", "Statistics", "Save to file", "Open file", "Exit"};
    content(arr, 18);
    cout << YELLOW << " <<--------------------------->>\n\n" << RESET;
    LineNode *current = editor->head;
    int lineNum = 1;
    int line = 0;
    cout << setw(15) << "  =================================" << endl;
    while (current) {
        cout << "||" << GREEN << lineNum << ": " << RESET << current->text;
        if (current->text.empty()) cout << "  ";
        cout << endl;
        if (current == editor->cursorLinePtr) {
            line = lineNum;
            cout << "||  ";
            for (int j = 0; j < editor->cursorPos; j++) cout << " ";
            cout << MAGENTA << " ^\n" << RESET;
        }
        current = current->next;
        lineNum++;
    }
    cout << setw(15) << "  =================================" << endl;
    cout << setw(15) << "  ------------------------------" << endl;
    cout << "|" << CYAN << setw(10) << "   COL[ " << editor->cursorPos + 1 << " ]"
         << setw(10) << "    ROW[ " << line << " ]"
         << setw(10) << "    Charectars[ " << countChars(editor) << " ]"
         << setw(10) << "    Words[ " << countWords(editor) << " ]" << RESET;
    cout << setw(15) << "\n  ------------------------------" << endl;
}

void insertText(TextEditor *editor, const string &text) {
    saveState(editor);
    if (editor->cursorPos > (int)editor->cursorLinePtr->text.length())
        editor->cursorPos = editor->cursorLinePtr->text.length();
    editor->cursorLinePtr->text.insert(editor->cursorPos, text);
    editor->cursorPos += text.length();
}

void insertNewLine(TextEditor *editor) {
    saveState(editor);
    string currentLine = editor->cursorLinePtr->text;
    string firstPart = currentLine.substr(0, editor->cursorPos);
    string secondPart = currentLine.substr(editor->cursorPos);
    editor->cursorLinePtr->text = firstPart;
    LineNode *newNode = new LineNode;
    newNode->text = secondPart;
    newNode->next = editor->cursorLinePtr->next;
    newNode->prev = editor->cursorLinePtr;
    if (editor->cursorLinePtr->next) editor->cursorLinePtr->next->prev = newNode;
    editor->cursorLinePtr->next = newNode;
    if (editor->cursorLinePtr == editor->tail) editor->tail = newNode;
    editor->cursorLinePtr = newNode;
    editor->cursorPos = 0;
    editor->totalLines++;
}

void deleteChar(TextEditor *editor) {
    if (!editor->head) return;
    saveState(editor);
    string &line = editor->cursorLinePtr->text;
    if (editor->cursorPos < (int)line.length()) {
        line.erase(editor->cursorPos, 1);
    } else if (editor->cursorPos == (int)line.length() && editor->cursorLinePtr != editor->tail) {
        line += editor->cursorLinePtr->next->text;
        LineNode *toDelete = editor->cursorLinePtr->next;
        editor->cursorLinePtr->next = toDelete->next;
        if (toDelete->next) toDelete->next->prev = editor->cursorLinePtr;
        if (toDelete == editor->tail) editor->tail = editor->cursorLinePtr;
        delete toDelete;
        editor->totalLines--;
    }
    if (editor->cursorPos > 0) editor->cursorPos--;
}

void deleteLine(TextEditor *editor) {
    if (editor->totalLines <= 1) return;
    saveState(editor);
    if (editor->cursorLinePtr == editor->head) {
        LineNode *newHead = editor->head->next;
        delete editor->head;
        editor->head = newHead;
        if (editor->head) {
            editor->head->prev = nullptr;
            editor->cursorLinePtr = editor->head;
        }
    } else {
        LineNode *toDelete = editor->cursorLinePtr;
        toDelete->prev->next = toDelete->next;
        if (toDelete->next) toDelete->next->prev = toDelete->prev;
        else editor->tail = toDelete->prev;
        editor->cursorLinePtr = toDelete->prev;
        delete toDelete;
    }
    editor->totalLines--;
    if (editor->cursorPos > (int)editor->cursorLinePtr->text.length())
        editor->cursorPos = editor->cursorLinePtr->text.length();
}

void undo(TextEditor *editor) {
    if (editor->undoStack.size <= 1) {
        massage = "\033[41m Cannot undo!";
        return;
    }
    pushStack(&editor->redoStack, editor->head, editor->cursorLinePtr, editor->cursorPos, editor->totalLines);
    popStack(&editor->undoStack);
    StackNode *prevState = peekStack(&editor->undoStack);
    if (prevState) restoreState(editor, prevState);
}

void redo(TextEditor *editor) {
    if (isStackEmpty(&editor->redoStack)) {
        massage = "\033[41m Cannot redo!";
        return;
    }
    pushStack(&editor->undoStack, editor->head, editor->cursorLinePtr, editor->cursorPos, editor->totalLines);
    StackNode *redoState = peekStack(&editor->redoStack);
    if (redoState) {
        restoreState(editor, redoState);
        popStack(&editor->redoStack);
    }
}

void saveToFile(TextEditor *editor, const string &filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        massage = "\033[41m Error opening file!";
        return;
    }
    LineNode *current = editor->head;
    while (current) {
        file << current->text << endl;
        current = current->next;
    }
    file.close();
    massage = "\033[42m Saved: " + filename;
}

void loadFromFile(TextEditor *editor, const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        massage = "\033[41m Error opening file!" + filename;
        return;
    }
    saveState(editor);
    deleteAllLines(editor->head);
    string line;
    LineNode *prev = nullptr;
    editor->head = nullptr;
    editor->totalLines = 0;
    while (getline(file, line)) {
        LineNode *newNode = new LineNode;
        newNode->text = line;
        newNode->next = nullptr;
        newNode->prev = prev;
        if (!editor->head) editor->head = newNode;
        if (prev) prev->next = newNode;
        prev = newNode;
        editor->totalLines++;
    }
    if (!editor->head) {
        editor->head = new LineNode;
        editor->head->text = "";
        editor->tail = editor->head;
        editor->totalLines = 1;
    } else editor->tail = prev;
    editor->cursorLinePtr = editor->head;
    editor->cursorPos = 0;
    file.close();
    massage = "\033[42m Loaded: " + filename;
}

void moveCursor(TextEditor *editor, short direction) {
    switch (direction) {
        case 5:
            if (editor->cursorLinePtr->prev) {
                editor->cursorLinePtr = editor->cursorLinePtr->prev;
                if (editor->cursorPos > (int)editor->cursorLinePtr->text.length())
                    editor->cursorPos = editor->cursorLinePtr->text.length();
            }
            break;
        case 6:
            if (editor->cursorLinePtr->next) {
                editor->cursorLinePtr = editor->cursorLinePtr->next;
                if (editor->cursorPos > (int)editor->cursorLinePtr->text.length())
                    editor->cursorPos = editor->cursorLinePtr->text.length();
            }
            break;
        case 7: if (editor->cursorPos > 0) editor->cursorPos--; break;
        case 8: if (editor->cursorPos < (int)editor->cursorLinePtr->text.length()) editor->cursorPos++; break;
        case 11: editor->cursorPos = 0; break;
        case 12: editor->cursorPos = editor->cursorLinePtr->text.length(); break;
    }
}

void searchText(TextEditor *editor, const string &pattern) {
    ostringstream mass;
    LineNode *current = editor->head;
    int lineNum = 0, foundCount = 0;
    mass << YELLOW << "Search results for \"" << pattern << "\":" << RESET << endl;
    while (current) {
        size_t pos = current->text.find(pattern);
        while (pos != string::npos) {
            foundCount++;
            mass << GREEN << "\tLine " << lineNum + 1 << ", Position " << pos + 1 << RESET << endl;
            pos = current->text.find(pattern, pos + 1);
        }
        current = current->next;
        lineNum++;
    }
    if (foundCount == 0) mass << "\033[41m\tNot found: \"" << pattern << "\"" << RESET;
    else mass << "\t\033[42m\033[30m Found " << foundCount << " items";
    massage = mass.str();
}

void replaceText(TextEditor *editor, const string &oldText, const string &newText, bool replaceAll) {
    ostringstream mass;
    saveState(editor);
    int replacements = 0;
    LineNode *current = editor->head;
    while (current) {
        size_t pos = 0;
        while ((pos = current->text.find(oldText, pos)) != string::npos) {
            current->text.replace(pos, oldText.length(), newText);
            pos += newText.length();
            replacements++;
            if (!replaceAll) {
                mass << "\033[43m\033[30m Replaced 1 occurrence";
                massage = mass.str();
                return;
            }
        }
        current = current->next;
    }
    mass << "\033[42m\033[30m  Replaced " << replacements << " occurrence(s)";
    massage = mass.str();
}

void showStatistics(TextEditor *editor) {
    ostringstream mass;
    mass << YELLOW << "\n \t>>>>>>>  Text Statistics  >>>>>>>" << RESET;
    mass << GREEN << "\n\t\tLines: " << editor->totalLines << "\n\t\tWords: " << countWords(editor) << "\n\t\tCharacters: " << countChars(editor);
    mass << YELLOW << "\n \t>>----------------------------<<";
    massage = mass.str();
}

int chose() {
    int Choice;
    do {
        cout << "\n >>> ";
        cin >> Choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " \033[41\tmInvalid input!" << RESET << endl;
        } else if (Choice < 1 || Choice > 18) {
            cout << "\033[41\tmInvalid choice!" << RESET << endl;
            cin.clear();
        }
    } while (Choice < 1 || Choice > 18);
    return Choice;
}

int chose_2() {
    int Choice;
    do {
        cout << "\n >>> ";
        cin >> Choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " \033[41\tmInvalid input!" << RESET << endl;
        } else if (Choice < 1 || Choice > 3) {
            cout << "\033[41\tmInvalid choice!" << RESET << endl;
            cin.clear();
        }
    } while (Choice < 1 || Choice > 3);
    return Choice;
}
