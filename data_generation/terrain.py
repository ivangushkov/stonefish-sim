import numpy as np
import matplotlib.pyplot as plt

# Definitions of the grid
Nx = 800
Ny = 800


# In pixels
sigma = 200**2

P = np.diag([sigma, sigma])
loc = np.array([(Nx-1)//2, (Ny-1)//2])

P_inv = np.linalg.inv(P)
det_P = np.linalg.det(P)

def gauss_pdf(pos):
    
    p = np.exp(-1/2 * (pos - loc).T @ P_inv @ (pos - loc)) / (np.sqrt((2*np.pi)**2 * det_P))
    return p

Img = np.zeros((Nx, Ny))


for i in range(Nx):
    for j in range(Ny):
        pos = np.array([i, j])
        Img[i, j] = gauss_pdf(pos)


max_val = np.max(Img)
Img = Img * 255.0/max_val

bruh = 1

plt.imsave("terrain.png", Img)