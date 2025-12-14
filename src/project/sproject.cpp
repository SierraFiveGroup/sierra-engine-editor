// sproject.cpp - .sierra project file wrapper
// Licensed under LGPLv2

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <sierra/project/sproject.hpp>

namespace SierraEditor::Project {
    SProject::SProject(const std::string& filepath)
        : mData{ .filePath = filepath }
    {}

    bool SProject::load() {
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

        xml_node<>* root = doc.first_node("SierraProject");
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

        xml_node<>* dirsNode = root->first_node("Directories");
        if (dirsNode) {
            xml_node<>* assetsNode = dirsNode->first_node("Assets");
            if (assetsNode) {
                mData.assetsPath = assetsNode->value();
            }
            xml_node<>* scriptsNode = dirsNode->first_node("Scripts");
            if (scriptsNode) {
                mData.scriptsPath = scriptsNode->value();
            }
        }

        xml_node<>* metadataNode = root->first_node("Metadata");
        if (metadataNode) {
            xml_node<>* createdNode = metadataNode->first_node("Created");
            if (createdNode) {
                mData.createdDate = createdNode->value();
            }
            xml_node<>* modifiedNode = metadataNode->first_node("Modified");
            if (modifiedNode) {
                mData.modifiedDate = modifiedNode->value();
            }
        }

        xml_node<>* preferencesNode = root->first_node("Preferences");
        if (preferencesNode) {
            xml_node<>* lastOpenedSceneNode = preferencesNode->first_node("LastOpenedScene");
            if (lastOpenedSceneNode) {
                mData.lastOpenedScene = lastOpenedSceneNode->value();
            }
        }

        return true;
    }

    bool SProject::save() {
        using namespace SierraEditor::IO;
        using namespace rapidxml;

        mData.modifiedDate = QDate::currentDate().toString(Qt::ISODate).toStdString();

        xml_document<> doc;

        xml_node<>* root = doc.allocate_node(node_type::node_element, "SierraProject");
        doc.append_node(root);

        xml_node<>* versionNode = doc.allocate_node(node_type::node_element, "Version", doc.allocate_string(std::to_string(mData.version).c_str()));
        root->append_node(versionNode);

        xml_node<>* nameNode = doc.allocate_node(node_type::node_element, "Name", doc.allocate_string(mData.name.c_str()));
        root->append_node(nameNode);

        xml_node<>* dirsNode = doc.allocate_node(node_type::node_element, "Directories");
        root->append_node(dirsNode);

        xml_node<>* assetsNode = doc.allocate_node(node_type::node_element, "Assets", doc.allocate_string(mData.assetsPath.c_str()));
        dirsNode->append_node(assetsNode);

        xml_node<>* scriptsNode = doc.allocate_node(node_type::node_element, "Scripts", doc.allocate_string(mData.scriptsPath.c_str()));
        dirsNode->append_node(scriptsNode);

        xml_node<>* metadataNode = doc.allocate_node(node_type::node_element, "Metadata");
        root->append_node(metadataNode);

        xml_node<>* createdNode = doc.allocate_node(node_type::node_element, "Created", doc.allocate_string(mData.createdDate.c_str()));
        metadataNode->append_node(createdNode);
        xml_node<>* modifiedNode = doc.allocate_node(node_type::node_element, "Modified", doc.allocate_string(mData.modifiedDate.c_str()));
        metadataNode->append_node(modifiedNode);

        xml_node<>* preferencesNode = doc.allocate_node(node_type::node_element, "Preferences");
        root->append_node(preferencesNode);
        xml_node<>* lastOpenedSceneNode = doc.allocate_node(node_type::node_element, "LastOpenedScene", doc.allocate_string(mData.lastOpenedScene.c_str()));
        preferencesNode->append_node(lastOpenedSceneNode);

        // Convert XML to string
        std::string xmlString;
        rapidxml::print(std::back_inserter(xmlString), doc, 0);

        // Write to file
        return writeFileLines(mData.filePath, { xmlString }, true);
    }
}