#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import seaborn as sns
import os
import sys

plt.rcParams['figure.figsize'] = [8, 6]

def load_sol_tempo(loaded_arr):
    # This loadedArr is a 2D array, therefore
    # we need to convert it to the original
    # array shape.reshaping to get original
    # matrice with original shape.
    load_original_arr = loaded_arr.reshape(
        loaded_arr.shape[0], loaded_arr.shape[1] // 101, 101)

    return list(load_original_arr)

def main():
    h_x=0.001
    h_t=0.001
    x = np.arange(-0.05, 0.05+h_x/2, h_x)
    y = np.arange(-0.05, 0.05+h_x/2, h_x)

    file = sys.argv[1]
    if not os.path.isfile(file):
        print(f'{file} is not a file.')
        print(f'First argument is required. File with the solutions over time.')
        sys.exit(1)
    
    if len(sys.argv) < 3:
        print('Second argument is required. Directory where to save the images.')
        sys.exit(1)
    
    directory = sys.argv[2]
    os.makedirs(directory, exist_ok=True)

    sol_tempo = np.loadtxt(file)
    sol_tempo = load_sol_tempo(sol_tempo)

    x = x*1000
    y = y*1000
    x2, y2 = np.meshgrid(x, y)

    step = 10
    for k in range(0,len(sol_tempo),step):
        fig = plt.figure()
        ax = fig.gca(projection='3d')
        ax.plot_wireframe(x2,y2,sol_tempo[k], label='t='+"{:.0f} s".format(k*h_t*1000))
        fig.legend()
        ax.set_zlim(37, 55)
        ax.set_zlabel('T(°C)')
        ax.set_ylabel('y(mm)')
        ax.set_xlabel('x(mm)')

        plt.savefig(f'{directory}/sol_tempo_{k}.png')
        # plt.show()
        plt.close()
    
    sns.heatmap(sol_tempo[-1].T, xticklabels=x, yticklabels=y)
    ax = plt.gca()
    ax.invert_yaxis()
    ax.set_title('Temperature in Celsius in the final state')
    ax.set_xlabel('x(mm)')
    ax.set_ylabel('y(mm)')

    plt.savefig('heatmap.png')
    # plt.show()

if __name__=='__main__': main()
