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

#include "ProjectManager.h"
#include "TilemapManager.h"

// Instantiate static messages vector
std::string ProjectManager::name;
project_status_t ProjectManager::status;
std::vector<std::shared_ptr<SaveFile>> ProjectManager::SaveFiles;

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
    (void)key;
    (void)data;
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

    float array[512]; // Your large array of floats

    // Populate the array with sample data
    for (int i = 0; i < 512; ++i)
    {
        array[i] = i * 0.5f;
    }

    rapidjson::StringBuffer arrayBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> arrayWriter(arrayBuffer);

    arrayWriter.StartArray();

    for (int i = 0; i < 512; ++i)
    {
        arrayWriter.Double(array[i]);
        // Check if 12 values have been written, then append a newline
        if ((i + 1) % 12 == 0)
        {
            arrayBuffer.Put('\n');
        }
    }

    arrayWriter.EndArray();

    rapidjson::Value tiles(rapidjson::kArrayType);
    auto arrayString = arrayBuffer.GetString();
    tiles.SetString(arrayString, arrayBuffer.GetSize(), allocator);
    d.AddMember("tiles", tiles, allocator);

    // //d["object"] = { {"currency", "USD"}, {"value", 42.99} };
    // d["answer"]["everything"] = 42;

    if (!d.IsObject())
	{
        std::stringstream msg;
		msg << "JSON document creation failed.";
		// MessageManager::AddMessage(msg, message_t::ERROR_T);
		return;
	}

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
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
        // MessageManager::AddMessage(msg, message_t::INFO);
        msg << outPath;
        // MessageManager::AddMessage(msg, message_t::INFO);
        NFD_FreePath(outPath);
    }
    else if (result == NFD_CANCEL)
    {
        msg << "User pressed cancel.";
        // MessageManager::AddMessage(msg, message_t::WARNING);
    }
    else 
    {
        msg << "Error: " << NFD_GetError();
        // MessageManager::AddMessage(msg, message_t::ERROR_T);
    }
#endif

    //SaveFiles.emplace(SaveFiles.begin(), new SaveFile(timeinfo, j));
}