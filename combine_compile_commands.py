import os
import json
import argparse

def find_compile_commands(start_dir):
    """递归查找所有 compile_commands.json 文件"""
    matches = []
    for root, dirs, files in os.walk(start_dir):
        if "compile_commands.json" in files:
            path = os.path.join(root, "compile_commands.json")
            print(f"Found: {path}")
            matches.append(path)
    return matches

def merge_json_files(file_paths):
    """合并多个 compile_commands.json 文件"""
    merged = []
    for path in file_paths:
        try:
            with open(path, 'r', encoding='utf-8') as f:
                data = json.load(f)
                if isinstance(data, list):
                    merged.extend(data)
                else:
                    print(f"Warning: {path} is not an array, skipping.")
        except json.JSONDecodeError as e:
            print(f"Error decoding JSON in {path}: {e}")
    return merged

def write_output(data, output_path):
    """写入合并后的结果"""
    print(f"Writing merged compile_commands to {output_path}...")
    with open(output_path, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2)
    print(f"✅ Merged compile_commands written to: {output_path}")

def main():
    parser = argparse.ArgumentParser(description="Merge all compile_commands.json files in subdirectories.")
    parser.add_argument("--root", default=".", help="Root directory to start searching (default: .)")
    parser.add_argument("--output", default="compile_commands.json", help="Output file path (default: ./compile_commands.json)")
    args = parser.parse_args()

    print(f"🔍 Searching for compile_commands.json under '{args.root}'...")
    files = find_compile_commands(args.root)
    if not files:
        print("❌ No compile_commands.json found.")
        return

    print("🔄 Merging files...")
    merged_data = merge_json_files(files)

    print("💾 Writing output...")
    write_output(merged_data, args.output)

if __name__ == "__main__":
    main()