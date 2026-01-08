// inspector_panel.cpp
// Licensed under LGPLv2.1

#include <sierra/ui/widgets/inspector_panel.hpp>

namespace SierraEditor::UI {
    InspectorPanel::InspectorPanel(QWidget* parent)
        : QWidget(parent)
    {
        mList = new QListWidget(this);

        Project::SEntity* selectedEntity = Stateful::SelectionManager::getInstance().getSelectedEntity();
        if (selectedEntity) {
            mList->addItem(QString::fromStdString("Selected Entity: " + selectedEntity->getName()));
            mList->addItem(QString::fromStdString("Active: " + std::string(selectedEntity->isActive() ? "Yes" : "No")));
            mList->addItem("Components:");
            // List components
            for (const std::string& comp : selectedEntity->getComponents()) {
                mList->addItem(QString::fromStdString(" - " + comp));
            }
        } else {
            mList->addItem("No entity selected.");
        }

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(mList);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);
    }

    void InspectorPanel::refresh() {
        // TODO
        DBG("InspectorPanel::refresh() called");
        return;
    }
}