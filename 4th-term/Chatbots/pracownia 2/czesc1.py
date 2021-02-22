import convert_masculine_feminine as mf
# import tokenize
from tokenize import tokenize


def find_past_form(sentence):
    for token in tokenize(sentence):
        line = mf.find_line(token, "verb_czytalem.txt")
        if  line==None:
            pass
        else:
            return line
    return None

print(find_past_form("Kiedyś lubiłem małym chłopcem"))