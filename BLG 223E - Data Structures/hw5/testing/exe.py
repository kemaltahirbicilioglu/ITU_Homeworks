import os

for g in ("group1", "group2", "group3"):
    for f in os.listdir(g):
        file_name = os.path.join(g, f)
        os.system(f"g++ -Wall -Werror {file_name} -o bin/{file_name[:-4]}")
        
