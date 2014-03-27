
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "kat_names.h"

#define KATAKANA_FIRST	0x30A1
#define KATAKANA_LAST	0x30F4
#define LONG_VOWEL_MARK	0x30FC

/* out needs to be at least 3*strlen(str) wide. The worst-case input is "ツツツツツ", which will be converted to
 * "tsutsutsutsutsu"
 * Returns 1 if an invalid code point is found and 2 if something does not make sense (such as a sokuon at the start of
 * the word). In these cases, the contents of buf are undefined. */
int internal_to_hepburn(char *str, char *buf){
	char *start = str;
	char sokuon = 0;
	while(*str){
		if(*str == 85){ /* ー */
			if(str == start)
				return 2;
			char *tmp = buf-1;
			*buf++ = *tmp;
			str++;
			continue;
		}else if(*str == 35){ /* ッ */
			if(str == start)
				return 2;
			sokuon = 1;
			str++;
			continue;
		}
		/* Consonants */
		switch(*str){
			/* Ignore */
			case 2:   /* ア */
			case 4:   /* イ */
			case 6:   /* ウ */
			case 8:   /* エ */
			case 10:  /* オ */
				break;
			/* Combining cases */
			case 1:   /* ァ */
			case 3:   /* ィ */
			case 5:   /* ゥ */
			case 7:   /* ェ */
			case 9:   /* ォ */
			case 67:  /* ャ */
			case 69:  /* ュ */
			case 71:  /* ョ */
				if(str == start)
					return 2;
				buf--;
				break;
			case 11:  /* カ */
			case 13:  /* キ */
			case 15:  /* ク */
			case 17:  /* ケ */
			case 19:  /* コ */
				*buf++ = 'k';
				break;
			case 12:  /* ガ */
			case 14:  /* ギ */
			case 16:  /* グ */
			case 18:  /* ゲ */
			case 20:  /* ゴ */
				*buf++ = 'g';
				break;
			case 21:  /* サ */
			case 25:  /* ス */
			case 27:  /* セ */
			case 29:  /* ソ */
				*buf++ = 's';
				break;
			case 22:  /* ザ */
			case 26:  /* ズ */
			case 28:  /* ゼ */
			case 30:  /* ゾ */
			case 37:  /* ヅ */
				*buf++ = 'z';
				break;
			case 24:  /* ジ */
			case 34:  /* ヂ */
				*buf++ = 'j';
				break;
			case 31:  /* タ */
			case 38:  /* テ */
			case 40:  /* ト */
				*buf++ = 't';
				break;
			case 32:  /* ダ */
			case 39:  /* デ */
			case 41:  /* ド */
				*buf++ = 'd';
				break;
			case 42:  /* ナ */
			case 43:  /* ニ */
			case 44:  /* ヌ */
			case 45:  /* ネ */
			case 46:  /* ノ */
				*buf++ = 'n';
				break;
			case 47:  /* ハ */
			case 50:  /* ヒ */
			case 56:  /* ヘ */
			case 59:  /* ホ */
				*buf++ = 'h';
				break;
			case 48:  /* バ */
			case 51:  /* ビ */
			case 54:  /* ブ */
			case 57:  /* ベ */
			case 60:  /* ボ */
				*buf++ = 'b';
				break;
			case 49:  /* パ */
			case 52:  /* ピ */
			case 55:  /* プ */
			case 58:  /* ペ */
			case 61:  /* ポ */
				*buf++ = 'p';
				break;
			case 62:  /* マ */
			case 63:  /* ミ */
			case 64:  /* ム */
			case 65:  /* メ */
			case 66:  /* モ */
				*buf++ = 'm';
				break;
			case 68:  /* ヤ */
			case 70:  /* ユ */
			case 72:  /* ヨ */
				*buf++ = 'y';
				break;
			case 73:  /* ラ */
			case 74:  /* リ */
			case 75:  /* ル */
			case 76:  /* レ */
			case 77:  /* ロ */
				*buf++ = 'r';
				break;
			case 79:  /* ワ */
			case 80:  /* ヰ */
			case 81:  /* ヱ */
			case 82:  /* ヲ */
				*buf++ = 'w';
				break;
			case 78:  /* ヮ */
				if(str == start)
					return 2;
				break;
			case 23:  /* シ */
				*buf++ = 's';
				*buf++ = 'h';
				break;
			case 33:  /* チ */
				*buf++ = 'c';
				*buf++ = 'h';
				break;
			case 36:  /* ツ */
				*buf++ = 't';
				*buf++ = 's';
				break;
			case 53:  /* フ */
				*buf++ = 'f';
				break;
			case 84:  /* ヴ */
				*buf++ = 'v';
				break;
			case 83: /* ン */
				*buf++ = 'n';
				break;
			default: /* Invalid code point */
				return 1;
		}
		if(sokuon){
			char *tmp = buf-1;
			if(*tmp == 'C'){
				*tmp = 'T';
				*buf++ = 'C';
			}else{
				*buf++ = *tmp;
			}
		}
		switch(*str){
			case 1:   /* ァ */
			case 2:   /* ア */
			case 11:  /* カ */
			case 12:  /* ガ */
			case 21:  /* サ */
			case 22:  /* ザ */
			case 31:  /* タ */
			case 32:  /* ダ */
			case 42:  /* ナ */
			case 47:  /* ハ */
			case 48:  /* バ */
			case 49:  /* パ */
			case 62:  /* マ */
			case 67:  /* ャ */
			case 68:  /* ヤ */
			case 73:  /* ラ */
			case 78:  /* ヮ */
			case 79:  /* ワ */
				*buf++ = 'a';
				break;
			case 3:   /* ィ */
			case 4:   /* イ */
			case 13:  /* キ */
			case 14:  /* ギ */
			case 23:  /* シ */
			case 24:  /* ジ */
			case 33:  /* チ */
			case 34:  /* ヂ */
			case 43:  /* ニ */
			case 50:  /* ヒ */
			case 51:  /* ビ */
			case 52:  /* ピ */
			case 63:  /* ミ */
			case 74:  /* リ */
			case 75:  /* ル */
			case 80:  /* ヰ */
			case 36:  /* ツ */
			case 37:  /* ヅ */
				*buf++ = 'i';
				break;
			case 5:   /* ゥ */
			case 6:   /* ウ */
			case 84:  /* ヴ */
			case 15:  /* ク */
			case 16:  /* グ */
			case 25:  /* ス */
			case 26:  /* ズ */
			case 44:  /* ヌ */
			case 53:  /* フ */
			case 54:  /* ブ */
			case 55:  /* プ */
			case 64:  /* ム */
			case 69:  /* ュ */
			case 70:  /* ユ */
				*buf++ = 'u';
				break;
			case 7:   /* ェ */
			case 8:   /* エ */
			case 17:  /* ケ */
			case 18:  /* ゲ */
			case 27:  /* セ */
			case 28:  /* ゼ */
			case 38:  /* テ */
			case 39:  /* デ */
			case 45:  /* ネ */
			case 56:  /* ヘ */
			case 57:  /* ベ */
			case 58:  /* ペ */
			case 65:  /* メ */
			case 76:  /* レ */
			case 81:  /* ヱ */
				*buf++ = 'e';
				break;
			case 9:   /* ォ */
			case 10:  /* オ */
			case 19:  /* コ */
			case 20:  /* ゴ */
			case 29:  /* ソ */
			case 30:  /* ゾ */
			case 40:  /* ト */
			case 41:  /* ド */
			case 46:  /* ノ */
			case 59:  /* ホ */
			case 60:  /* ボ */
			case 61:  /* ポ */
			case 66:  /* モ */
			case 77:  /* ロ */
			case 82:  /* ヲ */
			case 71:  /* ョ */
			case 72:  /* ヨ */
				*buf++ = 'o';
				break;
		}
		str++;
	}
	*buf = 0;
	return 0;
}

