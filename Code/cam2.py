import cv2
import socket
import pickle
import struct

def receive_webcam():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('0.0.0.0', RECEIVER_PORT))
    server_socket.listen(5)
    print(f"Listening on 0.0.0.0:{RECEIVER_PORT}")

    conn, addr = server_socket.accept()
    data = b""

    while True:
        while len(data) < payload_size:
            data += conn.recv(4096)

        packed_msg_size = data[:payload_size]
        data = data[payload_size:]
        msg_size = struct.unpack("L", packed_msg_size)[0]

        while len(data) < msg_size:
            data += conn.recv(4096)

        frame_data = data[:msg_size]
        data = data[msg_size:]

        # Deserialize the frame
        frame = pickle.loads(frame_data)
        cv2.imshow('Received Webcam Feed', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cv2.destroyAllWindows()

RECEIVER_PORT = 5555
payload_size = struct.calcsize("L")
receive_webcam()
