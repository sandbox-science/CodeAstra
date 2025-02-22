#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QKeyEvent>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);
    virtual ~CodeEditor();
    enum Mode
    {
        NORMAL,
        INSERT
    };
    Mode mode = NORMAL;

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // CODEEDITOR_H