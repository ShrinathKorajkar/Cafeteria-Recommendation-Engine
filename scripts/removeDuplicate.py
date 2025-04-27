import os

script_dir = os.path.dirname(os.path.abspath(__file__))

def read_and_write_unique_lines(file_path):
    try:
        unique_lines = set()

        with open(file_path, 'r') as file:
            for line in file:
                unique_lines.add(line.strip().lower())

        with open(file_path, 'w') as file:
            for line in sorted(unique_lines): 
                file.write(line + '\n')
        
        print("Removed duplicate lines from " + os.path.basename(file_path))

    except FileNotFoundError:
        print(f"The file {file_path} does not exist.")
    except IOError:
        print(f"An error occurred while reading or writing the file {file_path}.")

print()
# file_path1 = os.path.join(script_dir, "../../assets/positiveSentiments.csv")
# file_path2 = os.path.join(script_dir, "../../assets/negativeSentiments.csv")
# read_and_write_unique_lines(file_path1)
# read_and_write_unique_lines(file_path2)

# file_path_comments = os.path.join(script_dir, "../../assets/commentsDb.txt")
# read_and_write_unique_lines(file_path_comments)
print()
