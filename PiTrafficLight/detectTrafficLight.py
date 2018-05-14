# USAGE
# python detectTrafficLight.py

# import the necessary packages
from imutils.video import VideoStream
from imutils.video import FPS
import numpy as np
import argparse
import imutils
import time
import cv2

def shape_compare(c, frame):

	#turn curves into lines
	shape = cv2.arcLength(c, True)
	#Approximate polygon curves to get better idea on straight lines
	approx = cv2.approxPolyDP(c, 0.04 * shape, True)

	#Compare vertices of approximated shapes
	if len(approx) == 4:

		(x, y, w, h) = cv2.boundingRect(approx)
		ar = w / float(h)

		if ar >= 0.20 and ar <= 0.80:
			cropped_img = frame[x: x + w, y: y+h]

			maskGreen = cv2.inRange(cropped_img, lower_green_bound, upper_green_bound)
			green = cv2.countNonZero(maskGreen)
			if(green >= 50):
				print("GREEN LIGHT")
				return "GreenLight"

			maskRed = cv2.inRange(cropped_img, lower_red_bound, upper_red_bound)
			red = cv2.countNonZero(maskRed)
			if(red >= 50):
				print("RED LIGHT")
				return "RedLight"

			return "rectangle"

# load our serialized model from disk
print("[INFO] loading model...")

# initialize the video stream, allow the cammera sensor to warmup,
# and initialize the FPS counter
print("[INFO] starting video stream...")

#comment first line and uncomment second one to use the PI camera module instead of laptops webcam
vs = VideoStream(src=0).start()
# vs = VideoStream(usePiCamera=True).start()

lower_green_bound = np.array([0, 175, 0], dtype = "uint8")
upper_green_bound = np.array([100, 255, 100], dtype = "uint8")

lower_red_bound = np.array([0, 0, 175], dtype = "uint8")
upper_red_bound = np.array([100, 100, 255], dtype = "uint8")


time.sleep(2.0)
fps = FPS().start()

# loop over the frames from the video stream
while True:
	# grab the frame from the threaded video stream and resize it
	# to have a maximum width of 400 pixels
	frame = vs.read()
	frame = imutils.resize(frame, width=400)

	# grab the frame dimensions and convert it to a blob
	(h, w) = frame.shape[:2]
	blob = cv2.dnn.blobFromImage(cv2.resize(frame, (300, 300)),
		0.007843, (300, 300), 127.5)

	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	blurred = cv2.GaussianBlur(gray, (5, 5), 0)
	thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]

	_, contours, _ = cv2.findContours(thresh.copy(), cv2.RETR_TREE,
		cv2.CHAIN_APPROX_SIMPLE)

	#Draw each contour we find
	for c in contours:

		#Calculate center of contours
		moment = cv2.moments(c)
		if (moment["m00"] == 0):
			moment["m00"]=1
		cX = int(moment["m10"] / moment["m00"])
		cY = int(moment["m01"] / moment["m00"])

		#Call the method we made above to decide what the shape of a contour is
		thishape = shape_compare(c, frame)

		if(thishape == "RedLight" or thishape == "GreenLight"):
			cv2.drawContours(frame, [c], -1, (0, 255, 0), 2)

			cv2.putText(frame, thishape, (cX - 20, cY - 20),
				cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)



	# show the output frame
	cv2.imshow("Frame", frame)
	key = cv2.waitKey(1) & 0xFF

	# if the `q` key was pressed, break from the loop
	if key == ord("q"):
		break

	# update the FPS counter
	fps.update()

# stop the timer and display FPS information
fps.stop()
print("[INFO] elapsed time: {:.2f}".format(fps.elapsed()))
print("[INFO] approx. FPS: {:.2f}".format(fps.fps()))

# do a bit of cleanup
cv2.destroyAllWindows()
vs.stop()
