import fileinput
import re
#KISS program to tokenize input. Splits input to words and then each word containing punctuation is 

def tokenize(string):
    # return_list=list()
    return_list=[]
    for token in re.findall(r"[\w]+|[^\s\w]", string):
        return_list.append(token)
    return return_list

# print(tokenize("Lam--lam, terefere!!!"))
