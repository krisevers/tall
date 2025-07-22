#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <ctime>
#include <filesystem>
#include <mutex>
#include <unordered_map>

namespace tall
{
    namespace log {
        const std::string LOG_DIR = "logs";

        using logging_config_t = std::unordered_map<std::string, std::string>;

        class logger
        {
            public:
                enum LogLevel
                {
                    INFO,
                    WARNING,
                    ERROR,
                    DEBUG
                };

                logger() = delete;
                logger(const logging_config_t &config) {
                };
                virtual ~logger() {};
                virtual void log(const std::string& message, LogLevel level) {};
                virtual void log(const std::string& message) {};

                std::string getTimestamp()
                {
                    time_t now = time(0);
                    tm *ltm = localtime(&now);

                    std::string timestamp = std::to_string(1900 + ltm->tm_year) + "-" + std::to_string(1 + ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday) + " " + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);

                    return timestamp;
                }

                std::string levelToString(LogLevel level)
                {
                    switch (level)
                    {
                        case INFO:
                            return "INFO";
                        case WARNING:
                            return "WARNING";
                        case ERROR:
                            return "ERROR";
                        case DEBUG:
                            return "DEBUG";
                        default:
                            return "UNKNOWN";
                    }
                }
            
            protected:
                std::mutex lock;

        };

        // logger that writes to the console
        class outLogger : public logger
        {
            public:
                outLogger() = delete;
                outLogger(const logging_config_t &config) : logger(config) {};

                virtual void log(const std::string& message, const LogLevel level) 
                {
                    std::string output;
                    output += levelToString(level);
                    output += " | ";
                    output += message;
                    log(output);
                };

                virtual void log(const std::string& message)
                {
                    std::lock_guard<std::mutex> guard(lock);
                    std::cout << message;
                    std::cout << "\n";
                }

            protected:
        };

        // logger that writes to a file
        class fileLogger : public logger
        {
            public:
                fileLogger() = delete;
                fileLogger(const logging_config_t &config) : logger(config) 
                {
                    auto name = config.find("filename");
                    if (name == config.end())
                    {
                        throw std::runtime_error("No filename provided for FileLogger.");
                    }
                    if (!std::filesystem::exists(LOG_DIR))
                    {
                        std::filesystem::create_directory(LOG_DIR);
                    }
                    filename = name->second;
                    filename = LOG_DIR + "/" + filename;
                    std::string timestamp = getTimestamp();
                    filename += "_" + timestamp + ".log";

                    auto interval = config.find("reopen_interval");
                    if(interval != config.end())
                    {
                        try {
                        reopen_interval = std::chrono::seconds(std::stoul(interval->second));
                        }
                        catch(...) {
                        throw std::runtime_error(interval->second + " is not a valid reopen interval");
                        }
                    }

                    reopen();

                };

                virtual void log(const std::string& message, const LogLevel level)
                {
                    std::string output;
                    output += getTimestamp();
                    output += " | ";
                    output += levelToString(level);
                    output += " | ";
                    output += message;

                    log(output);
                }

                virtual void log(const std::string& message)
                {
                    std::lock_guard<std::mutex> guard(lock);
                    file << message;
                    file << "\n";
                    file.flush();
                    reopen();
                }

            protected:
                std::string filename;
                std::ofstream file;
                std::chrono::seconds reopen_interval;
                std::chrono::system_clock::time_point last_reopen;

                void reopen()
                {
                    auto now = std::chrono::system_clock::now();
                    if (now - last_reopen > reopen_interval)
                    {
                        last_reopen = now;
                        try { file.close(); } catch (...) {}
                        try { 
                            file.open(filename, std::ofstream::out | std::ios::app);
                            last_reopen = now;
                        }
                        catch(std::exception &e)
                        {
                            try { file.close(); } catch (...) {}
                            throw e;
                        }
                    }
                }
        };

        using logger_creator = logger *(*)(const logging_config_t &);
        class loggerFactory
        {
            public:
                loggerFactory()
                {
                    creators.emplace("", [](const logging_config_t &config)->logger* { return new logger(config); });
                    creators.emplace("out", [](const logging_config_t &config)->logger* { return new outLogger(config); });
                    creators.emplace("file", [](const logging_config_t &config)->logger* { return new fileLogger(config); });
                }
                logger* produce(const logging_config_t &config)
                {
                    auto type = config.find("type");
                    if (type == config.end())
                    {
                        throw std::runtime_error("No type provided for logger.");
                    }
                    auto creator = creators.find(type->second);
                    if (creator == creators.end())
                    {
                        throw std::runtime_error("Unknown logger type.");
                    }
                    return creator->second(config);
                }

            protected:
                std::unordered_map<std::string, logger_creator> creators;
        };

        // statically get a factory
        inline loggerFactory &getLoggerFactory()
        {
            static loggerFactory factory;
            return factory;
        };

        // get a singleton
        inline logger &getLogger(const logging_config_t &config = { {"type", "out"} })
        {
            static std::unique_ptr<logger> singleton(getLoggerFactory().produce(config));
            return *singleton;
        };

        // configure the singleton
        inline void configure(const logging_config_t &config)
        {
            getLogger(config);
        };

        // statically log manually with level
        inline void log(const std::string &message, logger::LogLevel level)
        {
            getLogger().log(message, level);
        };

        // statically log manually without level
        inline void log(const std::string &message)
        {
            getLogger().log(message);
        };

        inline void INFO(const std::string &message)
        {
            getLogger().log(message, logger::INFO);
        };

        inline void WARNING(const std::string &message)
        {
            getLogger().log(message, logger::WARNING);
        };

        inline void ERROR(const std::string &message)
        {
            getLogger().log(message, logger::ERROR);
        };

        inline void DEBUG(const std::string &message)
        {
            getLogger().log(message, logger::DEBUG);
        };
    }
}

#endif // LOGGER_H

#ifdef TEST_LOGGING

#include <thread>
#include <vector>

void work()
{
    std::ostringstream oss; oss << "Thread " << std::this_thread::get_id() << " started.";
    std::string id = oss.str();

    for (int i = 0; i < 10; i++)
    {
        tall::log::INFO(id + " working...");
        tall::log::WARNING(id + " warning...");
        tall::log::ERROR(id + " error...");
        tall::log::DEBUG(id + " debug...");
    }

}

int main()
{
   
    // tall::log::configure({{"type", "file"}, {"filename", "test"}});

    std::vector<std::shared_ptr<std::thread>> threads(std::thread::hardware_concurrency());
    for (auto &thread : threads)
    {
        thread.reset(new std::thread(work));
    }

    for (auto &thread : threads)
    {
        thread->join();
    }


    return 0;
}

#endif // TEST_LOGGING