from pythonosc import udp_client
import sys
import math
import time

if __name__ == "__main__":

    if len(sys.argv) != 3:
        print("Usage: python controller_template.py <ip> <port>")
        sys.exit(1)

    ip = sys.argv[1]  # Get IP from command line arguments
    port = int(sys.argv[2])  # Get port from command line arguments
    client = udp_client.SimpleUDPClient(ip, port)

    # Enable the robots
    address = "/motion/enable"
    client.send_message(address, True)
    print(f"Sent message: {address}/True")

    # Oscillate the z position of the robot
    count = 1000
    step = 0.01
    for i in range(count):
        address = "/motion/pos"
        x = 0
        y = .75
        # map the sin function to the range [0, 1]
        z = math.sin(i * step) * 0.5 + .5
        # Send the message to Robot 0
        message = [0, x, y, z]
        client.send_message(address, message)
        # Send the message to Robot 1
        message = [1, x, y, z]
        client.send_message(address, message)
        print(f"Sent message: {address}/{message}")
        time.sleep(0.01)

    # Reset the robots
    address = "/motion/reset"
    message = [0, 0]  # Reset Robot 0
    client.send_message(address, 0)
    message = [0, 0] # Reset Robot 1
    client.send_message(address, 0)
    print(f"Sent message: {address}/{message}")

    # Wait fothe the robots to move to the reset position
    time.sleep(2.0)
    
    # Disable the robots
    address = "/motion/enable"
    client.send_message(address, False)
    print(f"Sent message: {address}/False")



    