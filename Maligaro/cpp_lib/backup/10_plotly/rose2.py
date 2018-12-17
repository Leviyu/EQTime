import itertools
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from matplotlib.collections import PatchCollection
import numpy as np

def main():
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='polar')
    x = np.radians(np.arange(0, 360, 10))
    y = np.random.random(x.size)
    z = np.random.random(y.size)
    cmap = plt.get_cmap('cool')
    coll = colored_bar(x, y, z, ax=ax, width=np.radians(10), cmap=cmap)
    fig.colorbar(coll)
    ax.set_yticks([0.5, 1.0])
    plt.show()   

def colored_bar(left, height, z=None, width=0.8, bottom=0, ax=None, **kwargs):
    if ax is None:
        ax = plt.gca()
    width = itertools.cycle(np.atleast_1d(width))
    bottom = itertools.cycle(np.atleast_1d(bottom))
    rects = []
    for x, y, h, w in zip(left, bottom, height, width):
        rects.append(Rectangle((x,y), w, h))
    coll = PatchCollection(rects, array=z, **kwargs)
    ax.add_collection(coll)
    ax.autoscale()
    return coll

if __name__ == '__main__':
    main()
