import csv

with open("databases/small.csv") as file:
    inp = csv.DictReader(file)
    print(inp.fieldnames)
