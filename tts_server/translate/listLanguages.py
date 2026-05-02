import translators as ts

a = ts.get_languages()
with open("languages.txt", "w", encoding="utf-8") as f:
    for i in a:
        f.write(i + "\n")

print("all available languages have been written to languages.txt")
