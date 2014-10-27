#! /usr/bin/env python

# omxplay is a simple python interface to omxplayer
# Version 1.03 9/7/2012
# ------------------------------------

# pyomxplayer.py AND omxchild.sh must be in the same directory
# to run do:  python pyomxplayer.py from a terminal opened in the same directory
# track to play is a file name e.g. my_track.mp4

# developed with python 2.7, not tried on python 3.
# +,- commands do not work in Wheezy beta 18/08 unless you have installed a later version of omxplayer.
# videos must be in a directory set in the variable videodir in main program below.
# you need a long video or else the controls will not work (bug in omxplayer)
# Running this with Idle produces funny results, just run from a terminal.

# Changes from version 1.02
#     simple error checking on user input
#     check omxchild.sh exists and make it executable


# Changes from version 1.01
#     rewritten to use python's subprocess module.
#     reorganised code into functions so omx_play could be cut and pasted into a better program.
#     now works if the video runs to its end.


# Bugs and Features
# quitting (q) a short video leaves omxchild running until pyomx is exited (may be related to omxplayer bug)
# would like to put the FIFO in the cwd but if so how to communicate this to omxchild.sh


import subprocess, os, time, sys, select
import RPi.GPIO as GPIO
_DEBUG=False
# function to play a track
global gFileName
def omx_play(omxoptions, track):
    '''
    # KEYBOARD
    def keyboard_poll():
        i,o,e = select.select([sys.stdin],[],[],0)
        for s in i:
            if s == sys.stdin:
                input = sys.stdin.readline().rstrip()
                return input
        return None
    # OMXCHILD
    '''
    def gpio_poll():
        global gFileName
        if GPIO.input(4) == True:
            print "4 triggered" 
            return "p"
        if GPIO.input(17) == True:
            print "17 triggered" 
            gFileName = "mov1.mp4"
            return "q"
        if GPIO.input(18) == True:
            print "18 triggered" 
            gFileName = "mov3.mp4"
            return "q"
        if GPIO.input(22) == True:
            print "22 triggered" 
            gFileName = "mov2.mp4"
            return "q"
        if GPIO.input(23) == True:
            gFileName = "mov4.mp4"
            print "23 triggered"
            return "q"
        if GPIO.input(24) == True:
            gFileName = "mov5.mp4"
            print "24 triggered" 
            return "q"
        if GPIO.input(25) == True:
            gFileName = "mov6.mp4"
            print "25 triggered"
            return "q"
        if GPIO.input(21) == True:
            print "21 triggered" 
            os.system("reboot")



    def child_start(cwd):
    # create the subprocess which will call omxplayer
    # a subprocess is needed because omxplayer controls must be piped
    # if I specify omxplayer  as the first argument of Popen then
    # I could not get the control commands to it hence the child is a shell script
    # which calls omxplayer.

        proc=subprocess.Popen([cwd + '/omxchild.sh'], shell=False, \
              stderr=subprocess.PIPE, \
              stdout=subprocess.PIPE, \
              stdin=subprocess.PIPE)
        pid = proc.pid
        if debug: print "PID: ", pid
        
        # wait until child has started otherwise commands sent get confused
        while proc.poll()!= None:
            time.sleep(0.1)
        if debug: print 'Subprocess ',pid," started"
        return proc

    # poll child process to see if it is running
    def child_running(proc):
            if proc.poll() != None:
                return False
            else:
                return True

    # wait for the child process to terminate
    def child_wait_for_terminate(proc):
        while proc.poll()!= None:
            time.sleep(0.1)
        if debug: print 'Child terminated'

    # send a command to the child
    def child_send_command (command, proc ):
        if debug: print "To child: "+ command
        proc.stdin.write(command)

    # not used
    def child_get_status():
        opp=proc.stdout.readline()
        print "From child:  " + opp


    # PIPE TO OMXPLAYER

    def omx_send_control(char,fifo):
        command="echo -n " + char + ">" + fifo
        if debug: print "To omx: " + command
        os.system(command)



    if _DEBUG == True:
        import pdb
        pdb.set_trace()
    # current working directory
    cwd= os.getcwd()
    
    # create a FIFO to be used between the parent and omxplayer
    fifo = "/tmp/omxcmd"
    if os.path.exists (fifo): os.system("rm " + fifo )
    os.system("mkfifo " + fifo )

    # start omxchild the child process that will run omxplayer
    proc=child_start(cwd)

    #send a 'track' command to the child to start the video
    child_send_command("track " + omxoptions + " " + track + "\n", proc )

    # and send the start command to omxplayer through the FIFO
    omx_send_control('.',fifo)

    # loop obtaining user input and send to omxplayer
    while True:
        # has the child process terminated because the track has ended
        if child_running(proc) == False:
                if debug: print "child now not running"
                # close the FIFO
                os.system("rm " + fifo)
                return
            
        # potential race condition here as omxplayer may terminate here
        # so sending the control will fail
            
        # poll the keyboard for a control
        # this will be replaced by polling the gpio inputs in my project
        #key = keyboard_poll ()            # you need to press [enter] after the command.
        
        key = gpio_poll ()            # you need to press [enter] after the command.
        if key != None:
            # send the control to omxplayer through the FIFO
            omx_send_control(key,fifo)
            if key == 'q':
                # q has been sent and omxplayer is terminating, wait for child to terminate
                # and then close the FIFO
                child_wait_for_terminate(proc)
                os.system("rm " + fifo)
                return
            
        time.sleep(0.1)





