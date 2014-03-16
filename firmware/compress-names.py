#!/usr/bin/env python3

from collections import defaultdict
import operator
import itertools
import argparse

names = [l.rstrip() for l in open('fgn-readings-katakana-normalized.sorted').readlines()]
# '#ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュ ユョヨラリルレロヮワヰヱヲンヴー'
# Pad on the left for \0 terminator char
kat = '#'+''.join(chr(i) for i in range(ord('ァ'), ord('ヴ')+1))+'ー'

print('Loaded', len(names), 'names.')

digrams = defaultdict(lambda: 0)
for n in names:
	for i in range(len(n)-1):
		digrams[n[i:i+2]] += 1
digrams = sorted(digrams.items(), key=operator.itemgetter(1))
digramtable = list(sorted(k for k,v in digrams[-169:]))

def encode(s):
	out, pos = [], 0
	while pos<len(s):
		digram = s[pos:pos+2]
		if digram in digramtable:
			out.append(len(kat) + digramtable.index(digram))
			pos += 2
		else:
			out.append(kat.index(s[pos]))
			pos += 1
	return out

print('Tree encoding')

def plainlist(prefix, strings):
	assert all(s.startswith(prefix) for s in strings)
	reduced = [ encode(s[len(prefix):]) for s in strings if len(s) > len(prefix)]
	l = { s[0] for s in reduced if len(s) == 1 }
	#print('Generating suffix list: ', ' '.join(s[len(prefix):] for s in strings if len(s) > len(prefix)))
	# make tuple from list so it can be added to a set
	others = { tuple(s) for s in reduced if len(s) > 1 }
	return l, others

def subtree(prefix, strings):
	outliers = set()
	treedata = []
	for k in kat[1:]:
		prefixed = { n for n in strings if n.startswith(prefix+k) }
		if prefixed:
			if prefix+k in prefixed:
				if len(prefixed) == 1:
					#print("{} ({}) Subtree codepoint {} ({}): {}".format(prefix, kat.index(prefix), kat.index(k), k, 1))
					treedata.append(1)
				else:
					#print("{} ({}) Subtree codepoint {} ({}): {}".format(prefix, kat.index(prefix), kat.index(k), k, 3))
					l, o = plainlist(prefix+k, prefixed)
					if l:
						treedata.append(3)
						treedata.append(l)
					else:
						#print('***Empty suffix list***')
						treedata.append(1)
					outliers = outliers.union(o)
			else:
				#print("{} ({}) Subtree codepoint {} ({}): {}".format(prefix, kat.index(prefix), kat.index(k), k, 2))
				l, o = plainlist(prefix+k, prefixed)
				if l:
					treedata.append(2)
					treedata.append(l)
				else:
					#print('***Empty suffix list***')
					treedata.append(0)
				outliers = outliers.union(o)
		else:
			#print("{} ({}) Subtree codepoint {} ({}): {}".format(prefix, kat.index(prefix), kat.index(k), k, 0))
			treedata.append(0)
	return treedata, outliers

treedata = []
outliers = set()
for k in kat[1:-1]: # A long vowel mark is never to be found at the beginning of a word, and neither is \0
	prefixed = { n for n in names if n.startswith(k) }
	if prefixed:
		if k in prefixed:
			if len(prefixed) == 1:
				#print("Codepoint {} ({}): {}".format(kat.index(k), k, 1))
				treedata.append(1)
			else:
				#print("Codepoint {} ({}): {}".format(kat.index(k), k, 3))
				treedata.append(3)
				l, o = subtree(k, prefixed)
				treedata += [None] + l + [None]
				outliers = outliers.union(o)
		else:
			#print("Codepoint {} ({}): {}".format(kat.index(k), k, 2))
			treedata.append(2)
			l, o = subtree(k, prefixed)
			treedata += [None] +l + [None]
			outliers = outliers.union(o)
	else:
		#print("Codepoint {} ({}): {}".format(kat.index(k), k, 0))
		treedata.append(0)
	#print(treedata[-100:])

#print(treedata)

packed = []
print("Packing digram table")
for g,e in itertools.groupby(digramtable, operator.itemgetter(0)):
	secchar = map(operator.itemgetter(1), e)
	packed.append(kat.index(g))
	for s in secchar:
		packed.append(kat.index(s))
	packed.append(0)
packed.append(0)

print("Packing tree data, start index", len(packed))

for t,d in itertools.groupby(treedata, type):
	data = list(d)
	if t is set:
		assert all(type(e) is int for e in data[0])
		packed += list(data[0]) + [0]
	elif t is int:
		data += [0]*(0,3,2,1)[len(data)%4]
		for i in range(0, len(data), 4):
			packed.append(data[i+3]<<6 | data[i+2]<<4 | data[i+1]<<2 | data[i])
	else:
		assert t is type(None) and data == [None]

print("Packing long names in plain list")
buckets = defaultdict(lambda: [])
for n in outliers:
	buckets[len(n)].append(n)

for l in range(2, max(buckets.keys())+1):
	assert len(buckets[l]) != 0
	encoded = list(sorted(buckets[l]))
	for g,e in itertools.groupby(encoded, lambda s: s[:2]):
		e = list(e)
		if len(e)>4: # FIXME
			assert type(g) is tuple
			packed += [255] + list(g) + [x for l in e for x in l[2:]] + [0]
		else:
			packed += [x for l in e for x in l]
	packed.append(0)
packed.append(0)

assert all(c<256 for c in packed)
print('Total length:', len(packed))

parser = argparse.ArgumentParser()
parser.add_argument('-b', '--binary', nargs='?', type=argparse.FileType('w'))
parser.add_argument('-c', '--cheader', nargs='?', type=argparse.FileType('w'))
args = parser.parse_args()
if args.binary:
	with args.binary as f:
		f.write(bytes(packed))

if args.cheader:
	cheader = """\
#ifndef __KAT_NAMES_H__
#define __KAT_NAMES_H__

#include <stdint.h>
unsigned int const longest_name = """+str(max(len(n) for n in names))+""";
unsigned int const name_count = """+str(len(names))+""";
uint8_t const namedata[] = {
"""

	chunker = lambda iterable, n: (itertools.filterfalse(lambda x: x == (), chunk) for chunk in (itertools.zip_longest(*[iter(iterable)]*n, fillvalue=())))
	phstr = lambda x: '0x{:02x}'.format(x)
	cheader += ',\n'.join(', '.join(map(phstr, chunk)) for chunk in chunker(packed, 20))

	cheader += """};

#endif//__KAT_NAMES_H__"""

	with args.cheader as f:
		f.write(cheader)
