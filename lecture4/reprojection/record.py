import cv2 as cv
import numpy as np
from config import *
from scipy.spatial.transform import Rotation as R


def world2pixel(in_world, R_gimbal2world):
    R_world2camera = R_camera2gimbal.T @ R_gimbal2world.T
    t_world2camera = -R_camera2gimbal.T @ t_camera2gimbal
    in_pixel, _ = cv.projectPoints(
        in_world, R_world2camera, t_world2camera, camera_matrix, distort_coeffs
    )
    return in_pixel.squeeze(1)


def draw_text(img, text, anchor):
    cv.putText(img, text, anchor, cv.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255))


name = "2024-05-14_10-27-48"
datas = open(f"{name}.txt")
video = cv.VideoCapture(f"{name}.avi")

W = int(video.get(cv.CAP_PROP_FRAME_WIDTH))
H = int(video.get(cv.CAP_PROP_FRAME_HEIGHT))
FPS = video.get(cv.CAP_PROP_FPS)
fourcc = cv.VideoWriter_fourcc(*"MJPG")
out = cv.VideoWriter(f"{name}_output.avi", fourcc, FPS, (W, H))

HEIGHT = 0.4
GRID_NUM = 50
GRID_SIZE = 0.1
points = []
for x in range(GRID_NUM):
    for y in range(GRID_NUM):
        points.append(
            [x * GRID_SIZE, y * GRID_SIZE - GRID_NUM * GRID_SIZE / 2, -HEIGHT]
        )
points = np.array(points)


for i, data in enumerate(datas):
    _, w, x, y, z = map(float, data.split())
    q = R.from_quat([x, y, z, w])
    R_imubody2imuabs = q.as_matrix()
    R_gimbal2world = R_gimbal2imubody.T @ R_imubody2imuabs @ R_gimbal2imubody

    success, frame = video.read()
    if not success:
        break

    yaw, pitch, roll = R.from_matrix(R_gimbal2world).as_euler("ZYX", degrees=True)
    draw_text(frame, f"{yaw=:.1f}", (10, 30))
    draw_text(frame, f"{pitch=:.1f}", (10, 60))
    draw_text(frame, f"{roll=:.1f}", (10, 90))

    projected_points = world2pixel(points, R_gimbal2world)
    for projected_point in projected_points:
        x, y = projected_point
        if x < 0 or x > W or y < 0 or y > H:
            continue
        cv.circle(frame, np.int32(projected_point), 3, (255, 255, 255), -1)

    cv.imshow("result", frame)
    key = cv.waitKey(33)
    if key == ord("q"):
        break

    # out.write(frame)
    # if i > 2000:
    #     break

print("DONE")
