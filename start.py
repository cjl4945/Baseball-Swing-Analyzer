from flask import Flask
from flask_socketio import SocketIO, emit
import logging
import socket
import os
import sys
import time
import math

# Flask Setup
app = Flask(__name__, static_url_path='')
app.logger.disabled = True
log = logging.getLogger('werkzeug')
log.disabled = True
connected = False
shoulder_absAngle, shoulder_absmax_Speed, hip_absAngle, hip_absmax_Speed = 0,0,0,0



# SocketIO Setup
socketio = SocketIO(app)


# SocketIO Connection Events
@socketio.on('connect')
def handle_connect():
    print('Client Connected')

@socketio.on('disconnect')
def handle_disconnect():
    print('Client Disconnected')


# SocketIO Listener Events
@socketio.on('calibrate')
def handle_calibrate():
    print('Calibrating now...')

    # After Calibrating update client
    emit('calibrate')

@socketio.on('start')
def handle_start():
    print('Gathering data...')
    
    shoulder_absAngle, shoulder_absmax_Speed, hip_absAngle, hip_absmax_Speed = get_data()
    shoulder_absAngle, shoulder_absmax_Speed, hip_absAngle, hip_absmax_Speed = MetricScoring(float(shoulder_absAngle), float(shoulder_absmax_Speed), float(hip_absAngle), float(hip_absmax_Speed))
    # Example data (this is the format we need)
    data = {
        "hipData": {
            "hipRotation": float(f'{hip_absAngle:.2f}'),
            "hipVelocity": float(f'{hip_absmax_Speed:.2f}'),
        },
        "shoulderData": {
            "shoulderRotation": float(f'{shoulder_absAngle:.2f}'),
            "shoulderVelocity": float(f'{shoulder_absmax_Speed:.2f}')
        }
    }
    # After getting data return it to client
    emit('metrics', data)
    print('Data sent to client')

# Flast Routes
@app.route('/')
def home():
    return app.send_static_file('index.html')


def local_socket():
    host = socket.gethostname()  # as both code is running on same pc
    port = 12345  # socket server port number
    print(host)
    global client_socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # instantiate
    client_socket.connect(("127.0.0.1", port))  # connect to the server
    print("Connected")

def get_data():
#     s = socket.socket(socket.AF_INET, socket.STREAM)
#     s.connect(("localhost", 8080))
#     s.send("message here")
#     # get data back
#     s.disconnect()
    #return data
    global connected
    if not connected:
        local_socket()
        connected = True
    

    message = "p"  # take input
    goodMsg = "good"
    goMsg = client_socket.recv(2).decode()  # send message
    print("Message Recieved")
    print (goMsg.lower().strip() != "go")
    while goMsg.lower().strip() != "go":
        print(goMsg.lower().strip())
        goMsg = client_socket.recv(2).decode()
        continue
    print("recieved go")
    client_socket.send(message.encode())
    for x in range(0,2):
        data = client_socket.recv(100).decode().strip().split()# receive response
        print(data)
        
#         while (data[0] != "hip"):
#             data = client_socket.recv(100).decode().strip().split()
#             print(data)
        if (data[0] == "hip"):
            hip_absAngle = data[1]
            print(hip_absAngle)
            hip_absmax_Speed = data[2]
            print(hip_absmax_Speed)
            
        elif (data[0] == "shoulder"):
            shoulder_absAngle = data[1]
            print(shoulder_absAngle)
            shoulder_absmax_Speed = data[2]
            print(shoulder_absmax_Speed)
                
        client_socket.send(goodMsg.encode())
        time.sleep(2)
    
      

#     print('Received from server: ' + data)  # show in terminal
    
    return shoulder_absAngle, shoulder_absmax_Speed, hip_absAngle, hip_absmax_Speed
#     message = input(" -> ")  # again take input


#     Metric Scoring
def MetricScoring(shoulder_absAngle, shoulder_absmax_Speed, hip_absAngle, hip_absmax_Speed):
    if shoulder_absAngle < 125.00:
        shoulder_absAngle = shoulder_absAngle/125 * 2
    elif shoulder_absAngle > 165.00:
        shoulder_absAngle = 10
    else:
        shoulder_absAngle = (.2 * shoulder_absAngle) - 23
    
    if shoulder_absmax_Speed > 1000.00:
        shoulder_absmax_Speed = 10
    else:
        shoulder_absmax_Speed = shoulder_absmax_Speed / 100
       
    if hip_absAngle < 60.00:
        hip_absAngle = hip_absAngle/60 * 2
    elif hip_absAngle > 100:
        hip_absAngle = 10
    else:
        hip_absAngle = (.2 * hip_absAngle) - 10
        
    if hip_absmax_Speed < 250.00:
        hip_absmax_Speed = hip_absmax_Speed/250 * 2
    elif hip_absmax_Speed > 650:
        hip_absmax_Speed = 10
    else:
        hip_absmax_Speed = (.02 * hip_absmax_Speed) - 3
    return shoulder_absAngle, shoulder_absmax_Speed, hip_absAngle, hip_absmax_Speed

if __name__ == '__main__':
    socketio.run(app, host = "0.0.0.0", port=80, debug = True) # You can set it to 80 like you orininally had
#     get_data()
    
    #local_socket()