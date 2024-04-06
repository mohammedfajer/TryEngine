
const char *read_file(const char *path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cout << "Error unable to open file " << path << std::endl;
		return nullptr;
	}
	std::cout << "Successfully loaded file " << path << std::endl;
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content = buffer.str();
	char *contentArray = new char[content.length() + 1];
	std::strcpy(contentArray, content.c_str());
	contentArray[content.length()] = '\0';
	return contentArray;
}

