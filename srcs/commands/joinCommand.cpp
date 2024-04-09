#include "../includes/Server.hpp"

void                Server::joinCommand(commandTokensVector cmd, Client* client)
{
    std::vector<std::string>            channelsNameList(split(*cmd.begin(), ','));
    std::vector<std::string>::iterator  nameIt;
    std::vector<std::string>            channelsPasswordList;
    std::vector<std::string>::iterator  passwordIt;
    std::string                         password;

    if (cmd.size() > 1)
        channelsPasswordList = std::vector<std::string>(split(*(cmd.begin() + 1), ','));

    nameIt = channelsNameList.begin();
    passwordIt = channelsPasswordList.begin();
    for (; nameIt != channelsNameList.end(); nameIt++) {
        password = (passwordIt != channelsPasswordList.end()) ? *(passwordIt++) : "";

        if (nameIt->at(0) != '#' && nameIt->at(0) != '&') {
            sendMessage(client->getClientFd(), ":server 403 " + client->getNickName() + " " + *nameIt + ": Invalid channel name");
        }
        else if (this->_channelsList.find(*nameIt) == this->_channelsList.end()) {
            this->_channelsList.insert(std::make_pair(*nameIt, new Channel(*nameIt, client)));
        }
        else if (this->_channelsList.find(*nameIt) != this->_channelsList.end()) {
            if (!this->_channelsList.find(*nameIt)->second->addClient(client, password))
                std::cout << "Error while joining the server : bad password" << std::endl;
        }
    }
}