#!/usr/bin/python

from collectdata import *

def plot_NvsTime_boxplot(data, setName, dim=2):
    cutoff_time = 12*60*60
    methods = [
            ("Branch",       "#3349a1", "#828db5"), 
            #("SmithStar",    "#7c8929", "#b3bb7c"),
            ("SmithStar_DFS", "#7c8929", "#b3bb7c"),
            ("SmithFixed", "#83387d", "#ab7fa8")
            #("SmithStar_DFS","#834a38", "#c49f79"),
            ]

    #Convert data
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
    plt.savefig("plot_nvst_boxplot_d"+str(dim)+"_"+setName+".pdf")



data = collect_Cube()
for d in [2,3,4,5]:
    plot_NvsTime_boxplot(data, "Cube", dim=d)

data = collect_Shell()
for d in [2,3,4,5]:
    plot_NvsTime_boxplot(data, "Shell", dim=d)
