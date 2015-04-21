#!/usr/bin/python

from collectdata import *
import collections



#Print command for single-seed results
results = []
results.extend( collect_Sausage() )
results.extend( collect_Smith() )
results.extend( collect_Solids() )
tableDict = collections.OrderedDict([])
for resDict in results:
    entry = (resDict['name'], resDict['d'], resDict['n'])
    if not entry in tableDict:
        tableDict[entry] = {}
    tableDict[entry][resDict['method']] = resDict['results']['arithmean']
    if not 'length' in tableDict[entry] or not math.isnan(resDict['results']['minLength']):
        tableDict[entry]['length'] =  resDict['results']['minLength']


for rowEntry in tableDict:
    print "%s & %d & %d & %.1f & %.1f & %.1f & %.4f \\\\" % (
            rowEntry[0], 
            rowEntry[1], 
            rowEntry[2], 
            tableDict[rowEntry]['Branch'],
            tableDict[rowEntry]['SmithStar'],
            tableDict[rowEntry]['SmithFixed'],
            tableDict[rowEntry]['length']
            )


