#include "../inc/Webserv.hpp"

int main(int argc, char **argv)
{
    bool keep_on = true;
    if (argc != 2)
    {
        std::cerr << "First argument have to be a full path to config file" << std::endl;
        return (EXIT_FAILURE);
    }
    
    std::string result;
    Webserv webserv(argv[1], result);
    if (!result.empty()) {
        std::cerr << "[Webserv] Error: " << result << std::endl;
        exit(EXIT_FAILURE);
    }

    while (keep_on)
    {
        try { 
            webserv.start();
            keep_on = false;
        }
        catch (const std::bad_alloc& ex) {
            webserv.sendErrMsg(std::string("MEMORY: ") + ex.what());
            webserv.removeAllClients();
        }
        catch (std::runtime_error & ex) {
            webserv.sendErrMsg(std::string("FATAL RUNTIME ") + ex.what());
            webserv.removeAllClients();
        }
        catch (std::exception &ex) {
            std::cerr << "STANDART EXCEPTION: " << ex.what() << '\n';
            exit(EXIT_FAILURE);
        }
    }
    
    return (EXIT_SUCCESS);
}