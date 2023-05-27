#include "Chat.h"
#include <string>
#include <iostream>

void ChatManager::addNewMessage(std::string author, std::string message) {
	hide = 7.0;
	messages.push_back(message);
	authors.push_back(author);
	isChatUpdate = true;
	if (messages.size() > 20) {
		messages.erase(messages.begin());
	}
}
int ChatManager::update(double delta) {
	hide = std::max(hide-delta, 0.0);
	return std::min((int)(hide * 255.0), 255);

}
std::string ChatManager::getChat() {
	updateChat(); return chatStr;
}
std::string ChatManager::getChatPreview() {
	updateChat(); return chatStrPrev;
}

void ChatManager::updateChat() {
	if (isChatUpdate) {
		isChatUpdate = false;
		int size = messages.size();
		chatStr = "";
		chatStrPrev = "";
		for (int messageIndex = size - 1; messageIndex >= 0; messageIndex--)
		{
			if (messageIndex > size - 6) {
				chatStrPrev += authors[messageIndex] + ": " + messages[messageIndex] + "\n";
			}
			chatStr += authors[messageIndex] + ": " + messages[messageIndex] + "\n";
		}
	}
}
