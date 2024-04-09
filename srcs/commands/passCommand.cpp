#include "Server.hpp"

void	Server::passCommand(std::vector<std::string> cmd, int fd)
{
    // --- Debug message ---
    std::cout << " ----- Input of passCommand ----- " << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
        std::cout << cmd[i] << " ";
    std::cout << std::endl;
    // ---------------------
    Client	*currentClient = this->_clients[fd];

    if (currentClient->getIsRegister() == true) {
        this->printLogMessage("ERR_ALREADYREGISTERED (462)\n", ERROR);
        return;
    }
    else if (cmd.size() != 2 || cmd[1].empty()) {
        this->printLogMessage("ERR_NEEDMOREPARAMS (461)\n", ERROR);
        return;
    }
    if (this->_password.compare(cmd[1].c_str())) {
        this->printLogMessage("ERR_PASSWDMISMATCH (464)\n", ERROR);
        //Here close the client connection with ERROR COMMAND
    }
    currentClient->setServerPassword(cmd[1]);
    // --- Debug message ---
    // std::cout << "passcmd test: pswd set as: " << currentClient->getServerPassword() << std::endl;
    // ---------------------
}