import subprocess
import os
import cv2
from sourceCode2_with_logs import RBFAlgorithm
import time


def extract_frames(video_path, output_dir):
    """
    Extracts all frames from `video_path` into JPG files inside `output_dir`.
    Filenames will be frame_00001.jpg, frame_00002.jpg, ...
    """
    os.makedirs(output_dir, exist_ok=True)
    subprocess.run([
        "ffmpeg", "-i", video_path,
        os.path.join(output_dir, "frame_%05d.jpg")
    ], check=True)


def process_frame_rbfa(input_path, output_path):
    """
    Placeholder for the RBFA algorithm on a single frame.
    Currently: convert to grayscale then back to BGR.
    Replace with your real RBFA implementation.
    """
    print(input_path)
    print(output_path)

    obj1 = RBFAlgorithm(input_path)
    start = time.time()
    obj1.Enhance()
    # obj1.showOutput()
    end = time.time()
    obj1.saveOutput(output_path)

    elapsed = end - start
    print(f"Elapsed time: {elapsed:.2f} seconds")

    print(f'{output_path} has enhanced successfully')

def process_all_frames(input_dir, output_dir):
    """
    Runs `process_frame_rbfa` on every JPG in `input_dir`,
    writing results to `output_dir` with the same filenames.
    """
    os.makedirs(output_dir, exist_ok=True)
    for fname in sorted(os.listdir(input_dir)):
        if not fname.lower().endswith(".png"):
            continue
        in_path  = os.path.join(input_dir,  fname)
        out_path = os.path.join(output_dir, fname)
        process_frame_rbfa(in_path, out_path)


def assemble_video(frames_dir, output_video, fps=25):
    """
    Takes all JPGs named frame_00001.jpg, frame_00002.jpg, ... in `frames_dir`
    and assembles them into `output_video` at `fps` frames per second.
    """
    ffmpeg_path = r"C:\ffmpeg\bin\ffmpeg.exe"  # wherever you unzipped ffmpeg
    subprocess.run([
        ffmpeg_path,
        "-framerate", str(fps),
        "-i", os.path.join(frames_dir, "frame_%05d.png"),
        "-c:v", "libx264",
        "-pix_fmt", "yuv420p",
        output_video
    ], check=True)


#input_dir = "device_input_frames/v200_input_frames/car_parking"
#output_dir = "device_enhanced_frames/v200_enhanced_frames/tree_can_Expect_better_results"
#test2video = "test2video"
#process_all_frames(input_dir, output_dir)
#assemble_video(output_dir,"device_enhanced_videos/v200_enhanced_videos/tree_can_Expect_better_results.mp4",fps=25)

process_frame_rbfa("device_input_frames/v200_input_frames/stairs/frame_00003.png","device_enhanced_frames/v200_enhanced_frames/stairs/frame_00003.png")
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-213457UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-213457UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-213531UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-213531UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-213611UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-213611UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-214155UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-214155UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-214801UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-214801UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-215222UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-215222UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-215257UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-215257UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-215357UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-215357UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-215452UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-215452UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-215538UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-215538UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-220555UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-220555UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-220625UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-220625UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-220714UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-220714UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-220750UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-220750UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-221247UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-221247UTC-0500.mp4",fps=25)
#assemble_video( "device_enhanced_frames/Pauls V200 Low-Light Videos/PSS-20250625-221323UTC-0500","device_enhanced_videos/Pauls V200 enhanced Videos/PSS-20250625-221323UTC-0500.mp4",fps=25)







