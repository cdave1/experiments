#!/usr/bin/env python3
"""
MP4 to 3D Gaussian Splat Converter - MacBook Pro Optimized
Optimized for Apple Silicon (M1/M2/M3) with MPS acceleration
"""

import subprocess
import sys
import os
import argparse
from pathlib import Path
import platform


def detect_device():
    """Detect best available device for training"""
    import torch
    
    if platform.system() == 'Darwin':  # macOS
        if hasattr(torch.backends, 'mps') and torch.backends.mps.is_available():
            print("✓ Apple Silicon detected - will use MPS acceleration")
            return "mps"
        else:
            print("✓ Running on macOS - will use CPU")
            return "cpu"
    elif torch.cuda.is_available():
        print("✓ CUDA detected - will use GPU")
        return "cuda"
    else:
        print("✓ Will use CPU")
        return "cpu"


def check_dependencies():
    """Check if required dependencies are installed"""
    dependencies = {
        'colmap': 'COLMAP (install: brew install colmap)',
        'ffmpeg': 'ffmpeg (install: brew install ffmpeg)',
    }
    
    missing = []
    for cmd, package in dependencies.items():
        result = subprocess.run(['which', cmd], capture_output=True)
        if result.returncode != 0:
            missing.append(package)
    
    # Check Python packages
    try:
        import nerfstudio
        print("✓ Nerfstudio installed")
    except ImportError:
        missing.append("nerfstudio (install: pip install nerfstudio)")
    
    if missing:
        print("\n❌ Missing dependencies:")
        for dep in missing:
            print(f"   - {dep}")
        print("\nQuick install commands:")
        print("  brew install colmap ffmpeg")
        print("  pip install nerfstudio torch torchvision")
        return False
    
    print("✓ All dependencies found")
    return True


def extract_frames(video_path, output_dir, fps=2):
    """Extract frames from video using ffmpeg (optimized settings for Mac)"""
    print(f"\n📹 Extracting frames from {video_path}...")
    
    output_dir = Path(output_dir)
    images_dir = output_dir / "images"
    images_dir.mkdir(parents=True, exist_ok=True)
    
    # Lower FPS default for Mac (less frames = faster processing)
    cmd = [
        'ffmpeg',
        '-i', str(video_path),
        '-vf', f'fps={fps}',
        '-qscale:v', '2',
        str(images_dir / 'frame_%04d.png')
    ]
    
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"❌ Error extracting frames: {result.stderr}")
        return False
    
    frame_count = len(list(images_dir.glob('*.png')))
    print(f"✓ Extracted {frame_count} frames to {images_dir}")
    return True


def process_with_colmap(images_dir, output_dir):
    """Process images with COLMAP"""
    print("\n🎯 Running COLMAP (this will take a few minutes)...")
    
    output_dir = Path(output_dir)
    colmap_dir = output_dir / "colmap"
    colmap_dir.mkdir(parents=True, exist_ok=True)
    
    database_path = colmap_dir / "database.db"
    sparse_dir = colmap_dir / "sparse"
    sparse_dir.mkdir(parents=True, exist_ok=True)
    
    # Feature extraction
    print("  - Extracting features...")
    result = subprocess.run([
        'colmap', 'feature_extractor',
        '--database_path', str(database_path),
        '--image_path', str(images_dir),
        '--ImageReader.single_camera', '1',
        '--ImageReader.camera_model', 'OPENCV',
        '--SiftExtraction.use_gpu', '0'  # Use CPU on Mac
    ], capture_output=True, text=True)
    
    if result.returncode != 0:
        print(f"❌ COLMAP feature extraction failed: {result.stderr}")
        return False
    
    # Feature matching
    print("  - Matching features...")
    result = subprocess.run([
        'colmap', 'exhaustive_matcher',
        '--database_path', str(database_path),
        '--SiftMatching.use_gpu', '0'  # Use CPU on Mac
    ], capture_output=True, text=True)
    
    if result.returncode != 0:
        print(f"❌ COLMAP matching failed: {result.stderr}")
        return False
    
    # Sparse reconstruction
    print("  - Building sparse reconstruction...")
    result = subprocess.run([
        'colmap', 'mapper',
        '--database_path', str(database_path),
        '--image_path', str(images_dir),
        '--output_path', str(sparse_dir)
    ], capture_output=True, text=True)
    
    if result.returncode != 0:
        print(f"❌ COLMAP mapper failed: {result.stderr}")
        return False
    
    # Convert to text format for Nerfstudio
    model_dir = sparse_dir / "0"
    if model_dir.exists():
        print("  - Converting to text format...")
        subprocess.run([
            'colmap', 'model_converter',
            '--input_path', str(model_dir),
            '--output_path', str(model_dir),
            '--output_type', 'TXT'
        ], capture_output=True)
    
    print("✓ COLMAP processing complete!")
    return True


