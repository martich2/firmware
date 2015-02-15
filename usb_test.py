# must be 2.7 compatible
import serial

command_list = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'L', 'Q']
port = "/dev/ttyACM0"
ser = serial.Serial(port, 9600)

print
print "TekVPI Test Fixture Command Interface 1.0"
print "Commands are A thru L and Q for quit"
print

cmd = raw_input("Enter Command> ").upper()
while cmd != 'Q':

    if cmd not in command_list:
        print "invalid command"
    else:
        ser.write(cmd) 
        print "command sent"

    cmd = raw_input("Enter Command> ").upper()

print "Good Bye..."
ser.close()
