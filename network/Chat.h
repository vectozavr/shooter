#ifndef SHOOTER_CHAT_H
#define SHOOTER_CHAT_H

#include <vector>
#include <string>

class ChatManager final {
private:
	std::vector<std::string> messages;
	std::vector<std::string> authors;

	bool isChatUpdate = true;
	std::string chatStr;
	std::string chatStrPrev;

	double hide = 0.0;

    void updateChat();
public:
	void addNewMessage(const std::string& author, const std::string& message);
	int update(double delta);

	std::string getChat();
	std::string getChatPreview();
};


#endif