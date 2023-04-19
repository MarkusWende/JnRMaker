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
 * @file project_manager.h
 * @brief This file contains the singleton project manager class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include <string>
#include <fstream>

// #include <cereal/archives/xml.hpp>
// #include <cereal/archives/json.hpp>
// #include <cereal/archives/binary.hpp>
// #include <cereal/types/vector.hpp>
// #include <cereal/cereal.hpp> // for defer
// #include <cereal/types/memory.hpp>
// #include <cereal/types/base_class.hpp>
// #include <cereal/types/map.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <nlohmann/json.hpp>

#include "time_helper.h"
#include "message_manager.h"

#define MAX_STORED_SAVE_FILES 20               /**< Defines the maximal number of save files to be stored. */


#ifdef __EMSCRIPTEN__
extern "C" {
    extern int getLocalTilemapFile();
    extern int saveJSONFile();
    extern int viewFullscreen();
}
#else
#include <nfd.h>
#endif

/**
 * @brief Enumeration for message types.
 */
enum class project_status_t {
  IDLE = 0,                          /**< Error message. */
  SAVE = 1,                          /**< Warning message. */
  LOAD = 2,                             /**< Info message. */
};

enum class save_file_t {
  JSON = 0,
  XML = 1
};

struct SaveFile {
	std::string json;
	std::string timeinfo;
    save_file_t type;

    SaveFile(std::string ts, const char* j)
    {
        

        std::stringstream msg;
        msg << j;
        json = msg.str().c_str();
        MessageManager::AddMessage(msg, message_t::INFO);
        //d.CopyFrom(newDoc, d.GetAllocator());
        timeinfo = ts;
        type = save_file_t::JSON;
    }
};

/**
 * @brief The sindgleton project manager class manages project related informations accross the application.
 */
class ProjectManager
{
public:
    static std::string name;
    static project_status_t status;
    static std::vector<std::shared_ptr<SaveFile>> SaveFiles;

    /**
	 * @brief Set the project name.
     * @param newName a string with the new project name.
	 * @return Void.
	*/
    static void SetName(std::string newName) { name = newName; };

    static std::string GetName() { return name; };

    static void SetStatus(project_status_t newStatus) { status = newStatus; };
    static project_status_t GetStatus() { return status; };
    static std::vector<std::shared_ptr<SaveFile>> GetSaveFiles() { return SaveFiles; };

    static void AddSaveObject(std::string key, std::string data);
    static void SaveCreate();
    static void SaveWrite();

private:
    ProjectManager() { };                             //!< constructor
    ~ProjectManager() { };                            //!< destructor
};

#endif  // PROJECT_MANAGER_H
