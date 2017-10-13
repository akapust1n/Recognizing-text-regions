import os
import time
from pathlib import Path

import cv2
import numpy as np
import tensorflow as tf
from keras import backend as K
from keras.models import load_model
from scipy.misc import imread, imresize

import lanms
import model
from icdar import restore_rectangle


def detect(score_map, geo_map, timer, score_map_thresh=0.8, box_thresh=0.1, nms_thres=0.2):
    '''
    restore text boxes from score map and geo map
    :param score_map:
    :param geo_map:
    :param timer:
    :param score_map_thresh: threshhold for score map
    :param box_thresh: threshhold for boxes
    :param nms_thres: threshold for nms
    :return:
    '''
    if len(score_map.shape) == 4:
        score_map = score_map[0, :, :, 0]
        geo_map = geo_map[0, :, :, ]
    # filter the score map
    xy_text = np.argwhere(score_map > score_map_thresh)
    # sort the text boxes via the y axis
    xy_text = xy_text[np.argsort(xy_text[:, 0])]
    # restore
    start = time.time()
    text_box_restored = restore_rectangle(xy_text[:, ::-1] * 4, geo_map[xy_text[:, 0], xy_text[:, 1], :])  # N*4*2
    # print('{} text boxes before nms'.format(text_box_restored.shape[0]))
    boxes = np.zeros((text_box_restored.shape[0], 9), dtype=np.float32)
    boxes[:, :8] = text_box_restored.reshape((-1, 8))
    boxes[:, 8] = score_map[xy_text[:, 0], xy_text[:, 1]]
    timer['restore'] = time.time() - start
    # nms part
    start = time.time()
    # boxes = nms_locality.nms_locality(boxes.astype(np.float64), nms_thres)
    boxes = lanms.merge_quadrangle_n9(boxes.astype('float32'), nms_thres)
    timer['nms'] = time.time() - start
    if boxes.shape[0] == 0:
        return None, timer
    # here we filter some low score boxes by the average score map, this is different from the orginal paper
    for i, box in enumerate(boxes):
        mask = np.zeros_like(score_map, dtype=np.uint8)
        cv2.fillPoly(mask, box[:8].reshape((-1, 4, 2)).astype(np.int32) // 4, 1)
        boxes[i, 8] = cv2.mean(score_map, mask)[0]
    boxes = boxes[boxes[:, 8] > box_thresh]
    return boxes, timer


def sort_poly(p):
    min_axis = np.argmin(np.sum(p, axis=1))
    p = p[[min_axis, (min_axis + 1) % 4, (min_axis + 2) % 4, (min_axis + 3) % 4]]
    if abs(p[0, 0] - p[1, 0]) > abs(p[0, 1] - p[1, 1]):
        return p
    else:
        return p[[0, 3, 2, 1]]


def resize_image(im, max_side_len=1024):
    '''
    resize image to a size multiple of 32 which is required by the network
    :param im: the resized image
    :param max_side_len: limit of max image size to avoid out of memory in gpu
    :return: the resized image and the resize ratio
    '''
    h, w, _ = im.shape
    resize_w = w
    resize_h = h
    # limit the max side
    if max(resize_h, resize_w) > max_side_len:
        ratio = float(max_side_len) / resize_h if resize_h > resize_w else float(max_side_len) / resize_w
    else:
        ratio = 1.
    resize_h = int(resize_h * ratio)
    resize_w = int(resize_w * ratio)
    resize_h = resize_h if resize_h % 32 == 0 else (resize_h // 32 - 1) * 32
    resize_w = resize_w if resize_w % 32 == 0 else (resize_w // 32 - 1) * 32
    im = cv2.resize(im, (int(resize_w), int(resize_h)))
    ratio_h = resize_h / float(h)
    ratio_w = resize_w / float(w)
    return im, (ratio_h, ratio_w)


def rgb2gray(rgb):
    return np.dot(rgb[..., :3], [0.299, 0.587, 0.114])


os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
home_dir = str(Path.home())

with K.name_scope('a'):
    my_model = load_model(home_dir + '/resnet18/resnet18.h5')

g = tf.Graph()
sess = tf.Session(graph=g, config=tf.ConfigProto(allow_soft_placement=True))
with K.name_scope('b'), g.as_default(), sess.as_default():
    input_images = tf.placeholder(tf.float32, shape=[None, None, None, 3], name='input_images')
    global_step = tf.get_variable('global_step', [], initializer=tf.constant_initializer(0), trainable=False)
    f_score, f_geometry = model.model(input_images, is_training=False)
    variable_averages = tf.train.ExponentialMovingAverage(0.997, global_step)
    saver = tf.train.Saver(variable_averages.variables_to_restore())
    checkpoint_path = home_dir + '/east_icdar2015_resnet_v1_50_rbox'
    ckpt_state = tf.train.get_checkpoint_state(checkpoint_path)
    model_path = os.path.join(checkpoint_path, os.path.basename(ckpt_state.model_checkpoint_path))
    saver.restore(sess, model_path)


def predict(file, return_image=False):
    image = imread(file, mode='RGB')
    print('detecting text...')
    file_read_time = time.time()
    im_formatted = rgb2gray(image)
    im_formatted = imresize(im_formatted, (224, 224))
    im_formatted = im_formatted.astype('float32')
    im_formatted /= 255
    im_formatted = im_formatted.reshape(1, 224, 224, 1)
    with K.name_scope('a'):
        prediction = my_model.predict(im_formatted)
    end_time = time.time()
    has_text = prediction[0][0] > 0.9
    if has_text:
        print('has text')
    else:
        print('no text')
    print('calculation time (s): ', end_time - file_read_time)
    result = []
    if has_text:
        print('locating text...')
        start_time = time.time()
        im_resized, (ratio_h, ratio_w) = resize_image(image)
        timer = {'net': 0, 'restore': 0, 'nms': 0}
        start = time.time()
        with K.name_scope('b'), g.as_default(), sess.as_default():
            score, geometry = sess.run([f_score, f_geometry], feed_dict={input_images: [im_resized]})
        timer['net'] = time.time() - start
        boxes, timer = detect(score_map=score, geo_map=geometry, timer=timer)
        print('net {:.0f}ms, restore {:.0f}ms, nms {:.0f}ms'.format(
            timer['net'] * 1000, timer['restore'] * 1000, timer['nms'] * 1000))
        if boxes is not None:
            boxes = boxes[:, :8].reshape((-1, 4, 2))
            boxes[:, :, 0] /= ratio_w
            boxes[:, :, 1] /= ratio_h
        duration = time.time() - start_time
        print('[timing] {}'.format(duration))
        if boxes is not None:
            for box in boxes:
                # to avoid submitting errors
                box = sort_poly(box.astype(np.int32))
                if np.linalg.norm(box[0] - box[1]) >= 5 and np.linalg.norm(box[3] - box[0]) >= 5:
                    result.append(
                        '{},{},{},{},{},{},{},{}'.format(box[0, 0], box[0, 1], box[1, 0], box[1, 1],
                                                         box[2, 0], box[2, 1], box[3, 0], box[3, 1]))
                    if return_image:
                        cv2.polylines(image, [box.astype(np.int32).reshape((-1, 1, 2))], True,
                                      color=(0, 0, 255), thickness=1)
        else:
            print('no text')
    if return_image:
        return result, image
    return result


if __name__ == '__main__':
    while True:
        path = input('img path: ')
        try:
            res, img = predict(path, True)
            print(res)
            cv2.imwrite('/tmp/text_located.jpg', img)
        except OSError as e:
            print(e)
