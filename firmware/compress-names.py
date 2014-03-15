#!/usr/bin/env python3

from collections import defaultdict
import operator
import itertools

names = [l.rstrip() for l in open('fgn-readings-katakana-normalized.sorted').readlines()]
#ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶヷヸヹヺー
# Pad on the left for \0 terminator char
kat = '###ヱンュョメャロワラルフプパビポミベホヅデチッネハドニサシケコソタスセゥェァィキクォカヴヰヲーヤユムモレヨリピブヒボマヘペツテダヂヌノトナゴザグゲゼゾジズイウアガギエオバ'

print('Loaded', len(names), 'names.')

buckets = defaultdict(lambda: [])
for n in names:
	buckets[len(n)].append(n)
print('Names by length:', {k:len(v) for k,v in buckets.items()})

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

packed = []
for l in range(1, max(buckets.keys())+1):
	assert len(buckets[l]) != 0
	encoded = list(sorted(encode(n) for n in buckets[l]))
	for g,e in itertools.groupby(encoded, lambda s: s[:2]):
		e = list(e)
		if len(e)>4: # FIXME
			packed += [1] + g + [x for l in e for x in l[2:]] + [2]
		else:
			packed += [x for l in e for x in l]
	packed.append(0)
packed.append(0)

assert all(c<256 for c in packed)
print(len(packed))

print('Tree encoding')

def plainlist(prefix, strings):
	assert all(s.startswith(prefix) for s in strings)
	reduced = { s[len(prefix):] for s in strings }
	l = { s for s in reduced if len(s) == 1 }
	return l, reduced-l

def subtree(prefix, strings):
	outliers = {}
	treedata = []
	for k in kat:
		prefixed = { n for n in strings if n.startswith(prefix+k) }
		if prefixed:
			if k in prefixed:
				if len(prefixed == 1):
					treedata.append(1)
				else:
					treedata.append(2)
					l, o = plainlist(prefixed)
					treedata += l
					outliers = outliers.union(o)
			else:
				treedata.append(3)
				l, o = plainlist(prefixed)
				treedata += l
				outliers = outliers.union(o)
		else:
			treedata.append(0)
	return treedata, outliers


treedata = []
outliers = {}
for k in kat:
	prefixed = { n for n in names if n.startswith(k) }
	if prefixed:
		if k in prefixed:
			if len(prefixed == 1):
				treedata.append(1)
			else:
				treedata.append(2)
				l, o = subtree(k, prefixed)
				treedata.append(l)
				outliers = outliers.union(l)
		else:
			treedata.append(3)
			l, o = subtree(k, prefixed)
			treedata.append(l)
			outliers = outliers.union(l)
	else:
		treedata.append(0)

