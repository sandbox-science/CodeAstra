#include "../include/codeeditor.h"
#include "../include/mainwindow.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent) {}
CodeEditor::~CodeEditor() {}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    if (mode == NORMAL)
    {
        switch (event->key())
        {
        case Qt::Key_I:
            mode = INSERT;
            break;
        case Qt::Key_H:
            moveCursor(QTextCursor::Left);
            break;
        case Qt::Key_L:
            moveCursor(QTextCursor::Right);
            break;
        case Qt::Key_J:
            moveCursor(QTextCursor::Down);
            break;
        case Qt::Key_K:
            moveCursor(QTextCursor::Up);
            break;
        case Qt::Key_Escape:
            mode = NORMAL;
            break;
        }
    }
    else
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}
