import os
import subprocess
import argparse

def extract_frames(video_path: str, output_dir: str, image_ext: str = "png"):
    """
    Extract all frames from `video_path` into image files inside `output_dir`.
    Filenames will be: frame_00001.jpg, frame_00002.jpg, etc.

    Args:
      video_path:   Path to the input video file.
      output_dir:   Directory where frames should be written.
      image_ext:    File extension for each frame (e.g. "jpg" or "png").
    """
    os.makedirs(output_dir, exist_ok=True)

    # Build the output template. For ".jpg", it'll be "frame_%05d.jpg".
    template = os.path.join(output_dir, f"frame_%05d.{image_ext}")

    # Call ffmpeg. Requires "ffmpeg" on your PATH (or change to full path).
    ffmpeg_path = r"C:\ffmpeg\bin\ffmpeg.exe"
    subprocess.run(
        [ffmpeg_path , "-i", video_path, template],
        check=True
    )

#extract_frames("device_outputs/vb400_outputs/Day/office_standing.mp4" , "device_input_frames/vb400_input_frames/Day/office_standing"  )
extract_frames("device_outputs/Pauls V200 Low-Light Videos-20250626T050814Z-1-001/Pauls V200 Low-Light Videos/PSS-20250625-213457UTC-0500.mp4" , "device_input_frames/Pauls V200 Low-Light Videos/PSS-20250625-213457UTC-0500"  )


'''
import cv2
from PIL import Image
import os

def extract_frames_rgb(video_path, output_dir):
    """
    Opens `video_path`, reads each frame, converts it to RGB,
    then writes it out as a PNG in `output_dir`.
    """
    # 1) Make sure the output folder exists
    os.makedirs(output_dir, exist_ok=True)

    # 2) Open the video file
    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print(f"Error: cannot open video {video_path}")
        return

    frame_idx = 0
    while True:
        ret, frame_bgr = cap.read()
        if not ret:
            break  # no more frames

        # 3) Convert BGR → RGB
        #frame_rgb = cv2.cvtColor(frame_bgr, cv2.COLOR_BGR2RGB)

        # 4) Wrap as a PIL Image (which expects RGB)
        img = Image.fromarray(frame_bgr)

        # 5) Save as PNG (preserves exact RGB)
        filename = os.path.join(output_dir, f"frame_{frame_idx:05d}.png")
        img.save(filename)

        frame_idx += 1

    cap.release()
    print(f"Extracted {frame_idx} frames (BGR) → '{output_dir}'.")


if __name__ == "__main__":
    video_file = "v200_outputs/v200_outputs/tree_can_Expect_better_results.mp4"     # ← replace with your .mp4 path
    save_folder = "tree_can_Expect_better_results_inputframes2"  # ← where PNGs will be written
    extract_frames_rgb(video_file, save_folder)
    
'''
