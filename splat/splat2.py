#!/usr/bin/env python3
"""
MP4 to 3D Gaussian Splat using gsplat library
More direct approach using the latest gsplat implementation
"""

import torch
import numpy as np
from pathlib import Path
import cv2
import argparse
from tqdm import tqdm


def setup_environment():
    """Check for CUDA and setup device"""
    if torch.cuda.is_available():
        device = torch.device("cuda:0")
        print(f"Using GPU: {torch.cuda.get_device_name(0)}")
    else:
        device = torch.device("cpu")
        print("Warning: CUDA not available, using CPU (will be slow)")
    return device


def extract_frames_opencv(video_path, output_dir, max_frames=None, fps=None):
    """Extract frames from video using OpenCV"""
    output_dir = Path(output_dir)
    images_dir = output_dir / "images"
    images_dir.mkdir(parents=True, exist_ok=True)
    
    cap = cv2.VideoCapture(str(video_path))
    video_fps = cap.get(cv2.CAP_PROP_FPS)
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    
    print(f"Video FPS: {video_fps}, Total frames: {total_frames}")
    
    # Calculate frame skip
    if fps:
        frame_skip = int(video_fps / fps)
    else:
        frame_skip = 1
    
    if max_frames:
        frame_skip = max(frame_skip, total_frames // max_frames)
    
    frames = []
    frame_paths = []
    frame_count = 0
    saved_count = 0
    
    print("Extracting frames...")
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        
        if frame_count % frame_skip == 0:
            output_path = images_dir / f"frame_{saved_count:04d}.png"
            cv2.imwrite(str(output_path), frame)
            frames.append(frame)
            frame_paths.append(output_path)
            saved_count += 1
        
        frame_count += 1
    
    cap.release()
    print(f"Extracted {saved_count} frames to {images_dir}")
    return frames, frame_paths, images_dir


def run_colmap_sfm(images_dir, output_dir):
    """Run COLMAP Structure from Motion"""
    import subprocess
    
    output_dir = Path(output_dir)
    database_path = output_dir / "database.db"
    sparse_dir = output_dir / "sparse"
    sparse_dir.mkdir(parents=True, exist_ok=True)
    
    print("Running COLMAP feature extraction...")
    subprocess.run([
        'colmap', 'feature_extractor',
        '--database_path', str(database_path),
        '--image_path', str(images_dir),
        '--ImageReader.single_camera', '1',
        '--ImageReader.camera_model', 'OPENCV'
    ], check=True)
    
    print("Running COLMAP feature matching...")
    subprocess.run([
        'colmap', 'exhaustive_matcher',
        '--database_path', str(database_path)
    ], check=True)
    
    print("Running COLMAP mapper...")
    subprocess.run([
        'colmap', 'mapper',
        '--database_path', str(database_path),
        '--image_path', str(images_dir),
        '--output_path', str(sparse_dir)
    ], check=True)
    
    print("COLMAP reconstruction complete!")
    return sparse_dir


def main():
    parser = argparse.ArgumentParser(
        description='Convert MP4 to 3D Gaussian Splat using gsplat'
    )
    parser.add_argument('video_path', type=str, help='Input MP4 video')
    parser.add_argument('--output-dir', type=str, default='./output_gsplat')
    parser.add_argument('--fps', type=int, default=5, 
                       help='Extract frames at FPS (default: 5)')
    parser.add_argument('--max-frames', type=int, default=100,
                       help='Maximum frames to extract (default: 100)')
    
    args = parser.parse_args()
    
    # Setup
    device = setup_environment()
    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    
    # Extract frames
    frames, frame_paths, images_dir = extract_frames_opencv(
        args.video_path, 
        output_dir, 
        max_frames=args.max_frames,
        fps=args.fps
    )
    
    print("\n" + "="*50)
    print("Frame extraction complete!")
    print("="*50)
    print(f"\nNext steps:")
    print(f"1. Run COLMAP on the extracted frames in: {images_dir}")
    print(f"2. Use Nerfstudio or gsplat training scripts with the COLMAP output")
    print(f"\nRecommended command:")
    print(f"  ns-process-data video --data {output_dir}")
    print(f"  ns-train splatfacto --data {output_dir}/processed")


if __name__ == '__main__':
    main()