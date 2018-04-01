import sys
import telnetlib

FORWARD='w'
BACK='s'
LEFT='a'
RIGHT='d'
LOOK='l'
TALK='t'
ITEM1='1'

# t = telnetlib.Telnet('localhost', 7777)
inputs = '!' + FORWARD*5 + RIGHT + FORWARD + LEFT + FORWARD + RIGHT + FORWARD +  LEFT + FORWARD*3 + RIGHT
inputs += FORWARD*3 # right before the first click
# The easiest solution uses the seed 45.The other numbers are: 364058287
# To get there, we follow this path
# 1, 4, 7, 4, 7, 4, 7, 4, 
inputs += FORWARD*3 + (BACK + FORWARD)*2 + BACK
# 7, 4, 7, 6, 7, 6, 7, 8, 
inputs += FORWARD + BACK + FORWARD + RIGHT + (FORWARD +BACK)*2 + BACK
# 5, 8, 5, 8, 5, 8, 5, 8, 
inputs += RIGHT + (FORWARD + BACK) * 4
# 5, 8, 7, 8, 5, 2, 5, 2, 
inputs +=FORWARD + BACK + LEFT + FORWARD + BACK + RIGHT + FORWARD*2 + BACK + FORWARD
# 5, 2, 1, 0, 1, 0, 1, 0,
inputs+= BACK + FORWARD + LEFT + FORWARD*2 + (BACK + FORWARD)*2 
# 1, 2, 1
inputs += BACK + BACK + FORWARD

# Now we just need to press a 44 more times.
inputs += RIGHT + FORWARD*2 + (BACK + FORWARD)*41 + "!" + (BACK + FORWARD)*2 + FORWARD + FORWARD*10;
print inputs

exit()
t.write(inputs)
while True:
    sys.stdout.write(t.read_eager())
    sys.stdout.flush()
