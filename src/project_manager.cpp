/**
 * JnRMaker - A tile editor and jump and run game maker
 * See COPYRIGHT file at the top of the source tree.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the JnRMaker License Statement and
 * the GNU General Public License along with this program.
 *
 */

/**
 * @file project_manager.cpp
 * @brief This file contains the singleton project manager class implementation.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#include "project_manager.h"
#include "message_manager.h"
#include "tilemap_manager.h"

// Instantiate static messages vector
std::string ProjectManager::name;
project_status_t ProjectManager::status;
std::vector<SaveFile> ProjectManager::SaveFiles;

#ifdef __EMSCRIPTEN__
template<typename T, size_t sizeOfArray>
constexpr size_t getElementCount(T (&)[sizeOfArray]) {
    return sizeOfArray;
}

void openTilemapFile(std::string const& name, std::string const& type, emscripten::val data, int dataSize)
{
    std::stringstream msg;

    if ((type.compare("image/jpeg") == 0) || type.compare("image/png") == 0)
    {
        //ResourceManager::CreateTexture(rawData, dataSize, "loadedImage");
		TimeHelper::tic();
		std::string dataStr = data.as<std::string>();
		auto chrs = dataStr.c_str();
		auto uchrs = reinterpret_cast<unsigned char*>(const_cast<char*>(chrs));
        //unsigned char* rawData = reinterpret_cast<unsigned char*>(&dataStr);
        // const std::vector<unsigned char> rawArray = emscripten::vecFromJSArray<unsigned char>(data);
        // const unsigned char* rawData = rawArray.data();
        msg << "emscripten::vecFromJSArray" << TimeHelper::toc(1);
        MessageManager::AddMessage(msg, message_t::DEBUG);
        TimeHelper::tic();
		TilemapManager::Add(name, { 16.0f, 16.0f }, { 1.0f, 1.0f }, uchrs, dataSize);
        msg << "ResourceManager::CreateTexture(): " << TimeHelper::toc(1);
        MessageManager::AddMessage(msg, message_t::DEBUG);
    }
    else
    {
        msg << name << " is not a supported format.";
        MessageManager::AddMessage(msg, message_t::ERROR_T);
        getLocalTilemapFile();
        return;
    }
    
    msg << "Loaded: " << name << "\tType: " << type << "\tSize: " << ((float)dataSize/1024.0f) << "KB\n";
    MessageManager::AddMessage(msg, message_t::DEBUG);
}

emscripten::val getBytes()
{
    // auto text = R"(
    // {
    //     "hasdappy": true,
    //     "pi": 3.141
    // }
    // )";
    // create an empty structure (null)
    nlohmann::json j;

    // add a number that is stored as double (note the implicit conversion of j to an object)
    j["pi"] = 3.141;

    // add a Boolean that is stored as bool
    j["happy"] = true;

    // add a string that is stored as std::string
    j["name"] = "Niels";

    // add another null object by passing nullptr
    j["nothing"] = nullptr;

    // add an object inside the object
    j["answer"]["everything"] = 42;

    // add an array that is stored as std::vector (using an initializer list)
    j["list"] = { 1, 0, 2 };

    // add another object (using an initializer list of pairs)
    j["object"] = { {"currency", "USD"}, {"value", 42.99} };
    const auto& tmp = j.dump(2);
    std::basic_string<unsigned char> str(tmp.data(), std::next(tmp.data(), tmp.size()));
    return emscripten::val(emscripten::typed_memory_view(str.size(), str.data()));
}

EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::function("getBytes", &getBytes);
    emscripten::function("openTilemapFile", &openTilemapFile);
}
#endif

SaveFile ProjectManager::AddSaveFile(std::stringstream& data, save_file_t type)
{
    SaveFile newSaveFile;

	newSaveFile.data = data.str();
	newSaveFile.timeinfo = TimeHelper::GetTimeinfo();
	newSaveFile.type = type;

    std::stringstream msg;
    msg << data.str();
    MessageManager::AddMessage(msg, message_t::DEBUG);

	SaveFiles.insert(SaveFiles.begin(), newSaveFile);

	if (SaveFiles.size() > MAX_STORED_SAVE_FILES)
	    SaveFiles.pop_back();

	return newSaveFile;
}

void ProjectManager::Save()
{
    std::vector<SaveFile>* ptrSaveFiles;
	ptrSaveFiles = ProjectManager::GetSaveFiles();
    if (ptrSaveFiles->begin() == ptrSaveFiles->end())
    {
        std::stringstream msg;
        msg << "No save file.";
        MessageManager::AddMessage(msg, message_t::ERROR_T);
        return;
    }
    std::string saveFileJSON = ptrSaveFiles->back().data;
}