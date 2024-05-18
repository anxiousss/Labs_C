import random
import names


with open('test.txt', 'w') as fin:
    n_students = 50
    for i in range(n_students):
        surname = names.get_last_name()
        initials = surname[:3]
        school_number = random.randint(100, 2000)
        medal = random.randint(0, 1)
        scores = random.randint(2, 5)
        credit = random.randint(0, 1)
        fin.write(str(surname) + ' ' + str(initials) + ' ' + str(school_number) + ' ' +
                  str(medal) + ' ' + str(scores) + ' ' + str(credit) + '\n')
