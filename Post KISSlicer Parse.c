#include "Globals.c"

task main()
{
	TFileHandle parsingHandle;
	TFileIOResult parsingIOResult;
	const string parsingName = "part.txt";
	short parsingSize = 100;
	bool commandFull = false;

	byte SP = 0x20;
	byte LF = 0x0a;
	byte CR =	0x0d;

	OpenRead(parsingHandle, parsingIOResult, parsingName, parsingSize);

	while(true){
		if(parsingIOResult == ioRsltEndOfFile)
		{Close(parsingHandle, parsingIOResult);}
		if(!commandFull){
			char type, skipG;
			string numStr;
			float typeVal;

			for(int i=0;, i<3; i++){
				ReadByte(parsingHandle, parsingIOResult, skipG);
			};

			int float_index = 0;

			do{
				ReadByte(parsingHandle, parsingIOResult, type);
				char temp[8];
				do{
					ReadByte(parsingHandle, parsingIOResult, temp[float_index]);
					float_index++;
				}while(temp != SP && temp != CR && temp != LF);

				StringFromChars(numStr, temp);
				typeVal = atof(numStr);

				if(type == 'X') active.x = typeVal;
				else if(temp == 'Y') active.y = typeVal;
				else if(temp == 'Z') active.z = typeVal;
				else if(temp == 'E') active.e = typeVal;
			}while(temp != CR && temp != LF);
		}
	}
}
