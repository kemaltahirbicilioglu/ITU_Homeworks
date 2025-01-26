import os
import re
import subprocess
import matplotlib.pyplot as plt

# os system should be converted to subprocess.run
# capture return of the subprocess.run , regex \d+\.\d+



for operation in ("add", "update", "delete"):
    for g in range(1, 4):
        directory_name = fr".\bin\group{g}"
        plt.clf() 
        for f in os.listdir(directory_name):
            file_name = os.path.join(directory_name, f)
            values = []

            r = list(range(10,60,5))

            for k in r:
                info = subprocess.run([fr"{file_name}", fr".\dataset\{k}k_employee.csv", fr".\dataset\{k}k_{operation}_file.txt"], capture_output = True)
                result = re.search(r"\d+", info.stdout.decode())
                value = result.group(0)
                values.append(int(value))

            
            print(values)
            plt.plot(list(f"{i}k" for i in r), values, label = f[:-4])

        plt.ylabel("Measured time (ms)")
        plt.xlabel("Data size")
        plt.title(f"{operation if operation != 'update' else 'search'} group{g}")
        plt.legend()
        plt.savefig(f"./graphs/{operation} group{g}.png")
        #plt.show()    
        print("-------")

