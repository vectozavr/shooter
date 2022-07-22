#include "Chat.h"
#include <string>
#include <iostream>
void ChatManager::addNewMessage(std::string author, std::string message) {
	hide = 7.0;
	messages.push_back(message);
	authors.push_back(author);
	isChatUpdate = true;
	if (messages.size() > 6) {
		messages.erase(messages.begin());
	}
}
int ChatManager::update(double delta) {
	hide = std::max(hide-delta, 0.0);
	if (hide < 0.0000001) {
		messages.clear();
		authors.clear();
	}
	return std::min((int)(hide * 255.0), 255);

}
std::string ChatManager::getChat() {
	if (isChatUpdate) {
		isChatUpdate = false;
		int size = messages.size();
		chatStr = "";
		for (int messageIndex = messages.size()-1; messageIndex >= 0; messageIndex--)
		{
			chatStr += authors[messageIndex] + ": " + messages[messageIndex] + ";\n";
		}
	}
	return chatStr;
}
