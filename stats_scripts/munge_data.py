folder = ''

treatment_postfixes = [1]
reps = range(1, 51)
header = "uid treatment rep update total_orgs EQU COMPLEX\n"

outputFileName = "munged_basic.dat"

outFile = open(outputFileName, 'w')
outFile.write(header)

for t in treatment_postfixes:
    for r in reps:
        fname = folder +"Org_Vals" + str(r) + "SP" + str(r) + ".data"
        uid = str(t) + "_" + str(r)
        curFile = open(fname, 'r')
        for line in curFile:
            if (line[0] != "u"):
                splitline = line.split(',')
                outstring1 = "{} {} {} {} {} {} {}\n".format(uid, t, r, splitline[0], splitline[4], splitline[13], splitline[14])
                outFile.write(outstring1)
        curFile.close()
outFile.close()