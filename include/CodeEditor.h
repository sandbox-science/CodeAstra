#pragma once

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
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    QString getCurrentFileName() const { return currentFileName; }
    void setCurrentFileName(const QString &fileName) { currentFileName = fileName; }

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
    QString currentFileName;
    QString getFileExtension();
    void addLanguageSymbol(QTextCursor &cursor, const QString &commentSymbol);
    void commentSelection(QTextCursor &cursor, const QString &commentSymbol);
    void commentLine(QTextCursor &cursor, const QString &commentSymbol);
	void addComment();
};