// refreshable_panel.hpp
// Licensed under LGPLv2.1

#pragma once

namespace SierraEditor::UI {
    class RefreshablePanel {
        // Interface for refresh method
        public:
            virtual ~RefreshablePanel() = default;
            virtual void refresh() = 0;
    };
}