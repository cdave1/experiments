#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/*
 
 The following mapping from letters to digits is given:
 
 E | J N Q | R W X | D S Y | F T | A M | C I V | B K U | L O P | G H Z
 e | j n q | r w x | d s y | f t | a m | c i v | b k u | l o p | g h z
 0 |   1   |   2   |   3   |  4  |  5  |   6   |   7   |   8   |   9
 
 We want to use this mapping for encoding telephone numbers by words, so
 that it becomes easier to remember the numbers.
 
 Functional requirements
 
 Your task is writing a program that finds, for a given phone number, all
 possible encodings by words, and prints them. A phone number is an
 arbitrary(!) string of dashes - , slashes / and digits. The dashes and
 slashes will not be encoded. The words are taken from a dictionary which
 is given as an alphabetically sorted ASCII file (one word per line).
 
 [NOTE: The dictionary is in German and contains umlaut characters
 encoded as double-quotes.  The double-quotes should be ignored.  EG.]
 
 Only exactly each encoding that is possible from this dictionary and
 that matches the phone number exactly shall be printed. Thus, possibly
 nothing is printed at all. The words in the dictionary contain letters
 (capital or small, but the difference is ignored in the sorting), dashes
 - and double quotes " . For the encoding only the letters are used, but
 the words must be printed in exactly the form given in the dictionary.
 Leading non-letters do not occur in the dictionary.
 
 Encodings of phone numbers can consist of a single word or of multiple
 words separated by spaces. The encodings are built word by word from
 left to right. If and only if at a particular point no word at all from
 the dictionary can be inserted, a single digit from the phone number can
 be copied to the encoding instead. Two subsequent digits are never
 allowed, though. To put it differently: In a partial encoding that
 currently covers k digits, digit k+1 is encoded by itself if and only if,
 first, digit k was not encoded by a digit and, second, there is no word
 in the dictionary that can be used in the encoding starting at digit k+1.
 
 Your program must work on a series of phone numbers; for each encoding
 that it finds, it must print the phone number followed by a colon, a
 single(!) space, and the encoding on one line; trailing spaces are not
 allowed. All remaining ambiguities in this specification will be
 resolved by the following example. (Still remaining ambiguities are
 intended degrees of freedom.)
 
 Sample dictionary:
 
 an
 blau
 Bo"
 Boot
 bo"s
 da
 Fee
 fern
 Fest
 fort
 je
 jemand
 mir
 Mix
 Mixer
 Name
 neu
 o"d
 Ort
 so
 Tor
 Torf
 Wasser
 
 Sample phone number list:
 
 112
 5624-82
 4824
 0721/608-4067
 10/783--5
 1078-913-5
 381482
 04824
 
 Corresponding correct program output (on screen):
 5624-82: mir Tor
 5624-82: Mix Tor
 4824: Torf
 4824: fort
 4824: Tor 4
 10/783--5: neu o"d 5
 10/783--5: je bo"s 5
 10/783--5: je Bo" da
 381482: so 1 Tor
 04824: 0 Torf
 04824: 0 fort
 04824: 0 Tor 4
 
 Any other output would be wrong (except for different ordering of the
 lines).
 
 Wrong outputs for the above example would be e.g.
 
 562482: Mix Tor, because the formatting of the phone number is
 incorrect,
 
 10/783--5: je bos 5, because the formatting of the second word is
 incorrect,
 
 4824: 4 Ort, because in place of the first digit the words Torf, fort,
 Tor could be used,
 
 1078-913-5: je Bo" 9 1 da , since there are two subsequent digits in the
 encoding,
 
 04824: 0 Tor , because the encoding does not cover the whole phone
 number, and
 
 5624-82: mir Torf , because the encoding is longer than the phone number.
 
 
 Quantitative requirements
 
 Length of the individual words in the dictionary: 50 characters maximum.
 Number of words in the dictionary: 75000 maximum
 Length of the phone numbers: 50 characters maximum.
 Number of entries in the phone number file: unlimited.
 
 Quality requirements
 
 Work as carefully as you would as a professional software engineer and
 deliver a correspondingly high grade program. Specifically, thoroughly
 comment your source code (design ideas etc.).
 
 The focus during program construction shall be on correctness. Generate
 exactly the right output format right from the start. Do not generate
 additional output. We will automatically test your program with hundreds
 of thousands of phone numbers and it should not make a single mistake,
 if possible -- in particular it must not crash. Take youself as much time
 as is required to ensure correctness.
 
 Your program must be run time efficient in so far that it analyzes only
 a very small fraction of all dictionary entries in each word appending
 step. It should also be memory efficient in that it does not use 75000
 times 50 bytes for storing the dictionary if that contains many much
 shorter words. The dictionary must be read into main memory entirely,
 but you must not do the same with the phone number file, as that may be
 arbitrarily large.
 
 Your program need not be robust against incorrect formats of the
 dictionary file or the phone number file.
 
 Please use only a single source program file, not several source
 modules.  And please work alone.
 
 A large dictionary, which you can use for testing purposes, is available
 at [original URL elided] 
 
 */


