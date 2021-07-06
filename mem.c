/*************************************************
LibMem
*************************************************/

void mem_cpy(char dest[], char src[], int size) {

	for (int i = 0; i < size; i++) {
		dest[i] = src[i];
	}
}


void mem_set(short addr[], int size, char value) {

	for (int i = 0; i < size; i++) {
		addr[i] = value;
	}
}

int mem_scanMem(char * startAddress) {
	int i = 0;
	while (1) {
		char orig = startAddress[i];
		startAddress[i] = (char)0x00; //set value here
		if (startAddress[i] != (char)0x00) {
			break;
		}
		startAddress[i] = (char)0xFF; //set value here
		if (startAddress[i] != (char)0xFF) {
			break;
		}
		startAddress[i] = orig;
		i++;
	}
	return i+(int)startAddress;
}

