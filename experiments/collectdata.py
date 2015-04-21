#!/usr/bin/python

import math
import glob
import numpy
import scipy.stats.mstats

# --------------------------Methods collectors-------------------------

def collect_Branch(pattern):
    ret = {"succ": 0, "fail": 0, "geomean": 0, "arithmean": 0}
    times = []
    minLength = 100000000.0
    for fName in glob.glob(pattern):
        with open(fName) as f:
            content = f.readlines()
            if "Took" in content[-1] and not any(map(lambda a: "Stopping" in a, content)):
                times.append( float(content[-1].split(" ")[1]) )
                ret["succ"] += 1

                length = 10000000.0
                for l in content: 
                    if "Primal" in l:
                        length = float(l.split(" ")[2])
                minLength = min(minLength, length)
            else:
                ret["fail"] += 1


    if times:
        ret["arithmean"] = numpy.mean(times)
        ret["geomean"] = scipy.stats.mstats.gmean(times)
        ret["std"] = numpy.std(times)
        ret["median"] = numpy.median(times)
        ret["times"] = times
        ret["minLength"] = minLength
    else:
        ret["arithmean"] = float('NaN')
        ret["geomean"] = float('NaN')
        ret["std"] = float('NaN')
        ret["median"] = float('NaN')
        ret["times"] = times
        ret["minLength"] = float('NaN')
    return ret


def collect_SmithStar(pattern):
    ret = {"succ": 0, "fail": 0, "geomean": 0, "arithmean": 0}
    times = []
    minLength = 100000000.0
    for fName in glob.glob(pattern):
        with open(fName) as f:
            content = f.readlines()
            if "Took" in content[-1] and not any(map(lambda a: "Stopping" in a, content)):
                times.append( float(content[-1].split(" ")[1]) )
                ret["succ"] += 1

                length = 10000000.0
                for l in content: 
                    if "Length" in l:
                        length = float(l.split(" ")[1])
                minLength = min(minLength, length)
            else:
                ret["fail"] += 1


    if times:
        ret["arithmean"] = numpy.mean(times)
        ret["geomean"] = scipy.stats.mstats.gmean(times)
        ret["std"] = numpy.std(times)
        ret["median"] = numpy.median(times)
        ret["times"] = times
        ret["minLength"] = minLength
    else:
        ret["arithmean"] = float('NaN')
        ret["geomean"] = float('NaN')
        ret["std"] = float('NaN')
        ret["median"] = float('NaN')
        ret["times"] = times
        ret["minLength"] = float('NaN')

    return ret

def collect_SmithStar_DFS(pattern):
    ret = {"succ": 0, "fail": 0, "geomean": 0, "arithmean": 0}
    times = []
    minLength = 100000000.0
    for fName in glob.glob(pattern):
        with open(fName) as f:
            content = f.readlines()
            if not content:
                print fName
            if "Took" in content[-1] and not any(map(lambda a: "Stopping" in a, content)):
                times.append( float(content[-1].split(" ")[1]) )
                ret["succ"] += 1

                length = 10000000.0
                for l in content: 
                    if "Length" in l:
                        length = float(l.split(" ")[1])
                minLength = min(minLength, length)
            else:
                ret["fail"] += 1


    if times:
        ret["arithmean"] = numpy.mean(times)
        ret["geomean"] = scipy.stats.mstats.gmean(times)
        ret["std"] = numpy.std(times)
        ret["median"] = numpy.median(times)
        ret["times"] = times
        ret["minLength"] = minLength
    else:
        ret["arithmean"] = float('NaN')
        ret["geomean"] = float('NaN')
        ret["std"] = float('NaN')
        ret["median"] = float('NaN')
        ret["times"] = times
        ret["minLength"] = float('NaN')

    return ret

def collect_SmithFixed(pattern):
    ret = {"succ": 0, "fail": 0, "geomean": 0, "arithmean": 0}
    times = []
    minLength = 100000000.0
    for fName in glob.glob(pattern):
        with open(fName) as f:
            content = f.readlines()
            if "Took" in content[-1] and not any(map(lambda a: "Stopping" in a, content)):
                times.append( float(content[-1].split(" ")[4]) )
                ret["succ"] += 1

                length = 10000000.0
                for l in content: 
                    if "length" in l:
                        tokens = l.split(" ")
                        length = float(tokens[-1])
                minLength = min(minLength, length)
            else:
                ret["fail"] += 1


    if times:
        ret["arithmean"] = numpy.mean(times)
        ret["geomean"] = scipy.stats.mstats.gmean(times)
        ret["std"] = numpy.std(times)
        ret["median"] = numpy.median(times)
        ret["times"] = times
        ret["minLength"] = minLength
    else:
        ret["arithmean"] = float('NaN')
        ret["geomean"] = float('NaN')
        ret["std"] = float('NaN')
        ret["median"] = float('NaN')
        ret["times"] = times
        ret["minLength"] = float('NaN')

    return ret

# --------------------------Problem instance collectors-------------------------

