#!/usr/bin/python3
from sys import argv
import tarfile
import pandas as pd
import io
import matplotlib.pyplot as plt

if len(argv) != 2:
    print('Usage: ' + argv[0] + '<tar-gziped file>')

tar = tarfile.open(argv[1], 'r:gz')

points = pd.read_csv(io.TextIOWrapper(tar.extractfile('points.csv'), encoding='utf-8'), delimiter=',')
points.hist(edgecolor='black', linewidth=1.2)

points = csv.reader(io.TextIOWrapper(tar.extractfile('evolution.csv'), encoding='utf-8'), delimiter=',')
tar.close()

