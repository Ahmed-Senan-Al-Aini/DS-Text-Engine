#ifndef EDITOR_H
#define EDITOR_H

#include <string>
using namespace std;

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// ---------------------------
// Node for each line
// ---------------------------
struct LineNode
{
    string text;
    LineNode *next;
    LineNode *prev;
};

// ---------------------------
// Node for stack (editor state)
// ---------------------------
struct StackNode
{
    LineNode *editorHead;
    LineNode *cursorLinePtr;
    int cursorPos;
    int totalLines;
    StackNode *next;
};

// ---------------------------
// Stack using linked list
// ---------------------------
struct LinkedListStack
{
    StackNode *top;
    int size;
};

// ---------------------------
// Text Editor structure
// ---------------------------
struct TextEditor
{
    LineNode *head;
    LineNode *tail;
    LineNode *cursorLinePtr;
    int cursorPos;
    int totalLines;
    LinkedListStack undoStack;
    LinkedListStack redoStack;
};

// ---------------------------
// Varible globle declarations
// ---------------------------
extern string massage;

// ---------------------------
// Function declarations
// ---------------------------
void deleteAllLines(LineNode *head);

LineNode* copyLines(LineNode *sourceHead, LineNode *sourceCursor, LineNode **destCursorPtr);

StackNode* createStackNode(LineNode* head, LineNode* cursor, int pos, int totalLines);

void pushStack(LinkedListStack* stack, LineNode* head, LineNode* cursor, int pos, int totalLines);

bool popStack(LinkedListStack* stack);

StackNode* peekStack(LinkedListStack* stack);

bool isStackEmpty(LinkedListStack* stack);

void clearStack(LinkedListStack* stack);

void initStack(LinkedListStack* stack);

void initEditor(TextEditor* editor);

void saveState(TextEditor* editor);

void restoreState(TextEditor* editor, StackNode* state);

void menu();
void displayContent(TextEditor* editor, string file);

void insertText(TextEditor* editor, const string& text);

void insertNewLine(TextEditor* editor);

void deleteChar(TextEditor* editor);

void deleteLine(TextEditor* editor);

void undo(TextEditor* editor);

void redo(TextEditor* editor);

void saveToFile(TextEditor* editor, const string& filename);

void loadFromFile(TextEditor* editor, const string& filename);

void moveCursor(TextEditor* editor, short direction);

void searchText(TextEditor* editor, const string& pattern);

void replaceText(TextEditor* editor, const string& oldText, const string& newText, bool replaceAll=false);

int countWords(TextEditor* editor);

int countChars(TextEditor* editor);

void showStatistics(TextEditor* editor);

int chose();
int chose_2();

// ---------------------------
// Functions implementation
// ---------------------------
#endif
