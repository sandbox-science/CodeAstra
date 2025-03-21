#include "CodeEditor.h"
#include "MainWindow.h"
#include "LineNumberArea.h"

#include <QPainter>
#include <QTextBlock>
#include <QStatusBar>

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
