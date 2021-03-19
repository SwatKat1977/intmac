from Common.RawSocket.IPV4SocketLayer import IPV4SocketLayer 



#from IPV4Socket import IPV4Socket
#from IPV4SocketLayer import IPV4SocketLayer 
#import time


def main():

	print("[INFO] Starting server...")

	socketLayer = IPV4SocketLayer()
	
	# Create the server master socket
	serverSocket = socketLayer.CreateSocket()
		
	# Set the socket to non-blocking
	serverSocket.SetBlocking(False)
	
	serverSocket.Bind("localhost", 3490)

	serverSocket.Listen(5)

	recvStr = ""
	sendStr = '["0.0", "0014", "0000"]'
	hasWritten = False

	while True:
		acceptedSocket = serverSocket.Accept(.500)

		if acceptedSocket != None:
			print("[INFO] A new connection has been received....")

		if hasWritten == False and acceptedSocket != None:
			writtenBytes, errCode = acceptedSocket.Write(sendStr)

			print('[INFO] Written {0} out of {1} bytes'.format(writtenBytes, len(sendStr)))

			if writtenBytes == len(sendStr):
				hasWritten = True 
			else: 
				hasWritten = False

	
main()
