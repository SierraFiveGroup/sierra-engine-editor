// console_output.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QWidget>

#include <sierra/logger.hpp>

class QTextEdit;
class QTimer;

namespace SierraEditor::UI {
    class ConsoleOutputWidget : public QWidget {
        Q_OBJECT

    public:
        explicit ConsoleOutputWidget(QWidget* parent = nullptr);

    private slots:
        void refreshFromLogger();

    private:
        QString levelLabel(SierraEditor::LogEntry::Level level) const;
        QString levelColor(SierraEditor::LogEntry::Level level) const;
        QString formatEntry(const SierraEditor::LogEntry& entry) const;

        QTextEdit* mTextEdit = nullptr;
        QTimer* mTimer = nullptr;
        std::size_t mLastCount = 0;
    };
}
