// inspector_panel.cpp
// Licensed under LGPLv2.1

#include <sierra/ui/widgets/inspector_panel.hpp>

namespace SierraEditor::UI {
    InspectorPanel::InspectorPanel(QWidget* parent)
        : QWidget(parent)
    {
        mList = new QListWidget(this);

        // Placeholders
        mList->addItem("Transform");
        mList->addItem(" - Position: (0,0,0)");
        mList->addItem(" - Rotation: (0,0,0)");
        mList->addItem(" - Scale: (1,1,1)");
        mList->addItem("");
        mList->addItem("MeshRenderer");
        mList->addItem(" - Mesh: Cube");
        mList->addItem(" - Material: Default");

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(mList);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);
    }
}