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
#include "tilemap_manager.h"

// Instantiate static messages vector
std::string ProjectManager::name;
project_status_t ProjectManager::status;
std::vector<std::shared_ptr<SaveFile>> ProjectManager::SaveFiles;

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

EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::function("openTilemapFile", &openTilemapFile);
}
#endif

extern "C" {

const char* getJSON()
{
    auto ptrSaveFiles = ProjectManager::GetSaveFiles();
    auto saveFileJSON = ptrSaveFiles.front()->json;
    auto json_str = strdup(saveFileJSON.c_str());
    return json_str;
}

}

void ProjectManager::AddSaveObject(std::string key, std::string data)
{
    //auto ptrSaveFiles = ProjectManager::GetSaveFiles();
    //ptrSaveFiles.front()->d[key.c_str()] = data;

    //MessageManager::AddMessage(msg, message_t::INFO);
}

void ProjectManager::SaveCreate()
{
    nlohmann::json j = {
        {"name", "John"},
        {"age", 30},
        {"cars", nlohmann::json::array({"Ford", "BMW", "Fiat"})}
    };


    rapidjson::Document d;

    d.SetObject();
    auto& allocator = d.GetAllocator();

    auto gen = boost::uuids::random_generator();
    boost::uuids::uuid uniqueID = gen();
    rapidjson::Value id;
    std::string idStr = to_string(uniqueID);
    id.SetString(idStr.c_str(), idStr.size());
    d.AddMember("uuid", id, allocator);

    std::stringstream msg;
    msg << idStr;
    MessageManager::AddMessage(msg, message_t::INFO);

    std::string timeinfo = TimeHelper::GetTimeinfo();
    rapidjson::Value ts;
    ts.SetString(timeinfo.c_str(), timeinfo.length());
    d.AddMember("timestamp", ts, allocator);

    rapidjson::Value user;
    user.SetString("Testuser");
    d.AddMember("user", user, allocator);

    rapidjson::Value object;
    object.SetObject();
    rapidjson::Value currency;
    currency.SetString("USD");
    rapidjson::Value val;
    val.SetDouble(42.99);
    object.AddMember("currency", currency, allocator);
    object.AddMember("value", val, allocator);
    d.AddMember("object", object, allocator);

    rapidjson::Value answer;
    answer.SetObject();
    rapidjson::Value everything;
    everything.SetInt(42);
    answer.AddMember("everything", everything, allocator);
    d.AddMember("answer", answer, allocator);

    int matrix[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
    rapidjson::StringBuffer matrixBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> matrixWriter(matrixBuffer);
    matrixWriter.StartArray();
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            matrixWriter.Int(matrix[i][j]);
        }
    }
    matrixWriter.EndArray();

    rapidjson::Value tiles(matrixBuffer.GetString(), allocator);
    d.AddMember("tiles", tiles, allocator);

    // //d["object"] = { {"currency", "USD"}, {"value", 42.99} };
    // d["answer"]["everything"] = 42;

    if (!d.IsObject())
	{
        std::stringstream msg;
		msg << "JSON document creation failed.";
		MessageManager::AddMessage(msg, message_t::ERROR_T);
		return;
	}

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    SaveFiles.emplace(SaveFiles.begin(), new SaveFile(timeinfo, buffer.GetString()));

    if (SaveFiles.size() > MAX_STORED_SAVE_FILES)
    SaveFiles.pop_back();
}

void ProjectManager::SaveWrite()
{
	// auto ptrSaveFiles = ProjectManager::GetSaveFiles();
    // if (ptrSaveFiles.begin() == ptrSaveFiles.end())
    // {
    //     std::stringstream msg;
    //     msg << "Nothing to save..";
    //     MessageManager::AddMessage(msg, message_t::ERROR_T);
    //     return;
    // }

#ifdef __EMSCRIPTEN__
    saveJSONFile();
#else
    nfdchar_t *outPath;
    nfdfilteritem_t filterItem[2] = { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } };
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 2, NULL);

    std::stringstream msg;
    if (result == NFD_OKAY)
    {
        msg << "Success!";
        MessageManager::AddMessage(msg, message_t::INFO);
        msg << outPath;
        MessageManager::AddMessage(msg, message_t::INFO);
        NFD_FreePath(outPath);
    }
    else if (result == NFD_CANCEL)
    {
        msg << "User pressed cancel.";
        MessageManager::AddMessage(msg, message_t::WARNING);
    }
    else 
    {
        msg << "Error: " << NFD_GetError();
        MessageManager::AddMessage(msg, message_t::ERROR_T);
    }
#endif

    //SaveFiles.emplace(SaveFiles.begin(), new SaveFile(timeinfo, j));
}