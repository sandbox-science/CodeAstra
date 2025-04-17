#pragma once

#include <QPlainTextEdit>
#include <QKeyEvent>

class FileManager; // Forward declaration

/**
 * @class CodeEditor
 * @brief A custom code editor widget that extends QPlainTextEdit.
 *
 * The CodeEditor class provides a code editor with line number area, syntax highlighting,
 * and basic editing modes (NORMAL and INSERT). It emits signals for status messages and
 * handles key press and resize events.
 */
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
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void autoIndentation();

signals:
    void statusMessageChanged(const QString &message);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *m_lineNumberArea;
    FileManager *m_fileManager;

    void addLanguageSymbol(QTextCursor &cursor, const QString &commentSymbol);
    void commentSelection(QTextCursor &cursor, const QString &commentSymbol);
    void commentLine(QTextCursor &cursor, const QString &commentSymbol);
    void addComment();
};