import fileinput

def find_line(word, filename):
    file=open(filename,"r")
    # print(len(file.readlines()))
    for line in file:
        # print(line)
        if word in line:
            file.close()
            return line
    file.close()
    return None

def basic_form(word):
    return (find_line(word, "superbazy.txt")).split()[1]

def infinitive2masculine(verb):
    return (find_line(verb, "verb_czytalem.txt")).split()[1]

def infinitive2feminine(verb):
    return (find_line(verb, "verb_czytalam.txt")).split()[1]

def masculine2feminine(verb):
    return infinitive2feminine(basic_form(verb))

def feminine2masculine(verb):
    return infinitive2masculine(basic_form(verb))
