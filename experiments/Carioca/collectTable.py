
d = 3
for n in range(11,21):
    for s in range(1,6):
        len_branch = ""

        rmt_smith_orig = ""
        rmt_smith_star = ""
        rmt_branch = ""

        time_smith_orig = ""
        time_smith_star = ""
        time_branch = ""

        fName = "Branch/carioca_"+str(d)+"_"+str(n)+"_0"+str(s)+".txt"
        print fName
        with open(fName) as f:
            contents = f.readlines()
            for line in contents:
                if "Primal" in line:
                    len_branch = line.split(" ")[-1]
                if "Took" in line:
                    time_branch = line.split(" ")[1]

        print n,s, len_branch, time_branch


