#a script to run several replicates of several treatments locally

directory = "datafolder/"
seeds = range(1, 51)
# mutation_rates = [0.1 0.2, 0.3, 0.4, 0.5, 0.6, 0.7,]

import subprocess

def cmd(command):
    '''This wait causes all executions to run in sieries.                          
    For parralelization, remove .wait() and instead delay the                      
    R script calls unitl all neccesary data is created.'''
    return subprocess.Popen(command, shell=True).wait()

def silent_cmd(command):
    '''This wait causes all executions to run in sieries.                          
    For parralelization, remove .wait() and instead delay the                      
    R script calls unitl all neccesary data is created.'''
    return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).wait()

print("Copying MySettings.cfg to "+directory)
cmd("cp MySettings.cfg "+directory)

for a in seeds:
   # for b in mutation_rates: -MUTATION_RATE '+str(b)+'
        command_str = './native_project -SEED '+ str(a) +'  -FILE_PATH '+directory+' -FILE_NAME SP'+ str(a) +'.data'
        
        print(command_str)
        cmd(command_str)