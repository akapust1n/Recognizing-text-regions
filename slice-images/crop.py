import os
from pathlib import Path

from PIL import Image

home_dir = str(Path.home())
out_dir = home_dir + '/spam-crop'
size = 224


def get_images():
    files = []
    exts = ['jpg', 'png', 'jpeg', 'JPG']
    for parent, dirnames, filenames in os.walk(home_dir + '/spam-5k'):
        for filename in filenames:
            for ext in exts:
                if filename.endswith(ext):
                    files.append(os.path.join(parent, filename))
                    break
    return files


img_filenames = get_images()
cropped_images = []
for img_filename in img_filenames:
    image = Image.open(img_filename)
    w, h = image.size
    if w > size and h > size:
        cropped_images.append(image.crop((0, 0, size, size)))
        cropped_images.append(image.crop((w - size, 0, w, size)))
        cropped_images.append(image.crop((0, h - size, size, h)))
        cropped_images.append(image.crop((w - size, h - size, w, h)))
    elif w > size >= h:
        cropped_images.append(image.crop((0, 0, size, h)))
        cropped_images.append(image.crop((w - size, 0, w, h)))
    elif w <= size < h:
        cropped_images.append(image.crop((0, 0, w, size)))
        cropped_images.append(image.crop((0, h - size, w, h)))
    else:
        cropped_images.append(image)

i = 0
for cropped_image in cropped_images:
    cropped_image.save(f'{out_dir}/{i}.jpg')
    i += 1
