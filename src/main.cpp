#include "../inc/webserv.hpp"

void demonize()
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);

    freopen(MUTE_FILE, "r", stdin);
    
    chdir("/");

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
}


void    parse_config(const char *path)
{
    int file = open(path, O_RDONLY);
    dup2(file, 0);
    if (yyparse(&g_config))
    {
        std::cerr << "Сonfig file is not valid" << std::endl;
        exit(EXIT_FAILURE);
    }
    close(file);
    if (g_config.open_logs())
        std::cerr << "Сouldn't open log files" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        std::cerr << "First argument have to be a full path to config file" << std::endl;
    
    parse_config(argv[1]);

    if (!(argc > 2 && !strncmp(argv[2], "non-daemon", 11)))
        demonize();
    
    
    return (EXIT_SUCCESS);
}