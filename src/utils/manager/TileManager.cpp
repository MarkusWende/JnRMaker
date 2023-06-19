#include "TileManager.h"

#ifdef __EMSCRIPTEN__
extern "C" {
    extern int getLocalTilemapFile(TileManager* tileManagerInstance);
}
#endif

TileManager::TileManager(std::shared_ptr<ILogger> logger)
{
    ui_logger_ = std::dynamic_pointer_cast<UILogger>(logger);
}

void
TileManager::Load(const char* name, const char* tilemapFIle, glm::vec2 size, glm::vec2 scale)
{
    ui_logger_->Log(log_t::DEBUG, "Loading tilemap: %s", name);
    if (tilemaps_.find(name) == tilemaps_.end())
    {
        tilemaps_[name] = std::make_shared<Tilemap>(name, size, scale, tilemapFIle);
        active_tilemap_ = name;
    }
    else
    {
        std::stringstream msg;
        msg << "Tilemap with this name: " << name << " already exist.";
        ui_logger_->Log(log_t::WARNING, "%s", msg.str().c_str());
    }
}

void
TileManager::Unload(const char* name)
{
    ui_logger_->Log(log_t::DEBUG, "Unloading tilemap: %s", name);
}

std::shared_ptr<Tilemap>
TileManager::Get(const char* name)
{
    // Implementation to retrieve the tilemap_ by name
    auto it = tilemaps_.find(name);
    if (it != tilemaps_.end()) {
        return it->second;
    }
    return nullptr; // or handle the case when tilemap_ is not found
}

std::shared_ptr<std::vector<std::string>>
TileManager::GetKeys()
{
    std::vector<std::string> keys;
    for (const auto& pair : tilemaps_)
    {
        keys.push_back(pair.first);
    }
    return std::make_shared<std::vector<std::string>>(keys);
}

void msgErr(std::string const& msg)
{
	std::stringstream ss;
	ss << msg;
	//MessageManager::AddMessage(ss, message_t::ERROR_T);
}

void msgDebug(std::string const& msg)
{
    std::stringstream ss;
    ss << msg;
    //MessageManager::AddMessage(ss, message_t::DEBUG);
}

#ifdef __EMSCRIPTEN__
void TileManager::LoadTilemap(std::string const& name, std::string const& type, emscripten::val data, int dataSize)
#else
void TileManager::LoadTilemap(std::string const& name, std::string const& type, const char* data, int dataSize)
#endif
{
    std::stringstream msg;

    if ((type.compare("image/jpeg") == 0) || type.compare("image/png") == 0)
    {
        //ResourceManager::CreateTexture(rawData, dataSize, "loadedImage");
		TimeHelper::tic();
		std::string dataStr;
#ifdef __EMSCRIPTEN__
        dataStr = data.as<std::string>();
#else

#endif
		auto chrs = dataStr.c_str();
		auto uchrs = reinterpret_cast<unsigned char*>(const_cast<char*>(chrs));
        //unsigned char* rawData = reinterpret_cast<unsigned char*>(&dataStr);
        // const std::vector<unsigned char> rawArray = emscripten::vecFromJSArray<unsigned char>(data);
        // const unsigned char* rawData = rawArray.data();
        msg << "emscripten::vecFromJSArray" << TimeHelper::toc(1);
        // MessageManager::AddMessage(msg, message_t::INFO);
        TimeHelper::tic();
		//TilemapManager::Add(name, { 16.0f, 16.0f }, { 1.0f, 1.0f }, uchrs, dataSize);
        msg << "ResourceManager::CreateTexture(): " << TimeHelper::toc(1);
        // MessageManager::AddMessage(msg, message_t::INFO);
    }
    else
    {
        msg << name << " is not a supported format.";
        // MessageManager::AddMessage(msg, message_t::ERROR_T);
#ifdef __EMSCRIPTEN__
        TileManager* instance = this;
        getLocalTilemapFile(instance);
#else

#endif
        return;
    }
    
    msg << "Loaded: " << name << "\tType: " << type << "\tSize: " << ((float)dataSize/1024.0f) << "KB\n";
    // MessageManager::AddMessage(msg, message_t::DEBUG);
}
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::class_<TileManager>("TileManager")
        .function("LoadTilemap", &TileManager::LoadTilemap)
        ;

    emscripten::function("msgErr", &msgErr);
    emscripten::function("msgDebug", &msgDebug);
}
#endif

void
TileManager::Load()
{
#ifdef __EMSCRIPTEN__
    TileManager* instance = this;
    getLocalTilemapFile(instance);
#else
	nfdchar_t *outPath;
    nfdfilteritem_t filterItem[1] = { { "Tilemap", "png" } };
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);

    std::stringstream msg;
    if (result == NFD_OKAY)
    {
        //TilemapManager::Add(outPath, { 16.0f, 16.0f }, { 1.0f, 1.0f }, outPath);
        auto name = std::string(outPath);
        Load(name.c_str(), name.c_str(), { 16.0f, 16.0f }, { 1.0f, 1.0f });
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
}