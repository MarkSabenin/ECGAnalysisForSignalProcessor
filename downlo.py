import kagglehub

# Download latest version
path = kagglehub.dataset_download("gamalasran/physionet-challenge-2020")

print("Path to dataset files:", path)
with open('path_to_pc.txt', 'w') as file:
    file.write(path)