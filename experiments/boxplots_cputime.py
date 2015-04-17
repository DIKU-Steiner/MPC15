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

def collect_SmithMartin(pattern):
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
                        length = float(l.split(" ")[3])
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
            for meth in ["Branch", "SmithStar", "Smith_Martin"]:
                if meth=="Branch":          resDict = collect_Branch( "Carioca/%s/carioca_%d_%d_*.txt"%(meth,d,n) )
                if meth=="SmithStar":       resDict = collect_SmithStar( "Carioca/%s/carioca_%d_%d_*.txt"%(meth,d,n) )
                if meth=="Smith_Martin":    resDict = collect_SmithMartin( "Carioca/%s/carioca_%d_%d_*.txt"%(meth,d,n) )
                ret.append( {"name":"carioca", "d": d, "n": n,"method":meth,  "results":resDict} )

    return ret

def collect_Cube():
    ret = []
    for d in range(2,6):
        for n in range(10,16):
            for meth in ["Branch", "SmithStar", "Smith_Martin", "SmithStar_DFS"]:
                if meth=="Branch":          resDict = collect_Branch(       "Cube/%s/cube_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithStar":       resDict = collect_SmithStar(    "Cube/%s/cube_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="Smith_Martin":    resDict = collect_SmithMartin(  "Cube/%s/cube_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithStar_DFS":   resDict = collect_SmithStar_DFS("Cube/%s/cube_n%d_d%d_*.txt"%(meth,n,d) )
                ret.append( {"name":"cube", "d": d, "n": n, "method":meth, "results":resDict} )

    return ret

def collect_Iowa():
    ret = []
    for d in range(3,6):
        n = 10
        for meth in ["Branch", "SmithStar", "Smith_Martin"]:
            if meth=="Branch":          resDict = collect_Branch(       "Iowa05/%s/inst%dx%d_*.txt"%(meth,n,d) )
            if meth=="SmithStar":       resDict = collect_SmithStar(    "Iowa05/%s/inst%dx%d_*.txt"%(meth,n,d) )
            if meth=="Smith_Martin":    resDict = collect_SmithMartin(  "Iowa05/%s/inst%dx%d_*.txt"%(meth,n,d) )
            ret.append( {"name":"Iowa05", "d": d, "n": n, "method":meth, "results":resDict} )

    return ret

def collect_Sausage():
    ret = []
    for d in range(2,6):
        for n in range(10,16):
            for meth in ["Branch", "SmithStar", "Smith_Martin"]:
                if meth=="Branch":          resDict = collect_Branch(       "Sausage/%s/sausage_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithStar":       resDict = collect_SmithStar(    "Sausage/%s/sausage_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="Smith_Martin":    resDict = collect_SmithMartin(  "Sausage/%s/sausage_n%d_d%d_*.txt"%(meth,n,d) )
                ret.append( {"name":"Sausage", "d": d, "n": n, "method":meth, "results":resDict} )

    return ret

def collect_Shell():
    ret = []
    for d in range(2,6):
        for n in range(10,16):
            for meth in ["Branch", "SmithStar", "Smith_Martin", "SmithStar_DFS"]:
                if meth=="Branch":          resDict = collect_Branch(       "Shell/%s/shell_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithStar":       resDict = collect_SmithStar(    "Shell/%s/shell_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="Smith_Martin":    resDict = collect_SmithMartin(  "Shell/%s/shell_n%d_d%d_*.txt"%(meth,n,d) )
                if meth=="SmithStar_DFS":   resDict = collect_SmithStar_DFS("Shell/%s/shell_n%d_d%d_*.txt"%(meth,n,d) )
                ret.append( {"name":"Shell", "d": d, "n": n, "method":meth, "results":resDict} )

    return ret

def collect_Smith():
    ret = []
    for d in range(4,7):
        for meth in ["Branch", "SmithStar", "Smith_Martin"]:
            if meth=="Branch":          resDict = collect_Branch(       "Smith/%s/noctha_%d_*.txt"%(meth,d) )
            if meth=="SmithStar":       resDict = collect_SmithStar(    "Smith/%s/noctha_%d_*.txt"%(meth,d) )
            if meth=="Smith_Martin":    resDict = collect_SmithMartin(  "Smith/%s/noctha_%d_*.txt"%(meth,d) )
            ret.append( {"name":"noctha", "d": d, "n": 2*d, "method":meth, "results":resDict} )
    for d in range(3,9):
        for meth in ["Branch", "SmithStar", "Smith_Martin"]:
            if meth=="Branch":          resDict = collect_Branch(       "Smith/%s/nsimp_%d_*.txt"%(meth,d) )
            if meth=="SmithStar":       resDict = collect_SmithStar(    "Smith/%s/nsimp_%d_*.txt"%(meth,d) )
            if meth=="Smith_Martin":    resDict = collect_SmithMartin(  "Smith/%s/nsimp_%d_*.txt"%(meth,d) )
            ret.append( {"name":"nsimp", "d": d, "n": d+1, "method":meth, "results":resDict} )

    return ret

def collect_Solids():
    ret = []
    vertexMap = {'tetrahedron': 4, 'octahedron': 6, 'icosahedron':12, 'dodecahedron': 20, 'cube': 8}
    for solid in vertexMap:
        for meth in ["Branch", "SmithStar", "Smith_Martin"]:
            if meth=="Branch":          resDict = collect_Branch(       "Solids/%s/%s.txt"%(meth,solid) )
            if meth=="SmithStar":       resDict = collect_SmithStar(    "Solids/%s/%s.txt"%(meth,solid) )
            if meth=="Smith_Martin":    resDict = collect_SmithMartin(  "Solids/%s/%s.txt"%(meth,solid) )
            ret.append( {"name":solid, "d": 3, "n": vertexMap[solid], "method":meth, "results":resDict} )

    return ret

def plot_NvsTime_boxplot(data, dim=2):
    cutoff_time = 12*60*60
    methods = [
            ("Branch",       "#3349a1", "#828db5"), 
            #("SmithStar",    "#7c8929", "#b3bb7c"),
            ("SmithStar_DFS", "#7c8929", "#b3bb7c"),
            ("Smith_Martin", "#83387d", "#ab7fa8")
            #("SmithStar_DFS","#834a38", "#c49f79"),
            ]

    plotdata = [{'method': m, 'd': dim, 'color':c, 'fill': f} for (m,c,f) in methods]

    for pdict in plotdata:
        #Store times
        tmp = [ (dic['n'], dic['results']['times']) for dic in data if 
                dic['method']==pdict['method'] and 
                dic['d']==pdict['d'] ]
        tmp.sort(key=lambda x: x[0])
        tmp = [x[1] for x in tmp ]
        abovelimits = [len(filter(lambda v: v>=cutoff_time, x)) for x in tmp]
        tmp = [filter(lambda v: v<cutoff_time, x) for x in tmp]

        pdict['results'] = tmp
        pdict['n'] = dic['n']

        #Store succ's
        tmp = [ (dic['n'], dic['results']['succ']) for dic in data if 
                dic['method']==pdict['method'] and 
                dic['d']==pdict['d'] ]
        tmp.sort(key=lambda x: x[0])
        tmp = [x[1] for x in tmp ]

        #Subtract those that finished after 12h
        for i in range(0,len(tmp)):
            tmp[i] -= abovelimits[i]

        pdict['succ'] = tmp

    import numpy as np
    import matplotlib.pyplot as plt
    from matplotlib.patches import Polygon
    

    fig, ax1 = plt.subplots(figsize=(10,6))
    ax1.set_yscale('log')

    #Grid
    ax1.yaxis.grid(True, linestyle='-', which='major', color='lightgrey', alpha=0.5)
    ax1.set_axisbelow(True)
    
    
    ns = [10,11,12,13,14,15]

    for pdict in plotdata:

        offset = 0.25 * (plotdata.index(pdict)-(len(plotdata)-1)/2.0)
        pos = [ x+offset for x in ns ]
        #pos = range(plotdata.index(pdict), len(pdict['results'])*(len(methods)+1), len(methods)+1)
        #print pdict
        bp = plt.boxplot(pdict['results'], widths=0.21, positions = pos )
        plt.setp(bp['boxes'], color=pdict['color'])
        plt.setp(bp['whiskers'], color=pdict['color'], linestyle='solid')
        plt.setp(bp['fliers'], color=pdict['color'], marker='+')
        plt.setp(bp['caps'], color=pdict['color'])


        for i in range(len(pdict['results'])):
            box = bp['boxes'][i]
            boxX = []
            boxY = []
            for j in range(5):
                boxX.append(box.get_xdata()[j])
                boxY.append(box.get_ydata()[j])
            boxCoords = zip(boxX,boxY)
            boxPolygon = Polygon(boxCoords, facecolor=pdict['fill'])
            ax1.add_patch(boxPolygon)


        #Draw number of succesfully completed runs
        for n in ns:
            i = ns.index(n)
            ax1.text(pos[i]-0.05, 0.02, str(pdict['succ'][i]), fontsize=10)
        

    #Succesful label
    ax1.text(9.03, 0.02, "Completed:", fontsize=10)

    #Set tick positions and labels
    ax1.set_xticks([10,11,12,13,14,15])
    ax1.set_xticklabels(ns)

    #Horizontal line at cutoff time (12hours)
    plt.plot([0,20],[cutoff_time,cutoff_time], color='#a93030', linestyle='dashed' )
    
    #Hack to add legend
    handles = []
    labels  = []
    p, = plt.plot([1,1], linestyle='dashed', color='#a93030')
    handles.append(p)
    labels.append("Stop time")
    methodNames = ["Branch", "Smith*", "Smith", "Smith* (DFS)"]
    for pdict in plotdata:
        p, = plt.plot([1,1],linewidth=8.0, linestyle='-', color=pdict['fill'])
        handles.append(p)
        labels.append(methodNames[plotdata.index(pdict)])
    plt.legend( handles, labels, loc='upper left' )
    for p in handles:
        p.set_visible(False)
    
    #Axis labels
    #plt.title("Shell, d="+str(dim))
    plt.xlabel("Number of terminals")
    plt.ylabel("CPU-time (s)")
    
    #plt.xlim(-1,len(pdict['results'])*(len(methods)+1))
    plt.xlim(9,15.7)
    plt.ylim(0.01,100000)
    #plt.show()
    plt.savefig("plot_nvst_boxplot_d"+str(dim)+"_shell.pdf")



#data = collect_Shell()
#data = collect_Cube()
#for d in [2,3,4,5]:
#    plot_NvsTime_boxplot(data, dim=d)
#plot_NvsTime_boxplot(data,dim=4)

#results = []
#results.extend( collect_Cube() )
#results.extend( collect_Shell() )
#results.extend( collect_carioca() )
#results.extend( collect_Iowa() )
#results.extend( collect_Sausage() )
#results.extend( collect_Smith() )
#results.extend( collect_Solids() )

#for resDict in results:
#    print "%13s %3d %3d %13s %8.1f %8.1f %3d %3d"%(
#            resDict['name'],
#            resDict['d'],
#            resDict['n'],
#            resDict['method'],
#            resDict['results']['arithmean'],
#            resDict['results']['geomean'],
#            resDict['results']['succ'],
#            resDict['results']['fail'] )

#for resDict in results:
#    try:
#        print "%s;%d;%d;%s;%.1f;%.1f;%.1f;%.1f;%d;%d"%(
#            resDict['name'],
#            resDict['d'],
#            resDict['n'],
#            resDict['method'],
#            resDict['results']['arithmean'],
#            resDict['results']['geomean'],
#            resDict['results']['median'],
#            resDict['results']['std'],
#            resDict['results']['succ'],
#            resDict['results']['fail'])
#    except TypeError as e:
#        print e

#Print command for single-seed results
results = []
results.extend( collect_Sausage() )
results.extend( collect_Smith() )
results.extend( collect_Solids() )
import collections
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
            tableDict[rowEntry]['Smith_Martin'],
            tableDict[rowEntry]['length']
            )


