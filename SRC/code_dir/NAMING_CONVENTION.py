#!/usr/bin/python
#



import sys

PHASE = sys.argv[1]
def short_cut_phase_name(PHASE):
    if PHASE in my_dist:
        return my_dist[PHASE]
    else:
        return PHASE



my_dist = { "S": "S",
"ScS":"ScS",
"SS":"SS",
"SSS":"S3",
"ScSScS":"ScS2",
"Sdiff":"Sdiff",
"SSSm":"S3m",
"SSSS":"S4",
"SSSSm":"S4m",
"SSSSS":"S5",
"SSSSSm":"S5m",
"SSSSSS":"S6",
"SSSSSSm":"S6m",
"ScSScSScS":"ScS3",
"ScSScSScSm":"ScS3m",
"ScSScSScSScS":"ScS4",
"ScSScSScSScSm":"ScS4m",
"ScSScSScSScSScS":"ScS5",
"ScSScSScSScSScSm":"ScS5m"
}


## check if is depth phase
first_ch = PHASE[0]
if first_ch is "p" or first_ch is "s":
    #print "is depth phase"
    #print PHASE[1:]
    after_convert = short_cut_phase_name(PHASE[1:])
    print first_ch+str(after_convert)

    # not depth phase, print origional phase name
else:
    print short_cut_phase_name(PHASE)
    