# FUNCTIONS FOR MAIN

def get_track():
    print("Track to Play, help, or bye to exit")
    #track = sys.stdin.readline()
    #track = track.rstrip()
    track = gFileName 
    print "get_track", track 
    return track


def show_help ():
    print (" To control video type a character followed by [ENTER]")
    print (" p - pause/play\n spacebar - pause/play\n q - quit\n + - increase volume\n - - decrease volume")
    print (" z - tv show info\n 1 - reduce speed\n 2 - increase speed\n j - previous audio index\n k - next audio index\n i - back a chapter\n o - forward a chapter")
    print (" n - previous subtitle index\n m - next subtitle index\n s - toggle subtitles\n >cursor - seek forward 30\n <cursor - seek back 30\n ^cursor - seek forward 600\n _^cursor - seek back 600")


def check_runnable():
    # current working directory
    cwd= os.getcwd()
    
    path = cwd + "/omxchild.sh"
    if os.path.exists(path) == False:
	print "omxchild.sh not found, download from github"
        sys.exit()
    command = "chmod +x " + path
    os.system(command) 

# check user input is OK
def check_input (videodir,track):
    if '.' in track:
        path = videodir + track
        if os.path.exists(path) == True:
            return True
        else:
            print "File " + path + " not found"
    else:
        if (track == "help") or (track == "bye"):
            return True
        else:
            print track, " not understood"
            return False


#Rsspberry board GPIO pins functions setting
def set_gpio():
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)

    GPIO.setup(17, GPIO.IN)
    GPIO.setup(18, GPIO.IN)
    GPIO.setup(22, GPIO.IN)
    GPIO.setup(23, GPIO.IN)
    GPIO.setup(24, GPIO.IN)
    GPIO.setup(25, GPIO.IN)
    GPIO.setup(27, GPIO.IN)
    GPIO.setup(4,  GPIO.IN)
    GPIO.setup(21,  GPIO.IN)
# MAIN
# change videodir to where you store your videos. Must be a path from root
videodir ="/winston/cinema/"
debug = True


print "******* Welcome to pyomxplayer *******"

# check all is OK to run
check_runnable()

#configure raspberry gpio pin as input
set_gpio()
# ask where to send the sound
#while True:
#    print ("Output, hdmi or local")
#    op = sys.stdin.readline().rstrip()
#    if (op == "hdmi") or (op == "local"):
#        omxoptions = "-o" + op
#        break
#    else:
#        print op + " not understood"
omxoptions = "-o hdmi"
gFileName = "mov7.mp4"    
# loop to get a track and play it
while True:
    track = get_track()
    if check_input(videodir, track) == False:
        continue
    if track == "help":
        show_help()
        continue
    if track == "bye": break
    omx_play(omxoptions, videodir + track)

# say goodbye
print ("sweet dreams")
sys.exit()
