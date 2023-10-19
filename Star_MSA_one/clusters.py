import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.datasets import make_blobs
from scipy.spatial.distance import cdist

# Define una matriz de distancias predefinida
# Aquí se muestra una matriz de ejemplo para 5 puntos
dist_matrix = np.array([[0, -15, -9, -8, -4, -5, -7, -10, -14, -3, -10, -10],
                        [-15, 0, -13, 2, -14, -3, -1, -11, -8, -12, -6, -16],
                        [-9, -13, 0, -9, -12, -9, -14, -7, -11, -13, -8, -7],
                        [-8, 2, -9, 0, -14, 0, -2, -6, -3, -6, -7, -8],
                        [-4, -14, -12, -14, 0, -9, -15, -11, -14, -9, -9, -9],
                        [-5, -3, -9, 0, -9, 0, -3, -3, -5, -3, -10, -6],
                        [-7, -1, -14, -2, -15, -3, 0, -3, -6, -3, -4, -8],
                        [-10, -11, -7, -6, -11, -3, -3, 0, -12, -9, -9, -8],
                        [-14, -8, -11, -3, -14, -5, -6, -12, 0, -7, -4, -3],
                        [-3, -12, -13, -6, -9, -3, -3, -9, -7, 0, -12, -6],
                        [-10, -6, -8, -7, -9, -10, -4, -9, -4, -12, 0, -8],
                        [-10, -16, -7, -8, -9, -6, -8, -8, -3, -6, -8, 0]])

kmeans = KMeans(n_clusters=2, init='k-means++', random_state=0)

kmeans.fit(dist_matrix)

labels = kmeans.labels_

nombres = ['AaBbCcDdEeFf'[i] for i in range(len(dist_matrix))]

arr = [[],[]]
for cluster_num in range(2):
    for i, label in enumerate(labels):
        if label == cluster_num:
            arr[cluster_num].append(nombres[i])

print("Cluster 1", arr[0])
print("Cluster 2", arr[1])
