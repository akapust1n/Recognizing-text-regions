import os
from pathlib import Path
from shutil import copy
from predict import predict


home_dir = str(Path.home())
out_dir = home_dir + '/spam-train'


def get_images():
    files = []
    exts = ['jpg', 'png', 'jpeg', 'JPG']
    for parent, dirnames, filenames in os.walk(home_dir + '/spam-crop'):
        for filename in filenames:
            for ext in exts:
                if filename.endswith(ext):
                    files.append(os.path.join(parent, filename))
                    break
    return files


img_filenames = get_images()
for img_filename in img_filenames:
    result, message = predict(img_filename, ignore_detection=True)
    if len(result) > 0:
        copy(img_filename, out_dir)
