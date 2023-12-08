import cv2
import socket
import pickle
import struct

def send_webcam():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((RECEIVER_IP, RECEIVER_PORT))

    cap = cv2.VideoCapture(0)

    while True:
        ret, frame = cap.read()

        # Serialize the frame
        data = pickle.dumps(frame)
        message_size = struct.pack("L", len(data))

        # Send the frame size followed by the frame itself
        client_socket.sendall(message_size + data)

    cap.release()
    client_socket.close()

RECEIVER_IP = input("Enter Receiver's IP: ")
RECEIVER_PORT = 5555
send_webcam()
