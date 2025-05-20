import serial
import time
import numpy as np
import cv2

# Open the serial connection to Arduino (adjust COM port as needed)
arduinoData = serial.Serial('COM4', 9600)
time.sleep(2)  # Wait for Arduino to initialize

# Function to send center coordinates of the face
def send_coordinates_to_arduino(x, y, w, h):
    center_x = x + w // 2
    center_y = y + h // 2
    coordinates = f"{center_x},{center_y}\r"
    arduinoData.write(coordinates.encode())
    print(f"Sent: X={center_x}, Y={center_y}")

# Initialize webcam (0 = default camera)
capture = cv2.VideoCapture(0)

# Load Haar cascade classifier for face detection
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

while True:
    isTrue, frame = capture.read()
    if not isTrue:
        break

    # Convert frame to grayscale for face detection
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Detect faces
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=6, minSize=(100, 100))
    
    for (x, y, w, h) in faces:
        # Draw rectangle around the face
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
        send_coordinates_to_arduino(x, y, w, h)
        break  # Only send the first detected face per frame

    # Show the video feed with rectangles
    cv2.imshow('Face Tracking', frame)

    # Press 'd' to exit
    if cv2.waitKey(1) & 0xFF == ord('d'):
        break

# Release camera and close windows
capture.release()
cv2.destroyAllWindows()
arduinoData.close()
