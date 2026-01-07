// inspector_panel.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <sierra/stateful/selection_manager.hpp>
#include <sierra/ui/widgets/refreshable_panel.hpp>
#include <sierra/logger.hpp>

namespace SierraEditor::UI {
    class InspectorPanel : public QWidget, public RefreshablePanel {
        Q_OBJECT
        public:
            explicit InspectorPanel(QWidget* parent = nullptr);
            void refresh() override;

        private:
            QListWidget* mList;
    };
}