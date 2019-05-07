import fileinput
import re
#KISS program to tokenize input. Splits input to words and then each word containing punctuation is 
for line in fileinput.input():
    for word in line.split():
        for token in re.findall(r"[\w]+|[^\s\w]", word):
            print(token)