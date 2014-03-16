
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "kat_names.h"

#define KATAKANA_FIRST	0x30A1
#define KATAKANA_LAST	0x30F4
#define LONG_VOWEL_MARK	0x30FC

/* Write UTF-8 katakana character from internal code point.
 * buf must point to a buffer three bytes wide.
 * This function assumes code is a valid internal code point.
 */
void write_katakana(char *buf, uint8_t code){
	uint8_t foo = (code == 85) ? 0xFC : (code-1+0xA1);
	buf[0] = 0xE3;
	buf[1] = 0x80 | foo>>6;
	buf[2] = 0x80 | (foo&0x3F);
}

/* Assume code is a valid internal code point and not zero.
 * buf must point to a sufficient buffer, normally that would be 6 chars wide though in the case of the last char of the
 * longest word in the list, less would suffice.
 * Returns the number of bytes written.
 */
size_t decode(char *buf, uint8_t code){
	if(code <= 85){
		write_katakana(buf, code);
		return 3;
	}else{ /* code is a digram */
		size_t i = 0;
		uint8_t cp = 85;
		uint8_t fcp = namedata[0];
		while(cp != code){
			i++;
			if(namedata[i] == 0){
				i++;
				fcp = namedata[i];
			}else{
				cp++;
			}
		}
		write_katakana(buf, fcp);
		write_katakana(buf+3, namedata[i]);
		return 6;
	}
}

int descend_subtree(uint8_t const **_p, int *_name, int index, uint8_t k, char *buf){
	uint8_t const *p = *_p;
	int name = *_name;
	uint8_t l = 0; /* Codepoint 1 is ァ */
	for(;;){
		uint16_t data = 0x100 | *p;
		p++;
		while(data != 1){
			uint8_t node = data&3;
			data>>=2;
			l++;
			char tmp[8];
			write_katakana(tmp, k);
			write_katakana(tmp+4, l);
			tmp[3] = 0;
			tmp[7] = 0;
			fprintf(stderr, "%d (%s) [%d] Subtree codepoint %d (%s): %d\n", k, tmp, p-namedata, l, tmp+4, node);
			if(node&1){ /* The node itself is a valid name */
				name++;
				if(name == index){
					write_katakana(buf, k);
					write_katakana(buf+3, l);
					goto found;
				}
			}
			if(node&2){ /* The node hosts a suffix list */
				fprintf(stderr, "Handling suffix list: ");
				while(*(++p) != 0){
					char foo[7];
					foo[3] = 0;
					foo[6] = 0;
					decode(foo, *p);
					fprintf(stderr, "%s ", foo);
					name++;
					if(name == index){
						fprintf(stderr, "\n");
						write_katakana(buf, k);
						write_katakana(buf+3, l);
						decode(buf+6, *p);
						goto found;
					}
				}
				p++;
				fprintf(stderr, "\n");
				break;
			}
			/* At this point, k might count a little farther than 85. This is fine though, since the bytes are always
			 * padded with 0 bits on the left. */
		}
		if(l >= 85)
			break;
	}
	*_p = p;
	*_name = name;
	return 0;
found:
	return 1;
}

/* buf must be at least 3*longest_name+1 wide. */
int lookup_name(int index, char *buf){
	/* Identify the start of the tree table by two consecutive zeros */
	uint8_t const *tree_table = namedata;
	while(*tree_table != 0){
		while(*tree_table != 0)
			tree_table++;
		tree_table++;
	}
	tree_table++;

	int name = 0;
	/* Walk the tree */
	uint8_t const *p = tree_table;
	uint8_t k=0; /* Codepoint 1 is ァ */
	for(;;){
		uint16_t data = 0x100 | *p;
		p++;
		while(data){
			uint8_t node = data&3;
			data>>=2;
			k++;
			char tmp[4];
			write_katakana(tmp, k);
			tmp[3] = 0;
			fprintf(stderr, "Codepoint %d (%s): %d\n", k, tmp, node);
			if(node&1){ /* The node itself is a valid name */
				name++;
				if(name == index){
					write_katakana(buf, k);
					return 1;
				}
			}
			if(node&2){ /* The node hosts a subtree */
				if(descend_subtree(&p, &name, index, k, buf))
					return 1;
				break;
			}
			/* At this point, k might count a little farther than 85. This is fine though, since the bytes are always
			 * padded with 0 bits on the left. */
		}
		if(k >= 84) /* Already break on long vowel mark (ー) since that cannot be a word's first character. */
			break;
	}

	/* Walk the list */
	fprintf(stderr, "Walking list of long names, %x %x %x %x %x %x\n", p[0], p[1], p[2], p[3], p[4], p[5]);
	uint8_t len = 2;
	while(p < namedata+sizeof(namedata)){
		while(*p != 0){
			name++;
			if(name == index){
				for(uint8_t i=0; i<len; i++){
					buf += decode(buf, p[i]);
				}
				*buf = 0;
				return 1;
			}
			p += len;
		}
		len++;
		p++;
	}
	return 0;
}

int main(int argc, char **argv){
	/*
	printf("Katakana: ");
	for(uint8_t i=1; i<=85; i++){
		char buf[4];
		buf[4] = 0;
		write_katakana(buf, i);
		printf("%s", buf);
	}
	printf("\nDigrams: ");
	for(uint8_t i=0; i<169; i++){
		char buf[7];
		buf[6] = 0;
		decode(buf, 86+i);
		printf("%s ", buf);
	}
	printf("\n");
	return 0;
	*/
	if(argc < 2){
		fprintf(stderr, "Requiring at least one argument: The index of the name to look up. Name count: %d\n", name_count);
		return 1;
	}
	int index = atoi(argv[1]);
	if(index <= 0 || index > name_count){
		fprintf(stderr, "Invalid index: %d. A valid index is between 1 and %d (inclusive).\n", index, name_count);
		return 1;
	}
	char buf[3*longest_name+1];
	memset(buf, 0, sizeof(buf));
	if(!lookup_name(index, buf)){
		fprintf(stderr, "500 Internal Server Error. Something went awfully wrong...\n");
		return 2;
	}
	printf("%s\n", buf);
	return 0;
}

