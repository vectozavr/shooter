//#ifndef SHOOTER_SHOOTERSERVER_H
#ifndef CHAT_H
#define CHAT_H
#include <vector>
#include <string>
using namespace std;

class ChatManager final {
private:
	std::vector<std::string> messages;
	std::vector<std::string> authors;
	bool isChatUpdate = true;
	std::string chatStr = "";
	double hide = 0.0;
public:
	void addNewMessage(std::string author, std::string message);
	int update(double delta);
	std::string getChat();
};
#endif 