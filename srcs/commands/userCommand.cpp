#include "Server.hpp"

bool	Server::_isValidUserCommand(size_t i, Client  *currentClient, std::vector<std::string> *cmd)
{
    switch (i) {
        case 1:
            if ((*cmd)[1].empty()) {
                this->printLogMessage("ERR_NEEDMOREPARAMS (461)\n", ERROR);
                return false;
            }
            (*cmd)[1] = '~' + (*cmd)[1];
            if ((*cmd)[1].size() > USERLEN)
                (*cmd)[1] = (*cmd)[1].substr(0, USERLEN);
            break;
        case 2:
            if ((*cmd)[2].size() != 1 || (*cmd)[2].at(0) != '0') {
                this->printLogMessage("userCommand: invalid command, usage: USER <USERNAME> 0 * :<REALNAME>\n", ERROR);
                return false;
            }
            break;
        case 3:
            if ((*cmd)[3].size() != 1 || (*cmd)[3].at(0) != '*') {
                this->printLogMessage("userCommand: invalid command, usage: USER <USERNAME> 0 * :<REALNAME>\n", ERROR);
                return false;
            }
            break;
        case 4:
            if ((*cmd)[4].empty() || ((*cmd)[4].at(0) == ':' && (*cmd)[4].size() == 1)) {
                if (!currentClient->getNickName().empty()) {
                    (*cmd)[4] = currentClient->getNickName();
                }
                else {
                    this->printLogMessage("ERR_NEEDMOREPARAMS (461)\n", ERROR);
                    return false;
                }
            }
            if ((*cmd)[4].at(0) == ':') {
                (*cmd)[4] = (*cmd)[4].substr(1, (*cmd)[4].size());
            }
            break;
    }
    return (true);
}

void	Server::userCommand(std::vector<std::string> cmd, int fd)
{
    // --- Debug message ---
    std::cout << " ----- Input of userCommand ----- " << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
        std::cout << cmd[i] << " ";
    std::cout << std::endl;
    // ---------------------
    Client	*currentClient = this->_clients[fd];

    if (currentClient->getIsRegister() == true) {
        this->printLogMessage("ERR_ALREADYREGISTERED (462)\n", ERROR);
        return;
    }
    if (cmd.size() < 5) {
        sendMessage(fd, "USER :Not enough parameters");
        return;
    }
    for (size_t i = 1; i != cmd.size(); i++) {
        if (this->_isValidUserCommand(i, currentClient, &cmd) == false)
            return;
    }
    currentClient->setuserName(cmd[1]);
    currentClient->setrealName(cmd[4]);
    if (!currentClient->getServerPassword().empty() && !currentClient->getNickName().empty()) {
        sendMessage(fd, ":server 001 " + currentClient->getNickName() + " :Welcome to the localhost Network, " + currentClient->getNickName() + "!" + currentClient->getUserName() + "@localhost");
        currentClient->setIsRegister(true);
    }
    // std::cout << "usercmd test: username set as: " << currentClient->getUserName() << std::endl;
    // std::cout << "usercmd test: realname set as: " << currentClient->getRealName() << std::endl;
    // std::cout << "usercmd test: isRegister set as: " << std::string(currentClient->getIsRegister() ? "true" : "false") << std::endl;

}