#pragma once

#include "CodeEditor.h"

#include <QWidget>
#include <QPaintEvent>
#include <QSize>

/**
 * @class LineNumberArea
 * @brief A widget that displays line numbers for the CodeEditor.
 *
 * The LineNumberArea class is a QWidget that is used to display line numbers
 * alongside the CodeEditor widget.
 *
 * @note This class is intended to be used as a part of the CodeEditor widget.
 */
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor) {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};