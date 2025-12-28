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

        // Parse entities
        xml_node<>* entitiesNode = root->first_node("Entities");
        if (entitiesNode) {
            for (xml_node<>* entityNode = entitiesNode->first_node("Entity"); entityNode; entityNode = entityNode->next_sibling("Entity")) {
                // For now just read the entity's name and active status
                std::shared_ptr<SEntity> entity = std::make_shared<SEntity>();
                xml_node<>* entityNameNode = entityNode->first_node("Name");
                if (entityNameNode) {
                    entity->setName(entityNameNode->value());
                }
                xml_node<>* entityActiveNode = entityNode->first_node("Active");
                if (entityActiveNode) {
                    entity->setActive(std::string(entityActiveNode->value()) == "true");
                }
                mData.entities.insert(entity);
            }
        }

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

        // Parse entities
        xml_node<>* entitiesNode = doc.allocate_node(node_type::node_element, "Entities");
        root->append_node(entitiesNode);
        for (const auto& entity : mData.entities) {
            xml_node<>* entityNode = doc.allocate_node(node_type::node_element, "Entity");
            entitiesNode->append_node(entityNode);
            xml_node<>* entityNameNode = doc.allocate_node(node_type::node_element, "Name", doc.allocate_string(entity->getName().c_str()));
            entityNode->append_node(entityNameNode);
            xml_node<>* entityActiveNode = doc.allocate_node(node_type::node_element, "Active", doc.allocate_string(entity->isActive() ? "true" : "false"));
            entityNode->append_node(entityActiveNode);
        }

        // Convert XML to string
        std::string xmlString;
        rapidxml::print(std::back_inserter(xmlString), doc, 0);

        // Write to file
        std::vector<std::string> lines = { xmlString };
        bool res = writeFileLines(mData.filePath, lines);
        
        return res;
    }

    void SScene::addEntity(std::shared_ptr<SEntity> entity) {
        mData.entities.insert(entity);
    }

    void SScene::removeEntity(std::shared_ptr<SEntity> entity) {
        mData.entities.erase(entity);
    }

    std::shared_ptr<SEntity> SScene::getEntityByName(const std::string& name) const {
        for (const auto& entity : mData.entities) {
            if (entity->getName() == name) {
                return entity;
            }
        }
        return nullptr;
    }
}