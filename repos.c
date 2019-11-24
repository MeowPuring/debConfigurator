#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

struct Addrep {
	bool updates;
	bool security;
};

struct Nonfree {
	bool contrib;
	bool nonfree;
};

struct Poll {
	char linktorep[100];
	char release[20];
	bool turnsrc;
        struct Addrep Addrep;	
	struct Nonfree Nonfree;
} Poll; 

static char input[100];

static bool checkaccess(void);
struct Poll poll(void);
void editsrc(struct Poll);

bool
checkaccess()
{
	if ( access("/etc/apt/sources.list", W_OK) != -1 ) {
		return true;
	} else {
		return false;
	}
}

struct Poll
poll()
{
	static bool correctanswer; 

	printf("Please, enter link to you debian repository or 0 to default option\n"
	       "(e.g. and default option http://deb.debian.org/debian/)\n");
	gets(input);
	if ( strcmp(input, "0") == 0 ) {
		strcpy(Poll.linktorep, "http://deb.debian.org/debian/");
	} else {
		strcpy(Poll.linktorep, input);
	}

	printf("Enter debian release name what you want to use\n"
	       "(e.g. testing or buster)\n");
	gets(input);
	strcpy(Poll.release, input);

	printf("Do you want use source repositories? (yes/no)\n");
	correctanswer = false;
	while (correctanswer == false) {
		gets(input);
		if ( strcmp(input, "yes") == 0 ) {
			Poll.turnsrc = true;
			correctanswer = true;
		} else if ( strcmp(input, "no") == 0 ) {
			Poll.turnsrc = false;
			correctanswer = true;
		} else {
			printf("Answer is incorrect, try again.\n");
		}
	}
	
	printf("Do you want turn updates repositories? (yes/no)\n");
	correctanswer = false;
	while (correctanswer == false) {
		gets(input);
		if ( strcmp(input, "yes") == 0 ) {
			Poll.Addrep.updates = true;
			correctanswer = true;
		} else if ( strcmp(input, "no") == 0 ) {
			Poll.Addrep.updates = false;
			correctanswer = true;
		} else {
			printf("Answer is incorrect, try again.\n");
		}
	}

	printf("Do you want turn security repositories? (yes/no)\n");
	correctanswer = false;
	while (correctanswer == false) {
		gets(input);
		if ( strcmp(input, "yes") == 0 ) {
			Poll.Addrep.security = true;
			correctanswer = true;
		} else if ( strcmp(input, "no") == 0 ) {
			Poll.Addrep.security = false;
			correctanswer = true;
		} else {
			printf("Answer is incorrect, try again.\n");
		}
	}

	printf("Do you want turn contrib repositories? (yes/no)\n");
	correctanswer = false;
	while (correctanswer == false) {
		gets(input);
		if ( strcmp(input, "yes") == 0 ) {
			Poll.Nonfree.contrib = true;
			correctanswer = true;
		} else if ( strcmp(input, "no") == 0 ) {
			Poll.Nonfree.contrib = false;
			correctanswer = true;
		} else {
			printf("Answer is incorrect, try again.\n");
		}
	}

	printf("Do you want turn non-free repositories? (yes/no)\n");
	correctanswer = false;
	while (correctanswer == false) {
		gets(input);
		if ( strcmp(input, "yes") == 0 ) {
			Poll.Nonfree.nonfree = true;
			correctanswer = true;
		} else if ( strcmp(input, "no") == 0 ) {
			Poll.Nonfree.nonfree = false;
			correctanswer = true;
		} else {
			printf("Answer is incorrect, try again.\n");
		}
	}

	return Poll;
}

void
editsrc(struct Poll Poll)
{
	FILE *src;

	src = fopen("/etc/apt/sources.list", "w");
	fprintf(src, "%s\n\n", "#This file have been edited by debConfigurator.");
	fprintf(src, "%s %s %s %s", "deb", Poll.linktorep, Poll.release, " main");

	if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == true ) {
		fprintf(src, "%s %s\n", " contrib", " non-free");
	} else if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == false) {
		fprintf(src, "%s\n", " contrib");
	} else {
		fprintf(src, "%s\n", "");
	}
	
	if ( Poll.turnsrc == true ) {
		fprintf(src, "%s %s %s %s", "deb-src", Poll.linktorep, Poll.release, " main");
		if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == true ) {
			fprintf(src, "%s %s\n", " contrib", " non-free");
		} else if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == false) {
			fprintf(src, "%s\n", " contrib");
		} else {
		fprintf(src, "%s\n", "");
		}
	}

	if ( Poll.Addrep.updates == true ) {
		fprintf(src, "%s %s %s%s %s", "deb", Poll.linktorep, Poll.release, "-updates", " main");
		if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == true ) {
			fprintf(src, "%s %s\n", " contrib", " non-free");
		} else if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == false) {
			fprintf(src, "%s\n", " contrib");
		} else {
			fprintf(src, "%s\n", "");
		}
	
		if ( Poll.turnsrc == true ) {
			fprintf(src, "%s %s %s%s %s", "deb-src", Poll.linktorep, Poll.release, "-updates", " main");
			if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == true ) {
				fprintf(src, "%s %s\n", " contrib", " non-free");
			} else if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == false) {
				fprintf(src, "%s\n", " contrib");
			} else {
			fprintf(src, "%s\n", "");
			}
		}
	}

	if ( Poll.Addrep.security == true ) {
		int i = 0;
		char linkbuffer[100];
		char securitybuffer[11];
		strcpy(securitybuffer, "//security.");
		strcpy(linkbuffer, Poll.linktorep);

		while( linkbuffer[i] != '/', linkbuffer[i+1] != '/' ) {
			Poll.linktorep[i] = linkbuffer[i];
			i++;
		}

		i++;
		int n = i;

		for( int j = 0; j < 11; j++ ) {
			Poll.linktorep[i+j] = securitybuffer[j];
			n++;
		}

		n++;

		while( linkbuffer[i] != '\0' ) {
			Poll.linktorep[n] = linkbuffer[i];
			i++;
			n++;
		}


		fprintf(src, "%s %s %s %s", "deb", Poll.linktorep, Poll.release, " main");

		if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == true ) {
			fprintf(src, "%s %s\n", " contrib", " non-free");
		} else if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == false) {
			fprintf(src, "%s\n", " contrib");
		} else {
			fprintf(src, "%s\n", "");
		}
	
		if ( Poll.turnsrc == true ) {
			fprintf(src, "%s %s %s %s", "deb-src", Poll.linktorep, Poll.release, " main");
			if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == true ) {
				fprintf(src, "%s %s\n", " contrib", " non-free");
			} else if ( Poll.Nonfree.contrib == true && Poll.Nonfree.nonfree == false) {
				fprintf(src, "%s\n", " contrib");
			} else {
			fprintf(src, "%s\n", "");
			}
		}
	}
}

int
main (int argc, char *argv[])
{
	if ( checkaccess() == false ) {
		printf("Can't open the file. Lack of permissions or file doesn't exist.\n"
		       "You can try use sudo or run this application from root user, if it's doesn't work, please, create sources.list file.\n");
		return EXIT_FAILURE;
	} else {
		Poll = poll();
		editsrc(Poll);
	}

	return EXIT_SUCCESS;
}
