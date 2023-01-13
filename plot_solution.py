#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import seaborn as sns
import os

plt.rcParams['figure.figsize'] = [8, 6]

os.makedirs('figs', exist_ok=True)

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
    delta_t = 300
    x = np.arange(-0.05, 0.05+h_x/2, h_x)
    y = np.arange(-0.05, 0.05+h_x/2, h_x)
    t = np.arange(0, delta_t+h_t/2, h_t)

    u = np.loadtxt("u_h_x_0.001_h_t_0.001_300000_steps.txt")

    loaded_arr = np.loadtxt("sol_tempo_h_x_0.001_h_t_0.001_300000_steps.txt")
    sol_tempo = load_sol_tempo(loaded_arr)
    del loaded_arr

    x2, y2 = np.meshgrid(x, y)

    step = 10
    for k in range(0,len(sol_tempo),step):
        fig = plt.figure()
        ax = fig.gca(projection='3d')
        ax.plot_wireframe(x2,y2,sol_tempo[k], label='t='+"{:.0f} s".format(k*h_t*1000))
        fig.legend()
        ax.set_zlim(37)
        ax.set_ylabel('y')
        ax.set_xlabel('x')

        plt.savefig(f'figs/sol_tempo_{k}.png')
        # plt.show()
        plt.close()
    
    sns.heatmap(u.T)
    ax = plt.gca()
    ax.invert_yaxis()
    plt.savefig('heatmap.png')
    # plt.show()

if __name__=='__main__': main()