// profiler_panel.cpp
// Licensed under LGPLv2.1

#include <sierra/ui/widgets/profiler_panel.hpp>

namespace SierraEditor::UI {
    ProfilerPanel::ProfilerPanel(QWidget* parent)
        : QWidget(parent),
          mLayout(new QVBoxLayout(this))
    {
        QLabel* title = new QLabel("Profiler Panel", this);
        title->setAlignment(Qt::AlignCenter);
        mLayout->addWidget(title);
    }
}