/**
 * 1. Each word in the dictionary is defined here.  This struct
 * probably is not necessary in its current form, but I've kept it because
 * I started off with more variables, and the code grew around this.
 */

#define kStringLength 64

typedef struct 
{
    char word[kStringLength];
    unsigned int len;
} word_t;


/**
 * 2. The dictionary array.
 */
word_t dict[75000];


/**
 * 3. To save time looking through the dictionary for each phone number, we 
 * use an index.  This index has 100 entries, and maps each
 * word in the dictionary to the value of its first pair of encoded letters.
 *
 * To give an example, "31450101" will be mapped to the 31st bucket in the 
 * index.
 *
 * We could use triples instead of pairs, or some other structure such as a 
 * tree, but this works reasonably well for now.
 */
typedef struct 
{
    word_t * words;
    int wordCount;
} wordIndexEntry_t;

#define kWordIndexMaxSize 100
static wordIndexEntry_t wordIndex[kWordIndexMaxSize];


/*
 * 4. The encoding looks like this:
 *
 * E | J N Q | R W X | D S Y | F T | A M | C I V | B K U | L O P | G H Z
 * e | j n q | r w x | d s y | f t | a m | c i v | b k u | l o p | g h z
 * 0 |   1   |   2   |   3   |  4  |  5  |   6   |   7   |   8   |   9
 *
 * We can simply store these values in a string with 26 characters:
 */
const char * encodings = "57630499617851881234762239";




/**
 * 5. Converts the word ("in") to an encoded string, using the encoding table.
 *
 * We convert the string to lower case first.
 */
unsigned int encodeWord(char * out, const char * in)
{
    unsigned int len = strlen(in);
    unsigned int i = 0, j = 0;
    char letter;
    while (i < len)
    {
        letter = in[i];
        if (letter >= 'A' && letter <= 'Z')
        {
            letter += ('a' - 'A');
        }
        if (letter >= 'a' && letter <= 'z')
        {
            out[j] = encodings[(int)(letter - 'a')];
            ++j;
        }
        ++i;
    }
    return j;
}


/**
 * 6. Gives the index value for the string.  Assumes the string has already
 * been encoded.
 */
unsigned int getWordIndexPairValue(char * str)
{
    int len = strlen(str);
    assert(len > 0);
#if 0
    unsigned int index = 0;
    if (len >= 1)
    {
        assert(str[0] >= '0' && str[0] <= '9');
        index = (unsigned int)(str[0] - '0');
    }
    if (len >= 2)
    {
        assert(str[1] >= '0' && str[1] <= '9');
        index += (10 * (str[1] - '0'));      
    }
    if (len >= 3)
    {
        assert(str[2] >= '0' && str[2] <= '9');
        index += (100 * (str[2] - '0'));
    }
    return index;
#else
    
    assert(str[0] >= '0' && str[0] <= '9');
    if (len == 1) return (unsigned int)(str[0] - '0');
    assert(str[1] >= '0' && str[1] <= '9');
    return (unsigned int)(str[0] - '0') + (10 * (str[1] - '0'));
#endif
}


/**
 * 7. Utility function for stripping the debris from phone numbers.
 */
unsigned int stripNonNumericCharacters(char * out, char * in)
{
    unsigned int len = strlen(in);
    unsigned int i = 0, j = 0;
    while (i < len)
    {
        if (in[i] >= '0' && in[i] <= '9')
        {
            out[j++] = in[i];
        }
        ++i;
    }
    return j;
}


