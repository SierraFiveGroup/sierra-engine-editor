// console_output.cpp
// Licensed under LGPLv2.1

#include <sierra/ui/widgets/console_output.hpp>

#include <QTextEdit>
#include <QVBoxLayout>
#include <QTimer>
#include <QFont>
#include <QFontDatabase>
#include <QTextCursor>
#include <QtGlobal>
#include <QString>
#include <QScrollBar>

namespace SierraEditor::UI {

    ConsoleOutputWidget::ConsoleOutputWidget(QWidget* parent)
        : QWidget(parent)
    {
        mTextEdit = new QTextEdit(this);
        mTextEdit->setReadOnly(true);
        mTextEdit->setLineWrapMode(QTextEdit::NoWrap);
        mTextEdit->setUndoRedoEnabled(false);
        mTextEdit->setAcceptRichText(true);

        // Use system monospace font (Menlo on macOS, Courier New on Windows, etc.)
        QFont monoFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        monoFont.setPointSize(11);
        mTextEdit->setFont(monoFont);

        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(mTextEdit);

        mTimer = new QTimer(this);
        mTimer->setInterval(250);
        connect(mTimer, &QTimer::timeout, this, &ConsoleOutputWidget::refreshFromLogger);
        mTimer->start();

        refreshFromLogger();
    }

    QString ConsoleOutputWidget::levelLabel(SierraEditor::LogEntry::Level level) const
    {
        switch (level) {
            case SierraEditor::LogEntry::Level::INFO: return "LOG";
            case SierraEditor::LogEntry::Level::WARNING: return "WARN";
            case SierraEditor::LogEntry::Level::ERROR: return "ERROR";
            case SierraEditor::LogEntry::Level::DEBUG: return "DEBUG";
            case SierraEditor::LogEntry::Level::TODO: return "TODO";
        }
        return "LOG";
    }

    QString ConsoleOutputWidget::levelColor(SierraEditor::LogEntry::Level level) const
    {
        switch (level) {
            case SierraEditor::LogEntry::Level::INFO: return "#e0e0e0";
            case SierraEditor::LogEntry::Level::WARNING: return "#d0b200";
            case SierraEditor::LogEntry::Level::ERROR: return "#d32f2f";
            case SierraEditor::LogEntry::Level::DEBUG: return "#b388ff";
            case SierraEditor::LogEntry::Level::TODO: return "#2e7d32";
        }
        return "#e0e0e0";
    }

    QString ConsoleOutputWidget::formatEntry(const SierraEditor::LogEntry& entry) const
    {
        QString level = levelLabel(entry.level);
        QString color = levelColor(entry.level);
        QString message = Qt::convertFromPlainText(QString::fromStdString(entry.message));

        QString location;
        if (!entry.fileName.empty()) {
            location = QString::fromStdString(entry.fileName);
            if (entry.lineNumber > 0) {
                location += QString(":%1").arg(entry.lineNumber);
            }
        }

        QString function;
        if (!entry.functionName.empty()) {
            function = QString::fromStdString(entry.functionName);
        }

        QString prefix;
        if (!function.isEmpty()) {
            prefix += function;
        }
        if (!location.isEmpty()) {
            if (!prefix.isEmpty()) prefix += " ";
            prefix += "(" + location + ")";
        }
        if (!prefix.isEmpty()) {
            prefix += " - ";
        }

        return QString("<div style=\"color:%1;\"><b>[%2]</b> %3%4</div>")
            .arg(color, level, prefix, message);
    }

    void ConsoleOutputWidget::refreshFromLogger()
    {
        const auto& entries = SierraEditor::Logger::getConsoleEntries();
        if (entries.size() == mLastCount)
            return;

        QString html;
        html.reserve(static_cast<int>(entries.size() * 64));

        for (const auto& entry : entries) {
            html += formatEntry(entry);
        }

        mTextEdit->setHtml(html);
        mTextEdit->moveCursor(QTextCursor::End);
        mTextEdit->verticalScrollBar()->setValue(mTextEdit->verticalScrollBar()->maximum());

        mLastCount = entries.size();
    }
}
