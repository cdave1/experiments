#!/usr/bin/env python3
"""
MP4 to 3D Gaussian Splat Converter
Converts a video file to a 3D Gaussian Splat using Nerfstudio/Splatfacto
"""

import subprocess
import sys
import os
import argparse
from pathlib import Path


def check_dependencies():
    """Check if required dependencies are installed"""
    dependencies = {
        'ns-process-data': 'nerfstudio',
        'ns-train': 'nerfstudio',
        'ffmpeg': 'ffmpeg'
    }
    
    missing = []
    for cmd, package in dependencies.items():
        if subprocess.run(['which', cmd], capture_output=True).returncode != 0:
            missing.append(package)
    
    if missing:
        print(f"Missing dependencies: {', '.join(missing)}")
        print("\nInstall with:")
        print("  pip install nerfstudio")
        print("  # For CUDA support:")
        print("  pip install nerfstudio[cuda]")
        return False
    return True


def extract_frames(video_path, output_dir, fps=None):
    """Extract frames from video using ffmpeg"""
    print(f"Extracting frames from {video_path}...")
    
    output_dir = Path(output_dir)
    images_dir = output_dir / "images"
    images_dir.mkdir(parents=True, exist_ok=True)
    
    cmd = [
        'ffmpeg',
        '-i', str(video_path),
    ]
    
    if fps:
        cmd.extend(['-vf', f'fps={fps}'])
    
    cmd.extend([
        '-qscale:v', '2',  # High quality
        str(images_dir / 'frame_%04d.png')
    ])
    
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error extracting frames: {result.stderr}")
        return False
    
    print(f"Frames extracted to {images_dir}")
    return True


def process_data_colmap(data_dir):
    """Process data using COLMAP via Nerfstudio"""
    print("Processing data with COLMAP (this may take a while)...")
    
    cmd = [
        'ns-process-data',
        'video',
        '--data', str(data_dir),
        '--output-dir', str(data_dir / 'processed'),
        '--verbose'
    ]
    
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error processing data: {result.stderr}")
        return False
    
    print("Data processing complete!")
    return True


def train_gaussian_splat(data_dir, output_dir, max_num_iterations=30000):
    """Train 3D Gaussian Splat model using Splatfacto"""
    print("Training 3D Gaussian Splat model...")
    
    processed_dir = data_dir / 'processed'
    
    cmd = [
        'ns-train',
        'splatfacto',
        '--data', str(processed_dir),
        '--output-dir', str(output_dir),
        '--max-num-iterations', str(max_num_iterations),
        '--viewer.quit-on-train-completion', 'True'
    ]
    
    print(f"Running: {' '.join(cmd)}")
    result = subprocess.run(cmd)
    
    if result.returncode != 0:
        print("Error during training")
        return False
    
    print(f"Training complete! Model saved to {output_dir}")
    return True


def export_splat(config_path, output_path):
    """Export the trained model to .ply format"""
    print("Exporting Gaussian Splat...")
    
    cmd = [
        'ns-export',
        'gaussian-splat',
        '--load-config', str(config_path),
        '--output-dir', str(output_path)
    ]
    
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error exporting: {result.stderr}")
        return False
    
    print(f"Export complete! Splat saved to {output_path}")
    return True


def main():
    parser = argparse.ArgumentParser(
        description='Convert MP4 video to 3D Gaussian Splat'
    )
    parser.add_argument(
        'video_path',
        type=str,
        help='Path to input MP4 video'
    )
    parser.add_argument(
        '--output-dir',
        type=str,
        default='./output',
        help='Output directory for processed data and model'
    )
    parser.add_argument(
        '--fps',
        type=int,
        default=None,
        help='Extract frames at specific FPS (default: use all frames)'
    )
    parser.add_argument(
        '--iterations',
        type=int,
        default=30000,
        help='Number of training iterations (default: 30000)'
    )
    parser.add_argument(
        '--skip-dependencies-check',
        action='store_true',
        help='Skip checking for dependencies'
    )
    
    args = parser.parse_args()
    
    # Check dependencies
    if not args.skip_dependencies_check and not check_dependencies():
        sys.exit(1)
    
    # Validate input
    video_path = Path(args.video_path)
    if not video_path.exists():
        print(f"Error: Video file not found: {video_path}")
        sys.exit(1)
    
    # Create output directory
    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    
    # Step 1: Extract frames
    if not extract_frames(video_path, output_dir, args.fps):
        sys.exit(1)
    
    # Step 2: Process data with COLMAP
    if not process_data_colmap(output_dir):
        sys.exit(1)
    
    # Step 3: Train Gaussian Splat
    model_output = output_dir / 'models'
    if not train_gaussian_splat(output_dir, model_output, args.iterations):
        sys.exit(1)
    
    print("\n" + "="*50)
    print("SUCCESS! 3D Gaussian Splat generation complete!")
    print("="*50)
    print(f"\nModel location: {model_output}")
    print("\nTo view your model:")
    print(f"  ns-viewer --load-config {model_output}/splatfacto/*/config.yml")
    print("\nTo export to .ply format:")
    print(f"  ns-export gaussian-splat --load-config {model_output}/splatfacto/*/config.yml --output-dir {output_dir}/export")


if __name__ == '__main__':
    main()