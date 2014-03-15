#!/usr/bin/env python3

from collections import defaultdict
import operator
import itertools

names = [l.rstrip() for l in open('fgn-readings-katakana-normalized.sorted').readlines()]
#ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶヷヸヹヺー
# Pad on the left for \0 terminator char
kat = '###ヱンュョメャロワラルフプパビポミベホヅデチッネハドニサシケコソタスセゥェァィキクォカヴヰヲーヤユムモレヨリピブヒボマヘペツテダヂヌノトナゴザグゲゼゾジズイウアガギエオバ'

print('Loaded', len(names), 'names.')

digrams = defaultdict(lambda: 0)
for n in names:
	for i in range(len(n)-1):
		digrams[n[i:i+2]] += 1
digrams = sorted(digrams.items(), key=operator.itemgetter(1))
digramtable = [k for k,v in digrams[-169:]]

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
	reduced = [ encode(s[len(prefix):]) for s in strings ]
	l = { s[0] for s in reduced if len(s) == 1 }
	# make tuple from list so it can be added to a set
	others = { tuple(s) for s in reduced if len(s) > 1 }
	return l, others

def subtree(prefix, strings):
	outliers = set()
	treedata = []
	for k in kat:
		prefixed = { n for n in strings if n.startswith(prefix+k) }
		if prefixed:
			if k in prefixed:
				if len(prefixed == 1):
					treedata.append(1)
				else:
					treedata.append(2)
					l, o = plainlist(prefix+k, prefixed)
					treedata.append(l)
					outliers = outliers.union(o)
			else:
				treedata.append(3)
				l, o = plainlist(prefix+k, prefixed)
				treedata.append(l)
				outliers = outliers.union(o)
		else:
			treedata.append(0)
	return treedata, outliers

treedata = []
outliers = set()
for k in kat:
	prefixed = { n for n in names if n.startswith(k) }
	if prefixed:
		if k in prefixed:
			if len(prefixed) == 1:
				treedata.append(1)
			else:
				treedata.append(2)
				l, o = subtree(k, prefixed)
				treedata += l
				outliers = outliers.union(o)
		else:
			treedata.append(3)
			l, o = subtree(k, prefixed)
			treedata += l
			outliers = outliers.union(o)
	else:
		treedata.append(0)

packed = []
cons = 0
bit = 0
for data in treedata:
	if type(data) is set: # set of ints
		packed += list(data) + [0]
	else:
		assert data in range(4)
		cons |= data << bit
		bit += 2
		if bit == 8:
			bit = 0
			packed.append(cons)

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
			# g is a tuple
			packed += [1] + list(g) + [x for l in e for x in l[2:]] + [2]
		else:
			packed += [x for l in e for x in l]
	packed.append(0)
packed.append(0)

assert all(c<256 for c in packed)
print(len(packed))
