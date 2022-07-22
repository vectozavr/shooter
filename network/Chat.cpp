#include "Chat.h"
#include <string>
#include <iostream>
void ChatManager::addNewMessage(std::string author, std::string message) {
	hide = 3.0;
	messages.push_back(message);
	authors.push_back(author);
	isChatUpdate = true;
}
int ChatManager::update(double delta) {
	hide = std::max(hide-delta, 0.0);
	return std::min((int)(hide * 255.0), 255);

}
std::string ChatManager::getChat() {
	if (isChatUpdate) {
		isChatUpdate = false;
		int size = messages.size();
		chatStr = "";
		for (int messageIndex = messages.size()-1; messageIndex > 0 && chatStr.size() < 100; messageIndex--)
		{
			chatStr += authors[messageIndex] + ": " + messages[messageIndex] + ";\n";
		}
	}
	return chatStr;
}
