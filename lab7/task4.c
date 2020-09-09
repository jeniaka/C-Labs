
int digit_cnt(char * argv){
   	int i = 0;
	int counter = 0;
	while (argv[i] != '\0') {
		if (argv[i] >= '0' && argv[i] <= '9') {
			counter++;
			
		}
		i++;
	}
	return counter;

}

int main(int argc, char* argv[], char* envp[]) {
	char *c =argv[1];
    digit_cnt(c);
}
