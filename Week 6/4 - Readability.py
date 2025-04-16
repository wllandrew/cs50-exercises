text = input("Text: ")

letter = 0
words = 1
sentences = 0

for n in text:
    if n.isalpha():
        letter += 1
    elif n == "." or n == "!" or n == "?":
        sentences += 1
    elif n == " ":
        words += 1

index = 0.0588 * (letter/words) * 100 - 0.296 * (sentences/words) * 100 - 15.8

if index < 1:
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")