def train_gaussian_splat(data_dir, output_dir, iterations=10000):
    """Train 3D Gaussian Splat (optimized for Mac)"""
    print(f"\n🎓 Training Gaussian Splat model ({iterations} iterations)...")
    print("   This will take a while on Mac (30min - 2+ hours depending on your machine)")
    print("   Tip: Close other apps to free up memory\n")
    
    colmap_dir = data_dir / "colmap"
    model_output = output_dir / "models"
    
    # Check device
    try:
        detect_device()
    except:
        pass
    
    cmd = [
        'ns-train',
        'splatfacto',
        '--data', str(data_dir),
        '--output-dir', str(model_output),
        '--max-num-iterations', str(iterations),
        'colmap',
        '--colmap-model-path', str(colmap_dir / "sparse" / "0"),
        '--images-path', str(data_dir / "images")
    ]
    
    print(f"Running: {' '.join(cmd)}\n")
    result = subprocess.run(cmd)
    
    if result.returncode != 0:
        print("\n❌ Training failed")
        return False
    
    print(f"\n✓ Training complete! Model saved to {model_output}")
    return True


def main():
    parser = argparse.ArgumentParser(
        description='Convert MP4 to 3D Gaussian Splat (MacBook Pro optimized)',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s my_video.mp4
  %(prog)s my_video.mp4 --fps 3 --iterations 15000
  %(prog)s my_video.mp4 --output-dir ./my_splat --quick

Tips for Mac:
  - Use --quick for faster (but lower quality) results
  - Lower --fps means fewer frames = faster processing
  - Close other apps during training to free up RAM
        """
    )
    parser.add_argument('video_path', type=str, help='Path to input MP4 video')
    parser.add_argument('--output-dir', type=str, default='./output',
                       help='Output directory (default: ./output)')
    parser.add_argument('--fps', type=int, default=2,
                       help='Extract frames at FPS (default: 2 for Mac)')
    parser.add_argument('--iterations', type=int, default=10000,
                       help='Training iterations (default: 10000)')
    parser.add_argument('--quick', action='store_true',
                       help='Quick mode: fewer iterations (5000)')
    parser.add_argument('--skip-check', action='store_true',
                       help='Skip dependency check')
    
    args = parser.parse_args()
    
    print("="*60)
    print("🍎 MacBook Pro 3D Gaussian Splat Converter")
    print("="*60)
    
    # Check dependencies
    if not args.skip_check:
        if not check_dependencies():
            sys.exit(1)
    
    # Validate input
    video_path = Path(args.video_path)
    if not video_path.exists():
        print(f"❌ Video file not found: {video_path}")
        sys.exit(1)
    
    # Create output directory
    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    
    # Adjust iterations for quick mode
    iterations = 5000 if args.quick else args.iterations
    
    # Pipeline
    print(f"\n📋 Processing: {video_path.name}")
    print(f"   Output: {output_dir}")
    print(f"   FPS: {args.fps} | Iterations: {iterations}")
    
    # Step 1: Extract frames
    if not extract_frames(video_path, output_dir, args.fps):
        sys.exit(1)
    
    # Step 2: COLMAP processing
    images_dir = output_dir / "images"
    if not process_with_colmap(images_dir, output_dir):
        sys.exit(1)
    
    # Step 3: Train
    if not train_gaussian_splat(output_dir, output_dir, iterations):
        sys.exit(1)
    
    # Success!
    print("\n" + "="*60)
    print("🎉 SUCCESS! Your 3D Gaussian Splat is ready!")
    print("="*60)
    model_dir = output_dir / "models"
    print(f"\n📁 Model location: {model_dir}")
    print("\n📺 To view your model:")
    print(f"   ns-viewer --load-config {model_dir}/splatfacto/*/config.yml")
    print("\n💾 To export as .ply:")
    print(f"   ns-export gaussian-splat --load-config {model_dir}/splatfacto/*/config.yml")
    print("\n")


if __name__ == '__main__':
    main()