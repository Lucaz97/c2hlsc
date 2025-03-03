import os
import re
import argparse

def rename_folders(path, offset):
    # Validate path
    if not os.path.isdir(path):
        print(f"Error: {path} is not a valid directory")
        return

    # Compile regex pattern to match folder_<number>
    pattern = re.compile(r'.*_(\d+)$')
    log_pattern = re.compile(r'.*_(\d+)\.log$')
    folders = []
    
    # Collect matching folders and their numbers
    for entry in os.listdir(path):
        entry_path = os.path.join(path, entry)
        if os.path.isdir(entry_path):
            match = pattern.match(entry)
            if match:
                number = int(match.group(1))
                prefix = entry[:match.start(1)-1]
                folders.append((entry, number, prefix, ""))
        elif os.path.isfile(entry_path):
            match = log_pattern.match(entry)
            if match:
                number = int(match.group(1))
                prefix = entry[:match.start(1)-1]
                folders.append((entry, number, prefix, ".log"))
    if not folders:
        print("No matching folders found")
        return

    # Sort folders in reverse numerical order to prevent conflicts
    folders.sort(key=lambda x: x[1], reverse=True)

    # Rename folders with offset
    for old_name, old_num, prefix, postfix in folders:
        new_num = old_num + offset
        print(prefix)
        new_name = prefix + "_" +str(new_num) + postfix
        old_path = os.path.join(path, old_name)
        new_path = os.path.join(path, new_name)
        
        if os.path.exists(new_path):
            print(f"Conflict: {new_name} already exists. Skipping {old_name}")
            continue
            
        os.rename(old_path, new_path)
        print(f"Renamed: {old_name} -> {new_name}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Rename numbered folders with offset')
    parser.add_argument('path', help='Directory path containing the folders')
    parser.add_argument('offset', type=int, help='Numerical offset to add to folder numbers')
    
    args = parser.parse_args()
    
    rename_folders(args.path, args.offset)