/**
 * 8. Recursively find dictionary matchings for the given phone number string 
 * and length.
 *
 * If it finds a fully matched string, it will print the string and original 
 * phone number.  Otherwise, no output will be produced.
 *
 * "singleDigit" will be 1 if the previous match was a single digit.
 */
void matchPhoneNumber(char * orig, char * matchings, char * phoneNumber, 
                      const int len, const int singleDigit)
{
    char newMatchings[kStringLength];
    char encoded[kStringLength];
    int i = 0;
    int matches = 0;
    
    if (len == 0)
    {
        // we found a match!
        printf("%s:%s\n", orig, matchings);
        return;
    }
    if (len == 1)
    {
        if (singleDigit == 0)
        {
            snprintf(newMatchings, kStringLength, "%s %c", matchings, phoneNumber[0]);
            matchPhoneNumber(orig, newMatchings, NULL, 0, 1);
        }
        return;
    }
    
    wordIndexEntry_t indexEntry = wordIndex[getWordIndexPairValue(phoneNumber)];
    
    if (indexEntry.wordCount == 0)
    {
        if (!singleDigit)
        {
            snprintf(newMatchings, kStringLength, "%s %c", matchings, phoneNumber[0]);
            matchPhoneNumber(orig, newMatchings, phoneNumber + 1, len - 1, 1);
        }
    }
    else 
    {
        for (i = 0; i < indexEntry.wordCount; ++i)
        {
            unsigned int encLen = encodeWord(encoded, indexEntry.words[i].word);
            
            if (strncmp(phoneNumber, encoded, encLen) == 0)
            {
                ++matches;
                snprintf(newMatchings, kStringLength, "%s %s", matchings, indexEntry.words[i].word);
                matchPhoneNumber(orig, newMatchings, phoneNumber + encLen, len - encLen, 0);
            }
        }
        
        if (matches == 0 && singleDigit == 0)
        {
            snprintf(newMatchings, kStringLength, "%s %c", matchings, phoneNumber[0]);
            matchPhoneNumber(orig, newMatchings, phoneNumber + 1, len - 1, 1);
        }
    }
}


/**
 * 9. First, parse the dictionary file, preparing the dictionary table word 
 * counts.
 *
 * Next, we prepare the dictionary hashtable.
 *
 * Finally, we load the list of phone numbers, and put each phone number through
 * the matchPhoneNumber function above.
 */
int main (int argc, char ** argv)
{
    int i = 0, j = 0, len = 0;
    int found = 0;
    char in[kStringLength];
    char stripped[kStringLength];
    char encoded[kStringLength];
    int outLen;
    int dictCount = 0;
    
    FILE *dictFile = fopen("dictionary.txt", "r");
    if (!dictFile) perror("dictionary.txt\n");
    
    memset(wordIndex, 0, sizeof(wordIndex));

    while (fgets(in, sizeof(in), dictFile))
    {
        if (in[strlen(in)-1] == '\n')
            in[strlen(in)-1] = '\0';
        snprintf(dict[dictCount].word, kStringLength, "%s", in);
        dict[dictCount].len = strlen(in);
        ++dictCount;
        
        encodeWord(encoded, in);
        wordIndex[getWordIndexPairValue(encoded)].wordCount++;
    }
    fclose(dictFile);

    for (i = 0; i < kWordIndexMaxSize; ++i)
    {
        if (!(wordIndex[i].words = (word_t *)calloc(1, sizeof(word_t) * wordIndex[i].wordCount))) assert(0);
        
        int tmpWordCount = 0;
        for ( j = 0; j < dictCount; ++j)
        {
            encodeWord(encoded, dict[j].word);
            if (getWordIndexPairValue(encoded) == i)
            {
                wordIndex[i].words[tmpWordCount++] = dict[j];
            }
        }
        assert(tmpWordCount == wordIndex[i].wordCount);
    }
    
    FILE *inputFile = fopen("input.txt", "r");
    if (!inputFile) perror("input.txt");
    
    while (fgets(in, sizeof(in), inputFile))
    {
        if (in[strlen(in)-1] == '\n')
            in[strlen(in)-1] = '\0';
        memset(stripped, 0, sizeof(stripped));
        len = stripNonNumericCharacters(stripped, in);
        matchPhoneNumber(in, "", stripped, len, 0);
    }
    
    for (i = 0; i < kWordIndexMaxSize; ++i)
    {
        free(wordIndex[i].words);
    }
    
    fclose(inputFile);
    return 0;
}