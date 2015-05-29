task main() {
	TFileHandle pHandle;
	TFileIOResult pIOResult;

	const string fileName = "part.txt";
	const byte CARRIAGE_RETURN = 0x0D;
	const byte NEW_LINE = 0x0A;
	const byte SPACE = 0x20;

	short fileSize = 100;
	char list[100] ;
	string line = "";
	int index = 0;

	OpenRead(pHandle, pIOResult, fileName, fileSize);

	do {
		ReadFloat(pHandle, pIOResult, list[index]);
		index++;
	}while(list[index] != CARRIAGE_RETURN && list[index] != NEW_LINE && list[index] != SPACE);
	StringFromChars(line, list);

	for(int i = 0; i < index-1; i++ ){
		writeDebugStream(line);
	}
	while(true);

}