def collect_carioca():
    ret = []
    for d in range(3,6):
        for n in range(11,21):
            for meth in ["Branch", "SmithStar", "SmithFixed"]:
                if meth=="Branch":          resDict = collect_Branch( "Carioca/%s/carioca_%d_%d_*.txt"%(meth,d,n) )
                if meth=="SmithStar":       resDict = collect_SmithStar( "Carioca/%s/carioca_%d_%d_*.txt"%(meth,d,n) )
                if meth=="SmithFixed":    resDict = collect_SmithFixed( "Carioca/%s/carioca_%d_%d_*.txt"%(meth,d,n) )
                ret.append( {"name":"carioca", "d": d, "n": n,"method":meth,  "results":resDict} )

    return ret

def collect_Cube():
    ret = []
    for d in range(2,6):
        for n in range(10,16):
            for meth in ["Branch", "SmithStar", "SmithFixed", "SmithStar_DFS"]:
                if meth=="Branch":          resDict = collect_Branch(       "Cube/%s/cube_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithStar":       resDict = collect_SmithStar(    "Cube/%s/cube_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithFixed":    resDict = collect_SmithFixed(  "Cube/%s/cube_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithStar_DFS":   resDict = collect_SmithStar_DFS("Cube/%s/cube_n%d_d%d_*.txt"%(meth,n,d) )
                ret.append( {"name":"cube", "d": d, "n": n, "method":meth, "results":resDict} )

    return ret

def collect_Iowa():
    ret = []
    for d in range(3,6):
        n = 10
        for meth in ["Branch", "SmithStar", "SmithFixed"]:
            if meth=="Branch":          resDict = collect_Branch(       "Iowa05/%s/inst%dx%d_*.txt"%(meth,n,d) )
            if meth=="SmithStar":       resDict = collect_SmithStar(    "Iowa05/%s/inst%dx%d_*.txt"%(meth,n,d) )
            if meth=="SmithFixed":    resDict = collect_SmithFixed(  "Iowa05/%s/inst%dx%d_*.txt"%(meth,n,d) )
            ret.append( {"name":"Iowa05", "d": d, "n": n, "method":meth, "results":resDict} )

    return ret

def collect_Sausage():
    ret = []
    for d in range(2,6):
        for n in range(10,16):
            for meth in ["Branch", "SmithStar", "SmithFixed"]:
                if meth=="Branch":          resDict = collect_Branch(       "Sausage/%s/sausage_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithStar":       resDict = collect_SmithStar(    "Sausage/%s/sausage_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithFixed":    resDict = collect_SmithFixed(  "Sausage/%s/sausage_n%d_d%d_*.txt"%(meth,n,d) )
                ret.append( {"name":"Sausage", "d": d, "n": n, "method":meth, "results":resDict} )

    return ret

def collect_Shell():
    ret = []
    for d in range(2,6):
        for n in range(10,16):
            for meth in ["Branch", "SmithStar", "SmithFixed", "SmithStar_DFS"]:
                if meth=="Branch":          resDict = collect_Branch(       "Shell/%s/shell_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithStar":       resDict = collect_SmithStar(    "Shell/%s/shell_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithFixed":    resDict = collect_SmithFixed(  "Shell/%s/shell_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithStar_DFS":   resDict = collect_SmithStar_DFS("Shell/%s/shell_n%d_d%d_*.txt"%(meth,n,d) )
                ret.append( {"name":"Shell", "d": d, "n": n, "method":meth, "results":resDict} )

    return ret

def collect_Smith():
    ret = []
    for d in range(4,7):
        for meth in ["Branch", "SmithStar", "SmithFixed"]:
            if meth=="Branch":          resDict = collect_Branch(       "Smith/%s/noctha_%d_*.txt"%(meth,d) )
            if meth=="SmithStar":       resDict = collect_SmithStar(    "Smith/%s/noctha_%d_*.txt"%(meth,d) )
            if meth=="SmithFixed":    resDict = collect_SmithFixed(  "Smith/%s/noctha_%d_*.txt"%(meth,d) )
            ret.append( {"name":"noctha", "d": d, "n": 2*d, "method":meth, "results":resDict} )
    for d in range(3,9):
        for meth in ["Branch", "SmithStar", "SmithFixed"]:
            if meth=="Branch":          resDict = collect_Branch(       "Smith/%s/nsimp_%d_*.txt"%(meth,d) )
            if meth=="SmithStar":       resDict = collect_SmithStar(    "Smith/%s/nsimp_%d_*.txt"%(meth,d) )
            if meth=="SmithFixed":    resDict = collect_SmithFixed(  "Smith/%s/nsimp_%d_*.txt"%(meth,d) )
            ret.append( {"name":"nsimp", "d": d, "n": d+1, "method":meth, "results":resDict} )

    return ret

def collect_Solids():
    ret = []
    vertexMap = {'tetrahedron': 4, 'octahedron': 6, 'icosahedron':12, 'dodecahedron': 20, 'cube': 8}
    for solid in vertexMap:
        for meth in ["Branch", "SmithStar", "SmithFixed"]:
            if meth=="Branch":          resDict = collect_Branch(       "Solids/%s/%s.txt"%(meth,solid) )
            if meth=="SmithStar":       resDict = collect_SmithStar(    "Solids/%s/%s.txt"%(meth,solid) )
            if meth=="SmithFixed":    resDict = collect_SmithFixed(  "Solids/%s/%s.txt"%(meth,solid) )
            ret.append( {"name":solid, "d": 3, "n": vertexMap[solid], "method":meth, "results":resDict} )

    return ret

