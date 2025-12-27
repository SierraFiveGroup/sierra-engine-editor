// sscene.cpp
// Licensed under LGPLv2.1

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <sierra/project/sscene.hpp>

namespace SierraEditor::Project {
    SScene::SScene(const std::string& filepath) {
        mData.filePath = filepath;
    }

    bool SScene::load() {
        using namespace SierraEditor::IO;
        using namespace rapidxml;

        std::vector<std::string> lines = readFileLines(mData.filePath);
        if (lines.empty()) {
            return false;
        }

        std::string content;
        for (const auto& line : lines) {
            content += line + "\n";
        }

        // Parse XML
        xml_document<> doc;
        try {
            doc.parse<0>(&content[0]);
        } catch (const parse_error& e) {
            ERROR("XML Parse Error: " << e.what());
            return false;
        }

        xml_node<>* root = doc.first_node("Scene");
        if (!root) {
            return false;
        }

        xml_node<>* versionNode = root->first_node("Version");
        if (versionNode) {
            mData.version = static_cast<uint16_t>(std::stoi(versionNode->value()));
        }

        xml_node<>* nameNode = root->first_node("Name");
        if (nameNode) {
            mData.name = nameNode->value();
        }

        // Find Entities node and parse entities into mData.entities - TODO

        return true;
    }

    bool SScene::save() {
        using namespace SierraEditor::IO;
        using namespace rapidxml;

        xml_document<> doc;

        xml_node<>* root = doc.allocate_node(node_type::node_element, "Scene");
        doc.append_node(root);

        xml_node<>* versionNode = doc.allocate_node(node_type::node_element, "Version", doc.allocate_string(std::to_string(mData.version).c_str()));
        root->append_node(versionNode);

        xml_node<>* nameNode = doc.allocate_node(node_type::node_element, "Name", doc.allocate_string(mData.name.c_str()));
        root->append_node(nameNode);

        // Create Entities node and serialize mData.entities into it - TODO

        // Convert XML to string
        std::string xmlString;
        rapidxml::print(std::back_inserter(xmlString), doc, 0);

        // Write to file
        std::vector<std::string> lines = { xmlString };
        bool res = writeFileLines(mData.filePath, lines);
        
        return res;
    }
}