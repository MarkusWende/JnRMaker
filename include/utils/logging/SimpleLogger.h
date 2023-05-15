#include "ILogger.h"

class SimpleLogger : public ILogger {
public:
    void Log(const std::string& message) override;

    void Print() const override;

private:
    std::vector<std::string> logs_;
};