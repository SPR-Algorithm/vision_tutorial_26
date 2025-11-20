import cv2
import sys

def play_video(video_path, start_frame):
    cap = cv2.VideoCapture(video_path)
    cap.set(cv2.CAP_PROP_POS_FRAMES, start_frame)

    while cap.isOpened():
        success, frame = cap.read()
        if not success:
            break

        cv2.imshow('video', frame)

        key = cv2.waitKey(33) & 0xFF
        if key == ord('q'):
            break

if __name__ == "__main__":
    video_path = '2024-05-14_10-27-48.avi'
    start_frame = int(sys.argv[1]) if len(sys.argv) > 1 else 0
    play_video(video_path, start_frame)
