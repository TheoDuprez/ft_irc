#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

std::vector<std::string>	makeGigaChatVector()
{
	std::vector<std::string>	gigaChatVector;
	std::string 				line;
	std::ifstream				motivationFile("motivation.csv");

	if (!motivationFile.is_open()) {
		throw std::runtime_error("motivation.csv file cannot be open");
	}
	while (std::getline(motivationFile, line)) {
		gigaChatVector.push_back(line);
	}
	return gigaChatVector;
}

std::string createGigaChatSentence()
{
	std::vector<std::string>	gigaChat;
	int	randomSentence;
	srand(time(NULL));

	gigaChat = makeGigaChatVector();
	randomSentence = rand() % gigaChat.size();

	return gigaChat[randomSentence];
}

int main(void)
{

	try {
		std::cout << createGigaChatSentence() << std::endl;
	}
	catch (const std::runtime_error& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
