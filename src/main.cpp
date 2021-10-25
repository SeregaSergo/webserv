#include "../inc/webserv.hpp"

int main(int argc, char **argv)
{
    bool keep_on = true;
    if (argc != 2)
    {
        std::cerr << "First argument have to be a full path to config file" << std::endl;
        return (EXIT_FAILURE);
    }
    try
    {
        Webserv webserv(argv[1]);
        while (keep_on)
        {
            try {
                webserv.start();
                keep_on = false;
            }
            catch (std::runtime_error & ex) {
                webserv.sendErrMsg(std::string("FATAL ") + ex.what());
            }
        }
    }
    catch (std::runtime_error & ex) {
        std::cerr << "[Webserv] Error: " << ex.what() << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}