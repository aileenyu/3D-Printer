task main() {
	TFileHandle pHandle;
	TFileIOResult pIOResult;

	const string fileName = "part.txt";
	const byte CR = 0x0D; // carriage return
	const byte LF = 0x0A; // line feed
	const byte SP = 0x20; // space
	short fileSize = 100;


	float sum[6];
	char lineChk, command[8];
	int nLineCounter = 1;
	string num = "";


	int index;
	int lineNum = 0;

	OpenRead(pHandle, pIOResult, fileName, fileSize); // open the text file
	char array[4]; // INCLUDES THE CARRIAGE RETURNS/SPACES/LINE_FEEDS

	while() { // iterates through lines

		do { //goes through the line until end of line

			do {
			} while();



			if(command[cmd_index] == 'X') {
				sum[cmd_index] = x;
			}
		}while(temp != CR && temp != CR)
	}

	Close(pHandle, pIOResult);


	// stop if (pIOResult != ioRsltEndOfFile)
}