/* out must be thrice as wide as in plus one byte (for terminating \0)*/
void internal_to_utf8(char *in, char *out){
	while(*in){
		uint8_t foo = (*in == 85) ? 0xFC : (*in-1+0xA1);
		out[0] = 0xE3;
		out[1] = 0x80 | foo>>6;
		out[2] = 0x80 | (foo&0x3F);
		in++;
		out+=3;
	}
	*out = 0;
}

/* Assume code is a valid internal code point and not zero.
 * buf must point to a sufficient buffer.
 * Returns the number of bytes written.
 */
size_t decode(char *buf, uint8_t code){
	if(code <= 85){
		*buf = code;
		return 1;
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
		buf[0] = fcp;
		buf[1] = namedata[i];
		return 2;
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
			char in[] = {k, 0, l, 0};
			internal_to_utf8(in, tmp);
			internal_to_utf8(in+2, tmp+4);
			fprintf(stderr, "%d (%s) [%ld] Subtree codepoint %d (%s): %d\n", k, tmp, p-namedata, l, tmp+4, node);
			if(node&1){ /* The node itself is a valid name */
				name++;
				if(name == index){
					buf[0] = k;
					buf[1] = l;
					goto found;
				}
			}
			if(node&2){ /* The node hosts a suffix list */
				fprintf(stderr, "Handling suffix list: ");
				while(*(++p) != 0){
					char foo[7];
					char in[] = {decode(foo, *p)};
					internal_to_utf8(in, foo);
					fprintf(stderr, "%s ", foo);
					name++;
					if(name == index){
						fprintf(stderr, "\n");
						buf[0] = k;
						buf[1] = l;
						decode(buf+2, *p);
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
			char in[] = {k, 0};
			internal_to_utf8(in, tmp);
			fprintf(stderr, "Codepoint %d (%s): %d\n", k, tmp, node);
			if(node&1){ /* The node itself is a valid name */
				name++;
				if(name == index){
					*buf = k;
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
	int ec;
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
	char buf[longest_name+1];
	if(!lookup_name(index, buf)){
		fprintf(stderr, "500 Internal Server Error. Something went awfully wrong...\n");
		return 2;
	}
	char out[3*longest_name+1];
	char hepburn[3*longest_name+1];
	internal_to_utf8(buf, out);
	if((ec = internal_to_hepburn(buf, hepburn))){
		fprintf(stderr, "Romanization error %d.\n", ec);
	}
	printf("%s (%s)\n", out, hepburn);
	return 0;
}

