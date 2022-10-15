import socket
class Client:
    def __init__(self, ip, port):
        self.connectionRequest = "Connection Requested"
        self.bytesToSend = str.encode(self.connectionRequest)
        self.serverAddressPort = (ip, port)
        self.bufferSize = 32

    def initClient(self):
        # Create a UDP socket at client side
        udp = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
        print("[Client]: ", self.connectionRequest)

        # Send to server using created UDP socket
        udp.sendto(self.bytesToSend, self.serverAddressPort)
        connectionResponce = udp.recvfrom(self.bufferSize)
        connectionGrant = format(connectionResponce[0])[2:-1]

        print("[Server]: ", connectionGrant)

        return udp
