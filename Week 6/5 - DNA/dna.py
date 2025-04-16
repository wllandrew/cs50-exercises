import csv
import sys


def main():
    # Check command line arguments
    if len(sys.argv) != 3:
        print("Usage: python dna.py [DATABASE] [SEQUENCE]")
        return

    # Read database file into a variable
    people = []
    with open(sys.argv[1]) as file:
        inp = csv.DictReader(file)
        sqcs = inp.fieldnames
        for n in inp:
            people += [n]
    sqcs.remove("name")

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2]) as file:
        sequence = file.read()

    # TODO: Find longest match of each STR in DNA sequence
    occurence = {}
    for n in sqcs:
        occurence[n] = longest_match(sequence, n)

    # TODO: Check database for matching profiles
    for person in people:
        check = True
        for sq in sqcs:
            if int(person[sq]) != occurence[sq]:
                check = False
        if check:
            print(person["name"])

    print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
