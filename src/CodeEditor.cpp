#include "CodeEditor.h"
#include "MainWindow.h"
#include "LineNumberArea.h"

#include <QPainter>
#include <QTextBlock>
#include <QStatusBar>
#include <QFileInfo>

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent),
      m_lineNumberArea(new LineNumberArea(this))
{
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

CodeEditor::~CodeEditor() {}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) && event->key() == Qt::Key_Left)
    {
        moveCursor(QTextCursor::WordLeft, QTextCursor::KeepAnchor);
        return;
    }
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Slash)
    {
        addComment();
        return;
    }

    if (mode == NORMAL)
    {
        switch (event->key())
        {
        case Qt::Key_I:
            mode = INSERT;
            emit statusMessageChanged("Insert mode activated");
            break;
        case Qt::Key_A:
            moveCursor(QTextCursor::Left);
            break;
        case Qt::Key_D:
            moveCursor(QTextCursor::Right);
            break;
        case Qt::Key_X:
            moveCursor(QTextCursor::Down);
            break;
        case Qt::Key_W:
            moveCursor(QTextCursor::Up);
            break;
        default:
            emit statusMessageChanged("Insert mode is not active. Press 'i' to enter insert mode.");
            break;
        }
    }
    else if (mode == INSERT)
    {
        if (event->key() == Qt::Key_Escape)
        {
            mode = NORMAL;
            emit statusMessageChanged("Normal mode activated. Press 'escape' to return to normal mode.");
        }
        else
        {
            QPlainTextEdit::keyPressEvent(event);
        }
    }
}

QString CodeEditor::getFileExtension()
{
    QString filePath = getCurrentFileName();
    if (!QFile::exists(filePath))
    {
        return QString();
    }

    // Extract the file extension from the file path
    return QFileInfo(filePath).suffix().toLower();
}

void CodeEditor::addLanguageSymbol(QTextCursor &cursor, const QString &commentSymbol)
{
    if (cursor.hasSelection())
    {
        commentSelection(cursor, commentSymbol);
    }
    else
    {
        commentLine(cursor, commentSymbol);
    }
}

// Comment/uncomment the selected text or the current line
void CodeEditor::commentSelection(QTextCursor &cursor, const QString &commentSymbol)
{
    int start = cursor.selectionStart();
    int end   = cursor.selectionEnd();

    cursor.setPosition(start);
    int startBlockNumber = cursor.blockNumber();
    cursor.setPosition(end);
    int endBlockNumber = cursor.blockNumber();

    cursor.setPosition(start);
    for (int i = startBlockNumber; i <= endBlockNumber; ++i)
    {
        cursor.movePosition(QTextCursor::StartOfLine);
        QString lineText = cursor.block().text();

        if (lineText.startsWith(commentSymbol))
        {
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 3);
            cursor.removeSelectedText();
        }
        else
        {
            cursor.insertText(commentSymbol + " ");
        }

        cursor.movePosition(QTextCursor::NextBlock);
    }
}

// Comment/uncomment the single current line
void CodeEditor::commentLine(QTextCursor &cursor, const QString &commentSymbol)
{
    cursor.select(QTextCursor::LineUnderCursor);
    QString lineText = cursor.selectedText();

    if (lineText.startsWith(commentSymbol))
    {
        lineText.remove(0, 3);
    }
    else
    {
        lineText.prepend(commentSymbol + " ");
    }

    cursor.insertText(lineText);
}

void CodeEditor::addComment()
{
    QTextCursor cursor    = textCursor();
    QString fileExtension = getFileExtension();
    qDebug() << "File Extension:" << fileExtension;

    if (fileExtension == "cpp" || fileExtension == "h" ||
        fileExtension == "hpp" || fileExtension == "c" ||
        fileExtension == "java" || fileExtension == "go" ||
        fileExtension == "json")
    {
        addLanguageSymbol(cursor, "//");
    }
    else if (fileExtension == "py" || fileExtension == "yaml" ||
             fileExtension == "yml" || fileExtension == "sh" ||
             fileExtension == "bash")
    {
        addLanguageSymbol(cursor, "#");
    }
    else if (fileExtension == "sql")
    {
        addLanguageSymbol(cursor, "--");
    }
    else
    {
        qDebug() << "Unsupported file extension for commenting.";
        return;
    }
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max    = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int padding = 15;
    int space   = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits + padding;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
    {
        m_lineNumberArea->scroll(0, dy);
    }
    else
    {
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::lightGray).lighter(60);
        lineColor.setAlpha(100);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);

        selection.cursor = textCursor();

        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);

    // Match the background color of the editor
    painter.fillRect(event->rect(), palette().color(QPalette::Base));

    // Draw a separating line between the number area and the text editor
    int separatorX = m_lineNumberArea->width() - 4;
    painter.drawLine(separatorX, event->rect().top(), separatorX, event->rect().bottom());

    QTextBlock block = firstVisibleBlock();
    int blockNumber  = block.blockNumber();
    int top          = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom       = top + qRound(blockBoundingRect(block).height());

    int lineHeight = fontMetrics().height();
    int padding    = (blockBoundingRect(block).height() - lineHeight) / 2;

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::darkGray);

            painter.drawText(0, top + padding, m_lineNumberArea->width(), lineHeight,
                             Qt::AlignCenter, number);
        }

        block  = block.next();
        top    = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
