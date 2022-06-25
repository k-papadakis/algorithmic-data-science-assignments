# %%
import numpy as np
from sklearn.metrics.pairwise import cosine_distances, cosine_similarity
from sklearn.metrics import pairwise_distances
# %%
X = np.array([
    [17/4, 2, 0, 1],
    [7/3, 2, 4, 1],
    [10/3, 7/2, 1, 0]
])

# cosine_distances(X)
pairwise_distances(X, metric='cosine')

# %%
X = np.array([    
    [1, 1, 0, 1, 0, 0, 1, 0],
    [0, 1, 1, 1, 0, 0, 0, 0],
    [0, 0, 0, 1, 0, 1, 1, 1]
]).T

J = pairwise_distances(X, metric='jaccard')
