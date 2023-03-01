import string
import random

def generate():
    return "".join([random.choice("abcdefghijklmnopqrstuvwxyz") for _ in range(200)])

with open("randomtest1000.txt", "w") as f:
    f.write(generate() + '\n')
    f.write(generate() + '\n')
