#! /bin/python3.5

import sys
import csv

rows = []
multiplier = float (sys.argv[1])
print (multiplier)
with open ("test_front.txt", "r") as csvfile :
	spamreader = csv.reader(csvfile)
	for row in spamreader:
		rows.append((float(row[0]), float(row[1]) * multiplier))

with open('test_front.txt', 'w', newline='') as csvfile:
	spamwriter = csv.writer(csvfile)
	for row in rows :
		spamwriter.writerow(row)
